#pragma once

#include <cppurses/cppurses.hpp>
#include <string>

#include "GrepHandler.hpp"

using namespace cppurses;

struct ResultLine : public layout::Horizontal
{
    ResultLine(const Result& result);

    void highlightOn();
    void highlightOff();

    Label& line_number{this->make_child<Label>("")};
    Label& path{this->make_child<Label>("")};
};
