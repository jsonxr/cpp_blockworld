#include "Application.h"

#include "core.h"

namespace app {

void Application::init() {
  cout << "headless=" << options_.headless << "\nthreads=" << options_.threads
       << endl;
}

void Application::run() {}

};  // namespace app