#ifndef APP_APPLICATION_H
#define APP_APPLICATION_H

namespace app {

struct ApplicationOptions {
 public:
  bool headless{false};
  int threads{1};
};

class Application {
 public:
  Application(ApplicationOptions options = ApplicationOptions{})
      : options_(options){};
  void init();
  void run();

 private:
  ApplicationOptions options_{};
};

}  // namespace app

#endif  // APP_APPLICATION_H
