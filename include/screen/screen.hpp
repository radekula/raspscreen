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

#ifndef __RASPSCREEN_SCREEN_HPP__
#define __RASPSCREEN_SCREEN_HPP__

#include <chrono>
#include <string>




namespace raspscreen {
namespace screen {




class Screen
{
public:
    unsigned int priority;
    std::chrono::system_clock::time_point valid_until;

public:
    std::string name;
    std::string text;

public:
    Screen();
    ~Screen();
};




}
}

#endif

