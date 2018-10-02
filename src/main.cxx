#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <map>
#include <restsrv.hpp>
#include <rasplib.hpp>


std::mutex thread_lock;

rasplib::display::Alphanumeric lcd(16, 2);


class CScreen
{
public:
    unsigned int priority;

    std::chrono::system_clock::time_point valid_until;

public:
    std::string name;
    std::string text;

public:
    CScreen();
    ~CScreen();

};



CScreen::CScreen()
{
    priority = 0;
    valid_until = std::chrono::high_resolution_clock::now() + std::chrono::seconds(1);
};


CScreen::~CScreen()
{
};




std::map<std::string, std::shared_ptr<CScreen>> screens;
std::shared_ptr<CScreen> last_screen;



void check_new_messages()
{
    return;
};



void update_screen()
{
    std::lock_guard<std::mutex> guard(thread_lock);

    // remember last displayed screen
    static std::shared_ptr<CScreen> last_screen;

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
        lcd.print("    [EMPTY]    \n ");
        return;
    }

    std::shared_ptr<CScreen> current_screen;
    current_screen = screens.begin()->second;

    // search for screen with highest priority
    for(auto screen : screens)
    {
        if(screen.second->priority > current_screen->priority)
            current_screen = screen.second;
    };

    if(last_screen && current_screen->priority == last_screen->priority)
        current_screen = last_screen;

    lcd.print(current_screen->text);
    last_screen = current_screen;
};




void request_handler(rest::server::RestRequest &request, rest::server::RestResponse &response)
{
    std::lock_guard<std::mutex> guard(thread_lock);

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

    auto screen = request.body();

    auto now = std::chrono::high_resolution_clock::now();

    std::shared_ptr<CScreen> new_screen = std::make_shared<CScreen>();
    new_screen->name = screen["name"].asString();
    new_screen->text = screen["text"].asString();
    new_screen->priority = screen["priority"].asInt();
    new_screen->valid_until = now + std::chrono::seconds(screen["duration"].asInt());

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

    response.set_http_code(200);
}




void rest_handler()
{
    rest::server::RestSrv srv(request_handler);
    srv.set_port(8080);

    while(1)
    {
        try
        {
            srv.run();
        }
        catch(...)
        {
        };
    };
};



bool running = true;



int main(int argc, char* argv[])
{
    lcd.init(1, std::bitset<8>(0x27));
    lcd.set_mode(2, false, false);

    lcd.clean();

    auto rest_thread = new std::thread(rest_handler);

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

    return 0;
};




