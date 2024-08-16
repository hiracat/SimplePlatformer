#include "debugprint.h"

#include <fstream>
#include <iostream>
#include <vector>

std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        debugerror("failed to open file: " << filename);
    }

    size_t fileSize = (size_t)file.tellg();

    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
