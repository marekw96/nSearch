#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <sstream>
#include <regex>
#include "executeCommand.hpp"


struct Result
{
    int lineNumber;
    std::string filePath;
};

class GrepHandler
{
public:
    std::vector<Result> grep(std::string_view pattern)
    {
        auto command = prepareCommand(pattern);
        auto result = executeCommand(command);
        auto lines = splitByLine(result);

        return filterResult(lines);
    }

private:
    std::string prepareCommand(std::string_view pattern)
    {
        std::string begin = "grep -rn ";
        std::string end = " .";

        return begin + std::string(pattern) + end;
    }

    std::vector<std::string> splitByLine(std::string_view input)
    {
        std::stringstream stream(input.data());
        std::vector<std::string> lines;
        std::string temp;

        while(std::getline(stream, temp))
            lines.emplace_back(std::move(temp));

        return lines;
    }

    std::vector<Result> filterResult(const std::vector<std::string>& input)
    {
        std::vector<Result> output;
        std::regex regexp("\\w*:\\d*:");

       for(const auto& line : input)
       {
           if(std::regex_search(line, regexp))
           {
                output.emplace_back(getResultFromLine(line));
           }
       }

       return output;
    }

    Result getResultFromLine(std::string_view line)
    {
        auto mark = ':';
        std::stringstream stream;
        std::string path;
        int lineNumber = 0;
        bool parsingPath;

        for(auto letter : line)
        {
            if(letter == mark)
            {
                if(parsingPath)
                {
                    path = stream.str(); 
                    stream.str("");
                    parsingPath = false;
                    continue;
                }

                lineNumber = std::stoi(stream.str());
                break;
            }
            stream << letter;
        }

        return {lineNumber, path};
    }
};
