#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include "GrepHandler.hpp"

using Lines = std::vector<std::string>;

class FileReader
{
public:
    Lines readLines(std::string_view path)
    {
        auto content = readFile(path);
        auto lines = splitByLine(content);
        
        return std::move(lines);
    }

    std::string readFile(std::string_view path)
    {
        std::ifstream file(path.data());
        std::string output((std::istreambuf_iterator<char>(file)),
                           (std::istreambuf_iterator<char>()));

        return output;
    }

private:
    Lines splitByLine(std::string_view input)
    {
        std::stringstream stream(input.data());
        std::vector<std::string> lines;
        std::string temp;

        while(std::getline(stream, temp))
            lines.emplace_back(std::move(temp));

        return lines;
    }
};
