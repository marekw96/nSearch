#pragma once

#include "GrepHandler.hpp"
#include <cppurses/cppurses.hpp>
#include <string>

using namespace cppurses;

sig::Signal<void(std::string)> searchSignal;
sig::Signal<void()> selectLowerSignal;
sig::Signal<void()> selectUpperSignal;
sig::Signal<void(Result)> changedPreview;
