#include "Settings.hpp"

#include "Mmap.hpp"
#include <sstream>
#include <algorithm>
#include "DefaultSettings.hpp"
#include <aixlog.hpp>
#include <stdlib.h>

std::vector<std::string> splitByLine(const Mmap& input)
{
    std::stringstream stream(input.begin());
    std::vector<std::string> lines;
    std::string temp;

    while(std::getline(stream, temp))
        lines.emplace_back(std::move(temp));

    return lines;
}

Settings::Settings()
{
    auto* homeDir = getenv("HOME");
    auto configPath = std::string(homeDir).append("/.local/.nSearch");
    try
    {

        Mmap file(configPath); 
    
        auto lines = splitByLine(file);
        parseLines(lines);
    }
    catch (const FileOpenException& e)
    {
        LOG(DEBUG) << "Could no open " << configPath << "\n";
    }
    catch (const BadParseParameter& e)
    {
        LOG(DEBUG) << "Could not parse parameter: " << e.what() << "\n";
    }

    fillAllUnsetParams();
}
void Settings::fillAllUnsetParams()
{
   for(const auto& item : Values)
    {
        if(values.find(item.name.data()) == values.end())
        {
            this->values.emplace(item.name.data(), item.value);
        }
    }
}

const DefaultValue& getDefault(std::string_view key)
{
    for(const auto& item : Values)
    {
        if(item.name == key)
            return item;
    }

    throw BadParseParameter(key);
}


void Settings::parseLines(const std::vector<std::string>& lines)
{
    using namespace std::literals;

    std::string name, value;
    
    for(const auto& line : lines)
    {
        std::stringstream stream(line.data());

        std::getline(stream, name, ' ');
        std::getline(stream, value);

        LOG(DEBUG) << "Loaded " << name << " " << value << "\n";

        const auto& defaultValue = getDefault(name); 
        if("b"sv == defaultValue.value.type().name())
        {
            std::transform(begin(value), end(value), begin(value), ::tolower);
            if(value == "true")
            {
                values.emplace(defaultValue.name, true);
            }
            else if(value == "false")
            {
                values.emplace(defaultValue.name, false);
            }
            continue;
        }

        throw BadParseParameter(name, value);
    }
}

