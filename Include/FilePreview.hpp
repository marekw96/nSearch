#pragma once

#include <cppurses/cppurses.hpp>

#include "GrepHandler.hpp"
#include "FileReader.hpp"

using namespace cppurses;

struct FilePreview : public cppurses::layout::Vertical
{
    void setPreview(Result result);

private:
    void fillDisplayIfNeeded();
    
    FileReader file;
    Label& filePath{this->make_child<Label>("Loading")};
    layout::Vertical& display{this->make_child<layout::Vertical>()};
};

