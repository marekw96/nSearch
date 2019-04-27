#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include "GrepHandler.hpp"
#include "Mmap.hpp"

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

    Mmap readFile(std::string_view path)
    {
        Mmap output;
        output.openFile(path);

        return output;
    }

private:
    Lines splitByLine(const Mmap& input)
    {
        std::stringstream stream(input.cbegin());
        std::vector<std::string> lines;
        std::string temp;

        while(std::getline(stream, temp))
            lines.emplace_back(std::move(temp));

        return lines;
    }
};
