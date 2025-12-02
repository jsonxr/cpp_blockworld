#include "Application.h"

#include "core.h"

namespace app {

void Application::init() const {
  cout << "headless=" << options_.headless << "\nthreads=" << options_.threads
       << '\n';
}

void Application::run() {}

};  // namespace app
