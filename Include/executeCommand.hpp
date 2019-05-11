#pragma once

#include <string_view>
#include "Mmap.hpp"

Mmap executeCommand(std::string_view command);
