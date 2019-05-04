#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <memory>
#include "Mmap.hpp"

using Lines = std::vector<std::string_view>;

class FileReader
{
public:
    const Lines& readLines(std::string_view path);
    const Mmap& readFile(std::string_view path);

private:
    std::string lastPath;
    std::unique_ptr<Mmap> lastFile;
    Lines lastLines;
};
