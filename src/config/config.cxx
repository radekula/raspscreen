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


#include <fstream>
#include <stdexcept>
#include <string>
#include <regex>
#include <raspscreen.hpp>





namespace raspscreen {
namespace config {




raspscreen::config::Config *raspscreen::config::Config::handler = 0;





Config::Config()
{
};




Config::~Config()
{
};




raspscreen::config::Config* Config::instance()
{
    if(handler == 0)
        handler = new Config();

    return handler;
};




void Config::load(std::string file_name)
{
    std::ifstream file(file_name.c_str(), std::ios::binary);

    if(!file.is_open())
        throw std::runtime_error(std::string("Could not open file: ") + file_name);

    // Stored section name
    std::string section_name;

    // Regex to find section name
    std::regex section("^[ \\t]*\\[(.*)\\][ \\t]*$");

    // Regex to find empty line
    std::regex empty_line("^[\\s]*$");

    // Regex to find param with value
    std::regex param("^[ \\t]*([a-zA-Z0-9_^#]*)[ \\t]*=[ \\t]*([^#^\\n]*).*$");

    std::string line;
    std::smatch match;

    while(std::getline(file, line))
    {
        // Check if line contains section name and set it if found
        if(std::regex_search(line, match, section))
        {
            section_name = match.str(1) + ".";
            continue;
        };

        // Check if line is empty and unset section
        if(std::regex_search(line, match, empty_line))
        {
            section_name = "";
            continue;
        };

        // Check if line contains param with value and set it if found
        if(std::regex_search(line, match, param))
        {
            config_map[section_name + match.str(1)] = match.str(2);
            continue;
        };
    };

    file.close();
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
