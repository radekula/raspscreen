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

#ifndef __RASPSCREEN_CONFIG_HPP__
#define __RASPSCREEN_CONFIG_HPP__




#include <map>




namespace raspscreen {
namespace config {




class Config
{
private:
    static Config *handler;                                             //< Singleton instance handler
    std::map<std::string, std::string> config_map;                      //< stores configuration values in map format

private:
    Config();

public:
    ~Config();

public:
    /**
     * Singleton class - get object
     */
    static raspscreen::config::Config *instance();

public:
    /**
     * Load configuration from file
     *
     * @param file_name Path to file with configuration
     */
    void load(std::string file_name);

    /**
     * Clear / remove stored configuration from memory
     */
    void clear();

public:
    /**
     * Get value for given parameter
     *
     * @return Value as string
     */
    std::string get(std::string param_name);

    /**
     * Set / update parameter with value
     *
     * @param param_name Name of parameter
     * @param value new value
     */
    void set(std::string param_name, std::string value);
};




}
}




#endif
