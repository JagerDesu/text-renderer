#include "Utility.hpp"

namespace Utility {
void LoadFile(const char* path, std::vector<uint8_t>& buffer) {
	std::ifstream file(path, std::ios::binary);

	if (!file) {
		return;
	}

	size_t size;
	file.seekg(0, std::ios::end);
	size = file.tellg();
	file.seekg(0, std::ios::beg);
	buffer.resize(size);
	file.read((char*)buffer.data(), buffer.size());
}
}