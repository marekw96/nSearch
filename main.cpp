#include <algorithm>
#include <cppurses/cppurses.hpp>
#include <string>

#include "MainWindow.hpp"

using namespace cppurses;

int main()
{
    System sys;
    MainWindow mainWindow{};
    mainWindow.init();

    return sys.run(mainWindow);
}
