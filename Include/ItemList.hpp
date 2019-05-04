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

    int selected = 0;
    int minIndex = 0;
    int maxIndex = 0;
    std::vector<Result> results;
};
