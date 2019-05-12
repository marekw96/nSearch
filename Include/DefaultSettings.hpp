#pragma once
#include <string_view>
#include <any>
#include <initializer_list>

struct DefaultValue
{
    std::string_view name;
    std::any value;
};

std::initializer_list<DefaultValue> Values {
    DefaultValue{"OPEN_PREVIEW", true}
};
