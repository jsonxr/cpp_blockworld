#ifndef APP_CORE_LOGGER_H
#define APP_CORE_LOGGER_H

#include <cstdarg>
#include <cstdint>
#include <iostream>
#include <string>

namespace app {

// void log(std::string format, ...);

class Logger {
 public:
  enum class LogLevel : std::uint8_t {
    kError [[maybe_unused]] = 0,
    kWarn = 1,
    kInfo = 2,
    kDebug = 3,
    kTrace = 4,
  };

  Logger();
  Logger(std::string name);

  [[maybe_unused]] void trace(std::string format, ...);  // NOLINT
  [[maybe_unused]] [[maybe_unused]] void debug(std::string format,
                                               ...);     // NOLINT
  [[maybe_unused]] void info(std::string format, ...);   // NOLINT
  [[maybe_unused]] void warn(std::string format, ...);   // NOLINT
  [[maybe_unused]] void error(std::string format, ...);  // NOLINT

 private:
  std::string name_;
  LogLevel level_{LogLevel::kTrace};
};

// template <typename... TArg>
// void Logger(std::string name, TArg&&... args)
// {
//     if (Message == MSG::Click)
//     {
//         TMenu* pMenu = new TMenu(forward<TArg>(args)...);
//         pMenu->Show();
//     }
// }

// auto create_logger(const std::string &name)
//     -> std::function<void(const char *format, ...)>;

// auto create_logger =
//     [](const std::string &name) -> std::function<void(std::string format)> {
//   return [&, name](const std::string &format) {
//     std::cout << "format=" << format << std::endl;
//     // va_list vargs;
//     // va_start(vargs, format);
//     // vprintf(format.c_str(), vargs);
//     // va_end(vargs);
//   };
// };

}  // namespace app

#endif  // APP_CORE_LOGGER_H
