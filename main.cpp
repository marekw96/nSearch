#include <algorithm>
#include <cppurses/cppurses.hpp>
#include <string>
#include "aixlog.hpp"

#include "MainWindow.hpp"

using namespace cppurses;

int main()
{
    System sys;
    AixLog::Log::init<AixLog::SinkFile>(AixLog::Severity::trace, AixLog::Type::all, "log.txt");

    MainWindow mainWindow{};
    mainWindow.init();

    return sys.run(mainWindow);
}
