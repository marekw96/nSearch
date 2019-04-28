#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include "GrepHandler.hpp"
#include "Mmap.hpp"

using Lines = std::vector<std::string_view>;

class FileReader
{
public:
    const Lines& readLines(std::string_view path)
    {
        if(lastPath == path)
            return lastLines;

        const auto& content = readFile(path);
        auto lines = splitByLine(content);
        lastLines = std::move(lines);
        
        return lastLines;
    }

    const Mmap& readFile(std::string_view path)
    {
        if(lastPath == path)
            return *lastFile;

        lastPath = path;
        lastFile = std::make_unique<Mmap>(path);

        return *lastFile;
    }

private:
    Lines splitByLine(const Mmap& input)
    {
        Lines lines;

        const char* start = lastFile->cbegin();
        const char* end = start;

        for(const auto& letter : *lastFile)
        {
            if(letter == '\n')
            {
                lines.emplace_back(start, std::distance(start,end));
                start = ++end;
                continue;
            }
            
            ++end;
        }

        return lines;
    }

    std::string lastPath;
    std::unique_ptr<Mmap> lastFile;
    Lines lastLines;
};
