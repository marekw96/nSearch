#include "executeCommand.hpp"
#include "GrepHandler.hpp"

#include <algorithm>

#include <cppurses/cppurses.hpp>
#include <string>

using namespace cppurses;

sig::Signal<void(std::string)> searchSignal;

struct ResultLine : public cppurses::layout::Horizontal
{
    ResultLine(const Result& result)
    {
        this->height_policy.fixed(1);
        line_number.set_contents(std::to_string(result.lineNumber));
        path.set_contents(result.filePath);

        line_number.width_policy.fixed(5);
        highlightOff();
        
        this->update();
    }

    void highlightOn()
    {
       line_number.brush.set_foreground(Color::Red);
    }

    void highlightOff()
    {
       line_number.brush.set_foreground(Color::White);
    }

    Label& line_number{this->make_child<Label>("")};
    Label& path{this->make_child<Label>("")};
};

struct ItemList : public cppurses::layout::Vertical
{
    void setResults(std::vector<Result> results)
    {
        removeChildren();
        this->results = std::move(results);
        selected = 0;

        redraw();
    }

    void removeChildren()
    {
        this->children = Children_data{this};
    }

    void redraw()
    {
        auto height = std::min(this->height(), results.size());
        for(int i = 0; i < height; i++)
            this->make_child<ResultLine>(results[i]);

        hightlight();
    }

    void hightlight()
    {
        const auto& children = this->children.get();
        if(children.size() == 0)
            return;

        auto* child = reinterpret_cast<ResultLine*>(children[selected].get());
        child->highlightOn();
    }

private:
    int selected = 0;
    std::vector<Result> results;
};

struct ResultWindow : public cppurses::layout::Horizontal{
    void init()
    {}
    
    void setResults(std::vector<Result> results)
    {
        items.setResults(std::move(results));
    }

    ItemList& items{this->make_child<ItemList>()};
    Text_display& preview{this->make_child<Text_display>("Preview")};
};

struct SearchInput : cppurses::Line_edit{
    SearchInput() : Line_edit("Input search"){}

    bool key_press_event(const Key::State& key)
    {
        if(key.key == Key::Enter)
        {
            std::string command = this->contents().str();
            searchSignal(command);
            this->set_contents("");
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
