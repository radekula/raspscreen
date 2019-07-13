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




#include <iostream>
#include <raspscreen.hpp>
#include <signal.h>




int main(int argc, char* argv[])
{
    auto config = raspscreen::config::Config::instance();

    // Set default config
    config->set("HOST", "0.0.0.0");
    config->set("PORT", "8080");
    config->set("I2C_DEVICE", "0x27");
    config->set("LCD_WIDTH", "20");
    config->set("LCD_HEIGHT", "4");
    config->set("LCD_LINE_LENGTH", "20");

    // Try to load configuration from file
    try
    {
        config->load("/etc/raspscreen/raspscreen.conf");
    }
    catch(...)
    {
    }

    raspscreen::log::Logger::log("Application init");
    raspscreen::app::App::get()->init(argc, argv);

    raspscreen::log::Logger::log("Starting application");
    raspscreen::app::App::get()->run();
};




