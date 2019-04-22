#pragma once

#include <cppurses/cppurses.hpp>
#include <string>

#include "GrepHandler.hpp"

using namespace cppurses;

struct ResultLine : public layout::Horizontal
{
    ResultLine(const Result& result)
    {
        this->height_policy.fixed(1);
        line_number.set_contents(std::to_string(result.lineNumber));
        path.set_contents(result.filePath);

        line_number.width_policy.fixed(5);
        line_number.set_alignment(Alignment::Right);
    }

    void highlightOn()
    {
       line_number.brush.set_foreground(Color::Violet);
       line_number.append(""); //TODO:: ugly hack to hightlighting works :( 

       path.brush.set_foreground(Color::Violet);
       path.append(""); //TODO:: ugly hack to hightlighting works :( 
    }

    void highlightOff()
    {
       line_number.brush.set_foreground(Color::White);
       line_number.append(""); //TODO:: ugly hack to hightlighting works :( 

       path.brush.set_foreground(Color::White);
       path.append(""); //TODO:: ugly hack to hightlighting works :( 
    }

    Label& line_number{this->make_child<Label>("")};
    Label& path{this->make_child<Label>("")};
};
