#include <boost/predef/os.h>

#include <filesystem>
#include <string>

#if (BOOST_OS_WINDOWS)
#include <stdlib.h>
#elif (BOOST_OS_SOLARIS)
#include <limits.h>
#include <stdlib.h>
#elif (BOOST_OS_LINUX)
#include <limits.h>
#include <unistd.h>
#elif (BOOST_OS_MACOS)
#include <mach-o/dyld.h>
#elif (BOOST_OS_BSD_FREE)
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

// NOLINTBEGIN
namespace utils {

/*
 * Returns the full path to the currently running executable,
 * or an empty string in case of failure.
 */
auto get_executable_path() -> std::string {
#if (BOOST_OS_WINDOWS)
  char *exe_path;
  if (_get_pgmptr(&exe_path) != 0) exe_path = "";
#elif (BOOST_OS_SOLARIS)
  char exe_path[PATH_MAX];
  if (realpath(getexecname(), exe_path) == NULL) exe_path[0] = '\0';
#elif (BOOST_OS_LINUX)
  char exe_path[PATH_MAX];
  ssize_t len = ::readlink("/proc/self/exe", exe_path, sizeof(exe_path));
  if (len == -1 || len == sizeof(exe_path)) len = 0;
  exe_path[len] = '\0';
#elif (BOOST_OS_MACOS)
  char exe_path[PATH_MAX];
  uint32_t len = sizeof(exe_path);
  if (_NSGetExecutablePath(exe_path, &len) != 0) {
    exe_path[0] = '\0';  // buffer too small (!)
  } else {
    // resolve symlinks, ., .. if possible
    char *canonical_path = realpath(exe_path, nullptr);
    if (canonical_path != nullptr) {
      strncpy(exe_path, canonical_path, len);
      free(canonical_path);
    }
  }

#elif (BOOST_OS_BSD_FREE)
  char exe_path[2048];
  int mib[4];
  mib[0] = CTL_KERN;
  mib[1] = KERN_PROC;
  mib[2] = KERN_PROC_PATHNAME;
  mib[3] = -1;
  size_t len = sizeof(exe_path);
  if (sysctl(mib, 4, exe_path, &len, NULL, 0) != 0) exe_path[0] = '\0';
#else  // emscripten
  const char *exe_path = "";
#endif
  return std::string(exe_path);
}

auto get_executable_dir() -> std::filesystem::path {
  std::string dir = get_executable_path();
  const std::filesystem::path path = dir;  // Is this assignment safe?
  return path.parent_path();
}

}  // namespace utils
// NOLINTEND
