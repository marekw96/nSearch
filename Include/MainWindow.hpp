#pragma once

#include <cppurses/cppurses.hpp>
#include <string>

#include "ResultWindow.hpp"
#include "InputArea.hpp"
#include "GrepHandler.hpp"
#include "Settings.hpp"

using namespace cppurses;

class MainWindow : public layout::Vertical 
{
public:
    MainWindow(Settings& settings)
        : settings(settings), output(this->make_child<ResultWindow>(settings))
    {
        Signal::search.connect([&](std::string command){
                auto results = grep.grep(command);

                output.setResults(std::move(results));
                });
    }
    void init()
    {
        input.init();
        Focus::set_focus_to(&input.search);
    }

private:
    Settings& settings;
    ResultWindow& output;
    InputArea& input{this->make_child<InputArea>()};
    GrepHandler grep;
};
