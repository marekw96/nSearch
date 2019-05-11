#include "FilePreview.hpp"

#include <aixlog.hpp>
#include <string>
#include <algorithm>
#include <iostream>
#include "executeCommand.hpp"

void FilePreview::setPreview(Result result)
{
    fillDisplayIfNeeded();

    filePath.set_contents(std::string("Loading:") + result.filePath.data());
    const auto& content = file.readLines(result.filePath);

    LOG(DEBUG) << "Loaded file " << result.filePath.data() << std::endl;
    LOG(DEBUG) << "Size of content " << content.size() << std::endl;
    
    decltype(height()) halfHeight = height()/2;
    decltype(halfHeight) start = 0;

    if(result.lineNumber > halfHeight)
        start = result.lineNumber - halfHeight;

    auto stop = std::min(content.size(), start + height());

    for(auto& childPtr : display.children.get())
    {
        auto& child = static_cast<Label&>(*childPtr);
        
        if(start >= stop)
        {
            child.set_contents("");
            continue;
        }

        child.set_contents(content[start].data());

        if(start + 1 == result.lineNumber)
            child.brush.set_foreground(Color::Light_blue);
        else
            child.brush.set_foreground(Color::White);

        ++start;
    }

    filePath.set_contents(result.filePath);
}

void FilePreview::fillDisplayIfNeeded()
{
    if(!display.children.get().empty())
        return;

    for(auto i = 0u; i < display.height(); ++i)
        display.make_child<Label>("");
}
