/*
    Raspscreen
    Copyright (C) 2018  Radosław Ulatowski

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include <rasplib.hpp>





namespace raspscreen {
namespace config {




Config::Config()
{
	instance = 0;
};




Config::~Config()
{
};




raspscreen::config::Config* Config::instance()
{
	if(instance == 0)
        instance = new Config();

    return instance;
};




void Config::load(std::string file_name)
{
	std::ifstream file(file.c_str(), std::ios::binary);
	
	if(file.is_open())
	{
		std::string line;

		while(getline(file, line))
		{
			std::regex();
			
			
		}
		file.close();
	}
};




void Config::clear()
{
	config_map.clear();
};




std::string Config::get(std::string param_name)
{
	auto param = config_map.find(param_name);

    if(param != config_map.end())
        return param->second;

    return std::string();
};




void Config::set(std::string param_name, std::string value)
{
	config_map[param_name] = value;
};



	
}
}




#endif
