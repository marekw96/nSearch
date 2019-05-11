#include "executeCommand.hpp"

#include <string_view>
#include <string>
#include <cstdlib>
#include <fstream>
#include <streambuf>

Mmap executeCommand(std::string_view command)
{
    auto OUTPUT_FILE = "command_output.txt";
    std::string full_cmd{command};
    full_cmd += " --exclude ";
    full_cmd += OUTPUT_FILE;
    full_cmd += " > ";
    full_cmd += OUTPUT_FILE;
    full_cmd += " 2>&1";

    system(full_cmd.c_str());

    return Mmap(OUTPUT_FILE);
}
