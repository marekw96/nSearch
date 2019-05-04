#pragma once

#include <cppurses/cppurses.hpp>
#include <string>

#include "Signals.hpp"

using namespace cppurses;

struct SearchInput : cppurses::Line_edit
{
    SearchInput() : Line_edit("Input search"){}

    bool key_press_event(const Key::State& key) override;
};
