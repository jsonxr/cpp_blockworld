#include "main_options.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/threading.h>

#include "Application.h"
#include "core.h"

namespace app {

auto parseApplicationOptions(int argc, char **argv)
    -> expected<ApplicationOptions, string> {
  (void)argc;
  (void)argv;
  ApplicationOptions options{};
  options.headless = false;
  options.threads = emscripten_num_logical_cores();
  return options;
}

}  // namespace app

#else

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <thread>

#include "Application.h"
#include "core.h"
#include <nonstd/expected.hpp>

namespace po = boost::program_options;

namespace app {

auto parseApplicationOptions(int argc, char **argv)
    -> expected<ApplicationOptions, string> {
  ApplicationOptions options{};

  po::options_description desc("Allowed options");
  desc.add_options()                                             //
      ("help", "produce help message")                           //
      ("threads", po::value<int>(), "Number of threads to run")  //
      ("headless", "Run in headless mode");                      //

  const po::parsed_options parsed = po::command_line_parser(argc, argv)
                                        .options(desc)
                                        .allow_unregistered()
                                        .run();

  po::variables_map vm;
  po::store(parsed, vm);
  po::notify(vm);

  if (vm.contains("help")) {
    cout << desc << "\n";
    return make_unexpected("help");
  }

  if (vm.contains("headless")) {
    options.headless = true;
  }

  if (vm.contains("threads")) {
    options.threads = vm["threads"].as<int>();
  } else {
    const auto processor_count = std::thread::hardware_concurrency();
    options.threads = processor_count > 1
                          ? static_cast<int>(processor_count)
                          : 1;
  }

  return options;
}

}  // namespace app

#endif
