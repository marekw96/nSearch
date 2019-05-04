#include "FileReader.hpp"

Lines splitByLine(const Mmap& input)
{
    Lines lines;

    const char* start = input.cbegin();
    const char* end = start;

    for(auto letter : input)
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

const Lines& FileReader::readLines(std::string_view path)
{
    if(lastPath == path)
        return lastLines;

    const auto& content = readFile(path);
    auto lines = splitByLine(content);
    lastLines = std::move(lines);
    
    return lastLines;
}

const Mmap& FileReader::readFile(std::string_view path)
{
    if(lastPath == path)
        return *lastFile;

    lastPath = path;
    lastFile = std::make_unique<Mmap>(path);

    return *lastFile;
}
