#include "Macros.h"

std::map<void*, std::size_t> allocations;

void detectMemoryLeaks() {
    if (!allocations.empty()) {
        std::cout << "[MEM] Memory leaks detected:" << std::endl;
        for (const auto& alloc : allocations) {
            std::cout << "[MEM] Leak of " << alloc.second << " bytes at " << alloc.first << std::endl;
        }
    } else {
        std::cout << "[MEM] No memory leaks detected." << std::endl;
    }
}
