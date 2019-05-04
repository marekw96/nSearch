#pragma once

#include "GrepHandler.hpp"
#include <cppurses/cppurses.hpp>
#include <string>

using namespace cppurses;

struct Signal
{
    static sig::Signal<void(std::string)> search;
    static sig::Signal<void()> selectLower;
    static sig::Signal<void()> selectUpper;
    static sig::Signal<void(Result)> changedPreview;
};
