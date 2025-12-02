#ifndef APP_MAIN_OPTIONS_H
#define APP_MAIN_OPTIONS_H

#include "Application.h"
#include "core.h"

namespace app {

auto parseApplicationOptions(int argc, char **argv)
    -> expected<ApplicationOptions, string>;

}

#endif  // APP_MAIN_OPTIONS_H
