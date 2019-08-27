#pragma once

#include <fstream>
#include <vector>

namespace Utility {

void LoadFile(const char* path, std::vector<uint8_t>& buffer);

}