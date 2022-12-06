#include "main_options.h"

#ifdef __EMSCRIPTEN__
#include "emscripten/main_options_emscripten.cpp"
#else
#include "desktop/main_options_desktop.cpp"
#endif
