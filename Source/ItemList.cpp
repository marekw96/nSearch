#include "ItemList.hpp"

#include <cppurses/cppurses.hpp>
#include <string>
#include <algorithm>
#include <memory>

#include "GrepHandler.hpp"
#include "Signals.hpp"
#include "ResultLine.hpp"

ItemList::ItemList()
{
    Signal::selectLower.connect([&](){
            selectLower();
        });

    Signal::selectUpper.connect([&](){
            selectUpper();
        });
}

void ItemList::setResults(std::vector<Result> results)
{
    this->results = std::move(results);
    if(this->results.empty())
	    return;

    selected = 0;
    minIndex = 0;
    maxIndex = height();

    redrawFromTop();
    Signal::changedPreview(this->results[selected]);
}

void ItemList::selectLower()
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

void ItemList::selectUpper()
{
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

void ItemList::removeChildren()
{
    const auto& children = this->children.get();
    std::for_each(rbegin(children), rend(children), [](const auto& child){child->close();});
}

void ItemList::removeChildFromTop()
{
    const auto& children = this->children.get();
    children.front()->close();
}

void ItemList::removeChildFromBottom()
{
    const auto& children = this->children.get();
    children.back()->close();
}

void ItemList::redrawFromTop()
{
    removeChildren();
    auto height = std::min(this->height(), results.size());
    for(auto i = 0u; i < height; i++)
        this->make_child<ResultLine>(results[i]);

    changeSelection(selected, selected);
}

void ItemList::changeSelection(int from, int to)
{
    const auto& children = this->children.get();
    if(children.size() == 0)
        return;

    auto* child =  reinterpret_cast<ResultLine*>(children[from].get());
    child->highlightOff();
    
    auto* child2 = reinterpret_cast<ResultLine*>(children[to].get());
    child2->highlightOn();

    Signal::changedPreview(results[selected]);
}
