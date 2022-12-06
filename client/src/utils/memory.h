#ifndef APP_UTILS_MEMORY_H
#define APP_UTILS_MEMORY_H

#include <string>

namespace utils {

#ifdef _DEBUG
void set_debug(bool debug);
#endif

void display_sizeof_values();

/**
 * @brief Get the size of the available memory
 *
 * @return size_t
 */
auto getTotalSystemMemory() -> size_t;

/**
 * Prints a nice number of bytes (KB, MB, GB, etc)
 * Adapted from https://www.mbeckler.org/blog/?p=114
 * @param bytes
 * @return
 */
auto prettyBytes(size_t bytes) -> std::string;

}  // namespace utils
#endif  // APP_UTILS_MEMORY_H