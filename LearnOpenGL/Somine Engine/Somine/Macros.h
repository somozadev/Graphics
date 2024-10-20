#ifndef MACROS_H
#define MACROS_H

#include <iostream>
#include <map>
#include <cstdlib>
#include <new>  

extern std::map<void*, std::size_t> allocations;

#define NEW(cls, ...) \
([](auto&&... args) -> cls* { \
void* p = std::malloc(sizeof(cls)); \
if (p) allocations[p] = sizeof(cls); \
std::cout << "[MEM] Allocated " << sizeof(cls) << " bytes at " << p << std::endl; \
return new (p) cls(std::forward<decltype(args)>(args)...); \
}(__VA_ARGS__))

#define DELETE(ptr, cls) \
([](cls* p) { \
if (allocations.find(p) != allocations.end()) { \
std::cout << "[MEM] Deallocated " << allocations[p] << " bytes from " << p << std::endl; \
allocations.erase(p); \
} \
if (p) { \
p->~cls();  \
std::free(p); \
} \
}(ptr))

void detectMemoryLeaks();

#define ENABLE_MEMORY_LEAK_DETECTION() atexit(detectMemoryLeaks)
#endif  
