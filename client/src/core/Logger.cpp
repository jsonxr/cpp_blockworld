#include "Logger.h"

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <string>
#include <utility>

namespace app {

Logger::Logger() = default;

Logger::Logger(std::string name) : name_(std::move(name)) {}

// TODO: @jsonxr Learn templates
// TODO: @jsonxr Figure out how to templatize

[[maybe_unused]] void Logger::trace(std::string format, ...) {  // NOLINT
  if (level_ > LogLevel::kTrace) {
    return;
  }
  va_list vargs;
  va_start(vargs, format);
  std::cout << "[" << name_ << "] - ";
  vprintf(format.c_str(), vargs);
  va_end(vargs);
}

[[maybe_unused]] void Logger::debug(std::string format, ...) {  // NOLINT
  if (level_ > LogLevel::kDebug) {
    return;
  }
  va_list vargs;
  va_start(vargs, format);
  std::cout << "[" << name_ << "] - ";
  vprintf(format.c_str(), vargs);
  va_end(vargs);
}

[[maybe_unused]] void Logger::info(std::string format, ...) {  // NOLINT
  if (level_ > LogLevel::kInfo) {
    return;
  }
  va_list vargs;
  va_start(vargs, format);
  std::cout << "[" << name_ << "] - ";
  vprintf(format.c_str(), vargs);
  va_end(vargs);
}

[[maybe_unused]] void Logger::warn(std::string format, ...) {  // NOLINT
  if (level_ > LogLevel::kWarn) {
    return;
  }
  va_list vargs;
  va_start(vargs, format);
  std::cout << "[" << name_ << "] - ";
  vprintf(format.c_str(), vargs);
  va_end(vargs);
}

[[maybe_unused]] void Logger::error(std::string format, ...) {  // NOLINT
  va_list vargs;
  va_start(vargs, format);
  std::cout << "[" << name_ << "] - ";
  vprintf(format.c_str(), vargs);
  va_end(vargs);
}

}  // namespace app
