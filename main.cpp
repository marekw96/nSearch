#include "executeCommand.hpp"
#include "GrepHandler.hpp"
#include "FileReader.hpp"

#include <algorithm>
#include <iostream>

#include <cppurses/cppurses.hpp>
#include <string>

using namespace cppurses;

sig::Signal<void(std::string)> searchSignal;
sig::Signal<void()> selectLowerSignal;
sig::Signal<void()> selectUpperSignal;
sig::Signal<void(Result)> changedPreview;

struct ResultLine : public cppurses::layout::Horizontal
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
        if(selected == results.size() -1)
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


struct FilePreview : public cppurses::layout::Vertical
{
    void setPreview(Result result)
    {
        filePath.set_contents(std::string("Loading:") + result.filePath.data());
        auto content = file.readLines(result.filePath);
        
        const auto& children = display.children.get();
        std::for_each(rbegin(children), rend(children), [](const auto& child){child->close();});

        decltype(height()) halfHeight = height()/2;
        decltype(halfHeight) zero = 0;

        auto start = std::max(zero, result.lineNumber - halfHeight);
        auto stop =  std::min(content.size(),result.lineNumber + halfHeight);

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

struct ResultWindow : public cppurses::layout::Horizontal{
    void init()
    {
        changedPreview.connect([&](Result result){
            preview.setPreview(std::move(result));
                });
    }
    
    void setResults(std::vector<Result> results)
    {
        items.setResults(std::move(results));
    }

    ItemList& items{this->make_child<ItemList>()};
    FilePreview& preview{this->make_child<FilePreview>()};
};

struct SearchInput : cppurses::Line_edit{
    SearchInput() : Line_edit("Input search"){}

    bool key_press_event(const Key::State& key)
    {
        if(key.key == Key::Enter)
        {
            std::string command = this->contents().str();
            if(!command.empty())
            {
                searchSignal(command);
                this->set_contents("");
            }
        }
        if(key.key == Key::Arrow_down)
        {
            selectLowerSignal();
        }
        if(key.key == Key::Arrow_up)
        {
            selectUpperSignal();
        }

        return Line_edit::key_press_event(key);
    }
};

struct InputArea : public cppurses::layout::Horizontal{
    void init()
    {
        this->height_policy.fixed(1);
        search_btn.width_policy.fixed(10);
        this->border.enable();
        this->border.segments.enable_all();
    }

    Label& search_btn{this->make_child<Label>("Search")};
    SearchInput& search{this->make_child<SearchInput>()};
};

class MainWindow : public cppurses::layout::Vertical {
public:
    void init(){
        output.init();
        input.init();
        Focus::set_focus_to(&input.search);
        searchSignal.connect([&](std::string command){
                auto results = grep.grep(command);

                output.setResults(std::move(results));
                });
    }

private:
    ResultWindow& output{this->make_child<ResultWindow>()};
    InputArea& input{this->make_child<InputArea>()};
    GrepHandler grep;
};

int main()
{
    System sys;
    MainWindow mainWindow{};
    mainWindow.init();

    return sys.run(mainWindow);
}
