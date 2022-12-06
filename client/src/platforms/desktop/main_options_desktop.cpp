#include <boost/program_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <thread>

#include "Application.h"
#include "core.h"

namespace po = boost::program_options;

namespace app {

auto parseApplicationOptions(int argc, char **argv, char **envp)
    -> expected<ApplicationOptions, string> {
  ApplicationOptions options{};

  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()                                             //
      ("help", "produce help message")                           //
      ("threads", po::value<int>(), "Number of threads to run")  //
      ("headless", "Run in headless mode");                      //

  po::parsed_options parsed = po::command_line_parser(argc, argv)
                                  .options(desc)
                                  .allow_unregistered()
                                  .run();

  po::variables_map vm;
  po::store(parsed, vm);
  po::notify(vm);

  std::vector<std::string> to_pass_further =
      collect_unrecognized(parsed.options, po::include_positional);
  if (to_pass_further.size() > 0) {
    cout << "unknown option: " << to_pass_further.at(0) << endl;
    cout << desc << endl;
    return make_unexpected("unknown option");
  }

  if (vm.count("help")) {
    cout << desc << "\n";
    return make_unexpected("help");
  }

  if (vm.count("headless")) {
    options.headless = true;
  }

  if (vm.count("threads")) {
    options.threads = vm["threads"].as<int>();
  } else {
    // may return 0 when not able to detect
    const auto processor_count = std::thread::hardware_concurrency();
    options.threads = processor_count > 1    //
                          ? processor_count  // Use what we are told...
                          : 1;               // 0 means can't detect, asume 0
  }

  return options;
}

}  // namespace app
