#include "executeCommand.hpp"

#include <cppurses/cppurses.hpp>
#include <string>

using namespace cppurses;

sig::Signal<void(std::string)> searchSignal;

struct ResultWindow : public cppurses::layout::Horizontal{
    void init()
    {
    }
    Text_display& items{this->make_child<Text_display>("Items")};
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
                auto result = executeCommand(command);
                output.preview.set_contents(result);
                
                });
    }

private:
    ResultWindow& output{this->make_child<ResultWindow>()};
    InputArea& input{this->make_child<InputArea>()};

};

int main()
{
    System sys;
    MainWindow mainWindow{};
    mainWindow.init();

    return sys.run(mainWindow);
}
