#pragma once

#include <string_view>
#include <string>
#include <cstdlib>
#include <fstream>
#include <streambuf>

auto OUTPUT_FILE = "command_output.txt";

std::string executeCommand(std::string_view command)
{
    std::string full_cmd{command};
    full_cmd += " --exclude ";
    full_cmd += OUTPUT_FILE;
    full_cmd += " > ";
    full_cmd += OUTPUT_FILE;
    full_cmd += " 2>&1";

    system(full_cmd.c_str());

    std::ifstream file(OUTPUT_FILE);
    std::string output((std::istreambuf_iterator<char>(file)),
                       (std::istreambuf_iterator<char>()));

    return output;
}
