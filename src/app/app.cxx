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

#include <app/app.hpp>
#include <log/logger.hpp>




namespace raspscreen {
namespace app {




raspscreen::app::App *raspscreen::app::App::instance = 0;




App::App()
{
    // TODO: Make all constants default variables that can be passed from config file / command line
    int lcd_width = 20;
    int lcd_height = 4;
    int lcd_line_lenght = 20;

    bool mode = true;
    bool cursor = false;
    bool blink = false;

    auto address = std::bitset<8>(0x27);

    raspscreen::log::Logger::log(
        std::string("Set LCD parameters:")
        + "width: " + std::to_string(lcd_width)
        + ", height: " + std::to_string(lcd_height)
        + ", line_lenght: " + std::to_string(lcd_line_lenght)
    );
    lcd = std::make_unique<rasplib::display::Alphanumeric>(lcd_width, lcd_height, lcd_line_lenght);

    raspscreen::log::Logger::log("Set LCD address: " + address.to_string());
    lcd->init(1, address);

    raspscreen::log::Logger::log(std::string("Set LCD mode:")
        + "fullscreen: " + std::to_string(mode)
        + ", cursor: " + std::to_string(cursor)
        + ", blinking: " + std::to_string(blink)
    );
    lcd->set_mode(mode, cursor, blink);
};




App::~App()
{
};




raspscreen::app::App* App::get()
{
    if(instance == 0)
        instance = new App();

    return instance;
};




void App::init(int argc, char* argv[])
{
};




void App::rest_handler()
{
    // TODO: get port from config / command line
    int port = 8080;

    raspscreen::log::Logger::log(std::string("Starting REST interface on port: ") + std::to_string(port));
    rest::server::RestSrv srv(request_handler);
    srv.set_port(port);

    while(1)
    {
        try
        {
            srv.run();
        }
        catch(...)
        {
        };
    }
};



void App::request_handler(rest::server::RestRequest &request, rest::server::RestResponse &response)
{
    if(request.resource() != "/screen")
    {
        response.set_http_code(404);
        return;
    };

    if(request.method() != "POST")
    {
        response.set_http_code(405);
        return;
    };

    raspscreen::log::Logger::log("New screen request");
    raspscreen::app::App::get()->new_screen(request.body());
    response.set_http_code(200);
};




void App::new_screen(Json::Value &screen)
{
    raspscreen::log::Logger::log("Waiting for mutex");
    std::lock_guard<std::mutex> guard(thread_lock);
    raspscreen::log::Logger::log("Mutex acquired");

    auto now = std::chrono::high_resolution_clock::now();

    std::shared_ptr<raspscreen::screen::Screen> new_screen = std::make_shared<raspscreen::screen::Screen>();
    new_screen->name = screen["name"].asString();
    new_screen->text = screen["text"].asString();
    new_screen->priority = screen["priority"].asInt();
    new_screen->valid_until = now + std::chrono::seconds(screen["duration"].asInt());

    raspscreen::log::Logger::log("Adding new screen to list of screens");
    if(screens.find(new_screen->name) != screens.end())
    {
        screens[new_screen->name]->name = new_screen->name;
        screens[new_screen->name]->text = new_screen->text;
        screens[new_screen->name]->priority = new_screen->priority;
        screens[new_screen->name]->valid_until = new_screen->valid_until;
    }
    else
    {
        screens[new_screen->name] = new_screen;
    };
};




void App::update_screen()
{
    std::lock_guard<std::mutex> guard(thread_lock);

    // remember last displayed screen
    static std::shared_ptr<raspscreen::screen::Screen> last_screen;

    // get current time
    auto now = std::chrono::high_resolution_clock::now();

    // remove not valid screens
    for(auto screen : screens)
    {
        if(screen.second->valid_until < now)
        {
            if(screen.second == last_screen)
                last_screen = nullptr;

            auto it = screens.find(screen.first);
            screens.erase(it);
        };
    };

    // if list is empty clean screen
    if(screens.size() < 1)
    {
        lcd->clean();
        lcd->backlight(false);
        return;
    }
    else
    {
        lcd->backlight(true);
    };

    std::shared_ptr<raspscreen::screen::Screen> current_screen;
    current_screen = screens.begin()->second;

    // search for screen with highest priority
    for(auto screen : screens)
    {
        if(screen.second->priority > current_screen->priority)
            current_screen = screen.second;
    };

    if(last_screen && current_screen->priority == last_screen->priority)
        current_screen = last_screen;

    lcd->print(current_screen->text);
    last_screen = current_screen;
};




void App::run()
{
    running = true;

    raspscreen::log::Logger::log("Starting REST interface thread");
    auto rest_thread = new std::thread(rest_handler);

    raspscreen::log::Logger::log("Cleaning screen");
    lcd->clean();

    try
    {
        while(running)
        {
            update_screen();
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        };
    }
    catch (...)
    {
    };
};




}
}
