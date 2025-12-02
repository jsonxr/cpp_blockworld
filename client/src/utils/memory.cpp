#include "memory.h"

#include <array>

#ifdef __EMSCRIPTEN__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#include <boost/interprocess/mapped_region.hpp>
#pragma clang diagnostic pop
#else
#include <boost/interprocess/mapped_region.hpp>
#endif

#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>

// NOLINTBEGIN

#ifdef _DEBUG
thread_local bool debug_ = false;
auto operator new(size_t size) -> void * {
  auto *address = malloc(size);
  if (debug_) {
    std::cout << address << " - malloc " << size << "bytes" << std::endl;
  }
  return address;
}

void operator delete(void *address) noexcept {
  if (debug_) {
    std::cout << address << " - free" << std::endl;
  }
  free(address);
}
#endif

namespace utils {

#ifdef _DEBUG
void set_debug(bool debug) { debug_ = debug; }
#endif

using std::cout;
using std::endl;

//------------------------------------------------------------------------------
// display_sizeof_values
//------------------------------------------------------------------------------
void display_sizeof_values() {
  cout << "Size of char : " << sizeof(char) << " byte" << endl;
  cout << "Size of int : " << sizeof(int) << " bytes" << endl;
  cout << "Size of short int : " << sizeof(short int) << " bytes" << endl;
  cout << "Size of long int : " << sizeof(long int) << " bytes" << endl;
  cout << "Size of signed long int : " << sizeof(signed long int) << " bytes"
       << endl;
  cout << "Size of unsigned long int : " << sizeof(unsigned long int)
       << " bytes" << endl;
  cout << "Size of float : " << sizeof(float) << " bytes" << endl;
  cout << "Size of double : " << sizeof(double) << " bytes" << endl;
  cout << "Size of wchar_t : " << sizeof(wchar_t) << " bytes" << endl;
  cout << "Size of size_t : " << sizeof(size_t) << " bytes" << endl;
}

//------------------------------------------------------------------------------
// getTotalSystemMemory
//------------------------------------------------------------------------------
auto getTotalSystemMemory() -> size_t {
  //  auto pages = sysconf(_SC_PHYS_PAGES);
  //  std::cout << "pages=" << pages << std::endl;

  size_t size = boost::interprocess::mapped_region::get_page_size();

  auto page_size = sysconf(_SC_PAGE_SIZE);
  return page_size;
  //  std::cout << "page_size=" << page_size << std::endl;
  //  return pages * page_size;
}

//------------------------------------------------------------------------------
// prettyBytes
//------------------------------------------------------------------------------
auto prettyBytes(size_t bytes) -> std::string {
  std::array<std::string, 7> suffixes{
      "B", "KB", "MB", "GB", "TB", "PB", "EB",
  };

  uint32_t s = 0;  // which suffix to use
  double count = bytes;
  while (count >= 1024 && s < 7) {
    s++;
    count /= 1024;
  }

  int whole = int(count);
  if (count - floor(count) == 0.0) {
    return std::to_string(whole) + suffixes.at(s);
  } else {
    return std::to_string(count / 100) + suffixes.at(s);
  }
}

}  // namespace utils

// NOLINTEND
