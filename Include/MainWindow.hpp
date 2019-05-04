#pragma once

#include <cppurses/cppurses.hpp>
#include <string>

#include "ResultWindow.hpp"
#include "InputArea.hpp"
#include "GrepHandler.hpp"

using namespace cppurses;

class MainWindow : public layout::Vertical 
{
public:
    void init()
    {
        output.init();
        input.init();
        Focus::set_focus_to(&input.search);
        Signal::search.connect([&](std::string command){
                auto results = grep.grep(command);

                output.setResults(std::move(results));
                });
    }

private:
    ResultWindow& output{this->make_child<ResultWindow>()};
    InputArea& input{this->make_child<InputArea>()};
    GrepHandler grep;
};
