#include "SearchInput.hpp"

bool SearchInput::key_press_event(const Key::State& key)
{
    if(key.key == Key::Enter)
    {
        std::string command = this->contents().str();
        if(!command.empty())
        {
            Signal::search(command);
            this->set_contents("");
        }
    }
    if(key.key == Key::Arrow_down)
    {
        Signal::selectLower();
    }
    if(key.key == Key::Arrow_up)
    {
        Signal::selectUpper();
    }

    return Line_edit::key_press_event(key);
}
