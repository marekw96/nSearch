#pragma once

#include <cppurses/cppurses.hpp>
#include <string>
#include <algorithm>
#include <iostream>

#include "GrepHandler.hpp"
#include "FileReader.hpp"

using namespace cppurses;

struct FilePreview : public cppurses::layout::Vertical
{
    void setPreview(Result result)
    {
        filePath.set_contents(std::string("Loading:") + result.filePath.data());
        auto content = file.readLines(result.filePath);

        LOG(DEBUG) << "Loaded file " << result.filePath.data() << std::endl;
        LOG(DEBUG) << "Size of content " << content.size() << std::endl;
        
        const auto& children = display.children.get();
        std::for_each(rbegin(children), rend(children), [](const auto& child){child->close();});

        decltype(height()) halfHeight = height()/2;
        decltype(halfHeight) start = 0;

        if(result.lineNumber > halfHeight)
            start = result.lineNumber - halfHeight;

        auto stop =  std::min(content.size(), start + height());

        for(int i = start; i < stop; ++i)
        {
            auto& item = display.make_child<Label>(content[i]);
            if(i+1 == result.lineNumber)
                item.brush.set_foreground(Color::Light_blue);
        }
        filePath.set_contents(result.filePath);
    }

    FileReader file;
    Label& filePath{this->make_child<Label>("Loading")};
    layout::Vertical& display{this->make_child<layout::Vertical>()};
};

