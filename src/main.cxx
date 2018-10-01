#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <restsrv.hpp>



std::mutex thread_lock;




class CScreen
{
public:
    unsigned int priority;

    std::chrono::system_clock::time_point valid_until;

public:
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




std::list<std::shared_ptr<CScreen>> screens;




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

    // remove not valid screens screens
    for(auto screen : screens)
    {
        if(screen->valid_until < now)
        {
            screens.remove(screen);
            if(screen == last_screen)
                last_screen = nullptr;
        };
    };

    // if list is empty clean screen
    if(screens.size() < 1)
    {
        std::cout << now.time_since_epoch().count() << ":\n" << "[EMPTY]" << std::endl;
        return;
    }

    std::shared_ptr<CScreen> current_screen;
    current_screen = *screens.begin();

    // search for screen with highest priority
    for(auto screen : screens)
    {
        if(screen->priority > current_screen->priority)
            current_screen = screen;
    };

    if(last_screen && current_screen->priority == last_screen->priority)
        current_screen = last_screen;

    std::cout << now.time_since_epoch().count() << ":\n" << current_screen->text << std::endl;
    last_screen = current_screen;
};




void request_handler(rest::server::RestRequest &request, rest::server::RestResponse &response)
{
    std::lock_guard<std::mutex> guard(thread_lock);
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
    auto rest_thread = new std::thread(rest_handler);

    try
    {
        while(running)
        {
            update_screen();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        };
    }
    catch (...)
    {
    };

    return 0;
};




