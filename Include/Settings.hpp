#pragma once

#include <unordered_map>
#include <string>
#include <any>
#include <string_view>
#include <stdexcept>
#include <vector>

struct BadParameter : std::runtime_error
{
    BadParameter(std::string_view param) 
        : std::runtime_error{std::string(param.data()) + " param does not exit or has other type"}
    {}
};

struct BadParseParameter : std::runtime_error
{
    BadParseParameter(
                   std::string_view name = "undefined name ",
                   std::string_view value = " undefined value")
        : std::runtime_error {std::string("param with ").append(name).append(value) }
    {}
};

class Settings
{
public:
    Settings();
    template <typename T>
    T get(std::string_view param)
    {
        try
        {
            return std::any_cast<T>(values[param.data()]);
        }
        catch (const std::bad_any_cast &e)
        {
            throw BadParameter(param);
        }
    }

private:
    void parseLines(const std::vector<std::string>& lines);
    void fillAllUnsetParams();

    std::unordered_map<std::string, std::any> values;
};
