#include "Signals.hpp"

sig::Signal<void(std::string)> Signal::search{};
sig::Signal<void()> Signal::selectLower{};
sig::Signal<void()> Signal::selectUpper{};
sig::Signal<void(Result)> Signal::changedPreview{};
