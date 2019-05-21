#pragma once

#include <cppurses/cppurses.hpp>
#include <memory>

#include "GrepHandler.hpp"
#include "Signals.hpp"
#include "ItemList.hpp"
#include "FilePreview.hpp"
#include "Settings.hpp"

using namespace cppurses;

struct ResultWindow : public cppurses::layout::Horizontal{
    ResultWindow(Settings& settings)
        : settings(settings)
    {
        Signal::changedPreview.connect([&](Result result){
            if(settings.get<bool>("OPEN_PREVIEW"))
                preview.setPreview(std::move(result));
        });

    }
    
    void setResults(std::vector<Result> results)
    {
        items.setResults(std::move(results));
    }

    ItemList& items{this->make_child<ItemList>()};
    FilePreview& preview{this->make_child<FilePreview>()};
    Settings& settings;
};
