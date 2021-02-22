#pragma once
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include <utility>

// App Commandline Arguments parser.
// It is supposed to build 'parsed_args' object of type std::map<std::string, std::variant<...>>
// Pass C style array to parse_arguments to parse them.
// You have to pass default init for parsed_args in constructor.
class ArgParser
{
public:
	using value = std::variant<int, float, bool, std::string>; 
	using map_type = std::map<std::string, value>;
private:
	bool starts_with(const std::string& t, std::string&& prefix)
	{
		return t.find_first_of(prefix) == 0;
	}
	value get_value(const std::string& str_val)
	{
		if (starts_with(str_val, "--"))
		{
			return true;
		}

		if (str_val.find('.') != -1) {
			try {
				return std::stof(str_val);
			}
			catch (std::invalid_argument&){}
		}
		
		try {
			return std::stoi(str_val);
		}
		catch (std::invalid_argument&){}
		
		return str_val;
	}
public:
	map_type parsed_args;
	ArgParser(map_type&& args) : parsed_args(std::move(args)){}

	// Shows small help message.
	void give_help()
	{
		std::cout << "Argument parsing usage:" << std::endl;
		std::cout << ".\\GameOfLife.exe --arg1 param1 --arg2 --arg3 param2" << std::endl;
		std::cout << "Possible arguments:" << std::endl;
		for (auto& [key, val] : parsed_args)
		{
			std::cout << "--" << key << std::endl;
		}
	}
	
	// Updates 'parsed_args' based on C++ commandline arguments.
	void parse_arguments(const int num, const char** args_c)
	{
		std::vector<std::string> args(num);
		
		for (size_t i = 0; i < num; i++)
		{
			args.emplace_back(std::string(args_c[i]));
		}

		for (size_t i = 0; i < args.size(); i++)
		{

			if(starts_with(args[i], "--"))
			{
				auto command = args[i].substr(2);

				if (command == "help") {
					give_help();
					return;
				}
				
				value value = true;
				try
				{
					if (i < args.size()-1)
						value = get_value(args[i + 1]);
					else
						value = true;
				}
				catch(std::invalid_argument&){}

				if (parsed_args.find(command) == parsed_args.end())
					throw std::exception((std::string("Unresolved argument: --") + command).c_str());
				parsed_args[command] = value;
			}
		}
		
	}
};
   