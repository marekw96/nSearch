#include <cppurses/cppurses.hpp>
#include "aixlog.hpp"

#include "MainWindow.hpp"
#include "Settings.hpp"

using namespace cppurses;

int main()
{

    System sys;
    AixLog::Log::init<AixLog::SinkFile>(AixLog::Severity::trace, AixLog::Type::all, "log.txt");
    
    Settings settings{};

    MainWindow mainWindow{settings};
    mainWindow.init();

    return sys.run(mainWindow);
}
