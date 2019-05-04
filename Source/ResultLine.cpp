#include "ResultLine.hpp"

ResultLine::ResultLine(const Result& result)
{
    this->height_policy.fixed(1);
    line_number.set_contents(std::to_string(result.lineNumber));
    path.set_contents(result.filePath);

    line_number.width_policy.fixed(5);
    line_number.set_alignment(Alignment::Right);
}

void ResultLine::highlightOn()
{
   line_number.brush.set_foreground(Color::Violet);
   line_number.append(""); //TODO:: ugly hack to hightlighting works :( 

   path.brush.set_foreground(Color::Violet);
   path.append(""); //TODO:: ugly hack to hightlighting works :( 
}

void ResultLine::highlightOff()
{
   line_number.brush.set_foreground(Color::White);
   line_number.append(""); //TODO:: ugly hack to hightlighting works :( 

   path.brush.set_foreground(Color::White);
   path.append(""); //TODO:: ugly hack to hightlighting works :( 
}
