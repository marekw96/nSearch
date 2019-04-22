#pragma once

#include <cppurses/cppurses.hpp>
#include <string>

#include "Signals.hpp"

using namespace cppurses;

struct SearchInput : cppurses::Line_edit
{
    SearchInput() : Line_edit("Input search"){}

    bool key_press_event(const Key::State& key)
    {
        if(key.key == Key::Enter)
        {
            std::string command = this->contents().str();
            if(!command.empty())
            {
                searchSignal(command);
                this->set_contents("");
            }
        }
        if(key.key == Key::Arrow_down)
        {
            selectLowerSignal();
        }
        if(key.key == Key::Arrow_up)
        {
            selectUpperSignal();
        }

        return Line_edit::key_press_event(key);
    }
};
