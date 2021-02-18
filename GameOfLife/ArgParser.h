#pragma once
#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include <utility>
#include <valarray>

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
	ArgParser(map_type&& args) : parsed_args(std::move(args)){}
	map_type parsed_args;

	
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
				value value = true;
				try
				{
					if (i < args.size())
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
   