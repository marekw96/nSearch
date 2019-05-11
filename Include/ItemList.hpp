#pragma once

#include <cppurses/cppurses.hpp>

#include "GrepHandler.hpp"
#include <vector>

using namespace cppurses;

struct ItemList : public cppurses::layout::Vertical
{
    ItemList();

    void setResults(std::vector<Result> results);
    void selectLower();
    void selectUpper();

private:
    void removeChildren();
    void removeChildFromTop();
    void removeChildFromBottom();
    void redrawFromTop();
    void changeSelection(int from, int to);

    unsigned selected = 0;
    unsigned minIndex = 0;
    unsigned maxIndex = 0;
    std::vector<Result> results;
};
