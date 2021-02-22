#pragma once
#include <vector>
#include <iostream>
#include <thread>
#include <atomic>
#include <utility>
#include <chrono>


// Simple array of byte long bools
// Created in way that would be possible to pack bools in to byte
// But it was painful in openGL to unpack.
class packedBool {
public:
	packedBool() {
		data = nullptr;
	}
	packedBool(size_t size) : packedBool(){
		reserve(size);
	}
	~packedBool() {
		delete[] data;
	}

	//reallocs buffer (without copy)
	void reserve(size_t size) {
		if (data != nullptr) {
			delete[] data;
		}
		data = new char[size]; //TODO roud up

		for (size_t i = 0; i < size; i++) {
			data[i] = 0;
		}
			
	}
	bool get(size_t idx) const {
		return data[idx];
	}
	void set(size_t idx, bool val) {
		data[idx] = val;
	}
	//returns raw ptr to data
	const char* get_ptr() const {
		return data;
	}
private:
	char* data;
};

using namespace std::chrono;

// Game Of Life threaded engine.
// Multithreaded implementation of conway's gol
// Call 'init' before usage.
class gameoflife_engie
{	
	std::thread work;
	std::atomic<bool> isDone = true;
public:
	gameoflife_engie() = default;

	packedBool data[2];        // Data. double array makes thread-safe writes and reads. (always one array is for writing and other for reading)
	size_t value = 0;          // Counter for keeping track whitch value data is for read and witch is for write.
	size_t num_of_threads = 8; // Num of threads that system will work.
	double delta_time = 0.016; // Delta time of last execution.

	// Can fail on alloc. TODO - keep track of it.
	void init(size_t w, size_t h, size_t num_of_threads) {
		this->w = w;
		this->h = h;
		this->num_of_threads = num_of_threads;

		data[0].reserve(get_size());
		data[1].reserve(get_size());
	}

	// Fills engine with random values. TODO - make it pallarel. (huge lag on start)
	void fill_random(float ratio = 0.5) {
		for (size_t i = 0; i < get_size(); i++)
		{
			data[0].set(i, ratio < (rand() % 1000) / 1000.0f);
			data[1].set(i, ratio < (rand() % 1000) / 1000.0f);
		}
		value = 0;
	}

	[[nodiscard]] bool isJobDone() const
	{
		return isDone.load();
	}

	// Queues threaded job to calculate and detaches it. isJobDone will return false as long as the queue is not empty.
	// If job is arleady queued will print Skipping in console TODO - throw exeption here.
	void queue_job()
	{
		if (isJobDone()) {
			work = std::thread(&gameoflife_engie::Update, this);
			work.detach();
		}
		else
		{
			std::cout << "Skiping" << std::endl;
		}
	}

	//Do not use it. spawns threadpool and waits for execution. Could broke if 'queue_job' was used.
	void Update()
	{
		auto start_time = high_resolution_clock::now();
		isDone = false;
		std::vector<std::thread> threads_pool; //thread pool.
		threads_pool.reserve(num_of_threads);

		const auto chunk_size = get_size()/ num_of_threads;

		if (chunk_size * num_of_threads != get_size()) {
			std::cout << "BAD SIZE" << std::endl;
			return;
		}

//d1 = [0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0]   data arr used for read (curr frame)
//      ^^^^^^^  ^^^^^^^  ^^^^^^^  ^^^^^^^
//        W1       W2       W3       W4     ... Will spawn num_of_threads jobs.
//        \/       \/       \/       \/
//d2 = [1,0,1,0, 1,0,1,0, 0,0,1,0, 1,0,0,0]  data arr used for write (next frame)

		for (size_t i = 0; i < num_of_threads; i++)
		{
			size_t start = i* chunk_size;            //start of chunk
			size_t end = i* chunk_size + chunk_size; //end of chunk

			threads_pool.emplace_back(
				std::thread(
					&gameoflife_engie::process_chunk, this,
					start,
					end,
					std::ref(data[value]),
					std::ref(data[(value + 1) % 2])
				)
			);

		}

		//join threads.
		size_t counter = 0;
		while (counter != num_of_threads) {
			for (size_t i = 0; i < num_of_threads; i++)
			{
				if(threads_pool[i].joinable()){
					threads_pool[i].join();
					counter++;
				}
			}
		}

		value = (value+1)%2;

		isDone = true;
		auto end_time = high_resolution_clock::now();

		delta_time = (end_time - start_time) / 1.0ms;
	}

	//Will process chunk of data.
	void process_chunk(size_t start, size_t end, packedBool& src, packedBool& dst) 
	{
		for (size_t i = start; i < end; i++)
		{
			dst.set(i, process_cell(src, i));
		}
	}

	//calculates gameoflife iteration on i'th index of flatted arr.
	bool process_cell(packedBool& src, size_t i) {
		auto [x, y] = flat_to_cords(i);
		if (x == 0 || y == 0 || x == w - 1 || y == h - 1) {
			return false;
		}
		size_t neighbours = 0;
		neighbours += src.get(map(x - 1, y - 1));
		neighbours += src.get(map(x, y - 1));
		neighbours += src.get(map(x + 1, y - 1));
		neighbours += src.get(map(x - 1, y));
		neighbours += src.get(map(x + 1, y));
		neighbours += src.get(map(x - 1, y + 1));
		neighbours += src.get(map(x, y + 1));
		neighbours += src.get(map(x + 1, y + 1));

		bool value = src.get(i);
		switch (neighbours)
		{
		case 0:
			return false;
		case 1:
			return false;
		case 2:
			return value;
		case 3:
			return true;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			return false;
		}
	}

	// Raw pointer to current iteration.
	const char* get_ptr() const {
		return data[value].get_ptr();
	}

	// Flatenss cords.
	size_t map(size_t x, size_t y) {
		return w * x + y;
	}
	
	// reshapes flat cord to texture space.     x, y = flat_to_cords(map(x, y));
	std::pair<size_t, size_t> flat_to_cords(size_t idx) const {
		return std::make_pair(idx / w, idx % h);
	}
	size_t get_size() const {
		return w * h;
	}
	size_t get_w() const {
		return w;
	}
	size_t get_h() const {
		return h;
	}


private:
	size_t w;
	size_t h;
};
