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

#ifndef __RASPSCREEN_APP_HPP__
#define __RASPSCREEN_APP_HPP__

#include <rasplib.hpp>
#include <screen/screen.hpp>
#include <restsrv.hpp>




namespace raspscreen {
namespace app {




class App
{
private:
    static App *instance;                                                           //< Singleton instance handler
    bool running;                                                                   //< Control if main loop is running

private:
    std::mutex thread_lock;                                                         //< Control access to screens between threads
    std::unique_ptr<rasplib::display::Alphanumeric> lcd;                            //< Display object
    std::map<std::string, std::shared_ptr<raspscreen::screen::Screen>> screens;     //< Map of screens to display
    std::shared_ptr<raspscreen::screen::Screen> last_screen;                        //< Last displayed screen //TODO: make list of lasts screens

private:
    /**
     * Rest handler function
     */
    static void rest_handler();

    /**
     * Request handler for rest handler function
     */
    static void request_handler(rest::server::RestRequest &request, rest::server::RestResponse &response);

    /**
     * Handle new screen
     */
    void new_screen(Json::Value &screen);

    /**
     * Update screen with new screen
     */
    void update_screen();

private:
    App();

public:
    ~App();

public:
    /**
     * Singleton class - get object
     */
    static raspscreen::app::App *get();

    /**
     * Init aplication
     */
    void init(int argc, char* argv[]);

public:
    /**
     * Run applicaiton main loop
     */
    void run();
};




}
}

#endif
