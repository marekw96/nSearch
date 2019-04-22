#pragma once

#include <cppurses/cppurses.hpp>
#include <string>
#include <algorithm>

#include "GrepHandler.hpp"
#include "Signals.hpp"
#include "ResultLine.hpp"

using namespace cppurses;

struct ItemList : public cppurses::layout::Vertical
{
    ItemList()
    {
        selectLowerSignal.connect([&](){
                selectLower();
            });

        selectUpperSignal.connect([&](){
                selectUpper();
            });
    }

    void setResults(std::vector<Result> results)
    {
        this->results = std::move(results);
        selected = 0;
        minIndex = 0;
        maxIndex = height();

        redrawFromTop();
    }

    void selectLower()
    {
        if(selected >= results.size() -1 || results.empty())
            return;
        
        ++selected;

        if(selected >= maxIndex)
        {
            removeChildFromTop();
            this->make_child<ResultLine>(results[selected]);
            ++maxIndex;
            ++minIndex;
        }

        changeSelection(selected - minIndex -1, selected - minIndex);
    }

    void selectUpper()
    {
        int old = selected;
        
        if(selected == 0)
            return;

        --selected;

        if(selected < minIndex)
        {
            removeChildFromBottom();
            children.insert(std::make_unique<ResultLine>(results[selected]), 0);
            --maxIndex;
            --minIndex;
        }

        changeSelection(selected - minIndex + 1, selected - minIndex);
    }

private:
    void removeChildren()
    {
        const auto& children = this->children.get();
        std::for_each(rbegin(children), rend(children), [](const auto& child){child->close();});
    }

    void removeChildFromTop()
    {
        const auto& children = this->children.get();
        children.front()->close();
    }

    void removeChildFromBottom()
    {
        const auto& children = this->children.get();
        children.back()->close();
    }

    void redrawFromTop()
    {
        removeChildren();
        auto height = std::min(this->height(), results.size());
        for(int i = 0; i < height; i++)
            this->make_child<ResultLine>(results[i]);

        changeSelection(selected, selected);
    }

    void changeSelection(int from, int to)
    {
        const auto& children = this->children.get();
        if(children.size() == 0)
            return;

        auto* child =  reinterpret_cast<ResultLine*>(children[from].get());
        child->highlightOff();
        
        auto* child2 = reinterpret_cast<ResultLine*>(children[to].get());
        child2->highlightOn();

        changedPreview(results[to]);
    }

    int selected = 0;
    int minIndex = 0;
    int maxIndex = 0;
    std::vector<Result> results;
};
