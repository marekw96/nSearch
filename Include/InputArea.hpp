#pragma once

#include <cppurses/cppurses.hpp>
#include <string>

#include "SearchInput.hpp"

using namespace cppurses;

struct InputArea : public cppurses::layout::Horizontal{
    void init()
    {
        this->height_policy.fixed(1);
        search_btn.width_policy.fixed(10);
        this->border.enable();
        this->border.segments.enable_all();
    }

    Label& search_btn{this->make_child<Label>("Search")};
    SearchInput& search{this->make_child<SearchInput>()};
};
