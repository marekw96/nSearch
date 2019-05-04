#pragma once

#include <string_view>
#include <string>
#include <vector>

struct Result
{
    int lineNumber;
    std::string filePath;
};

class GrepHandler
{
public:
    std::vector<Result> grep(std::string_view pattern);
};
