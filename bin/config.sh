# Use homebrew llvm
LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
CPPFLAGS="-I/opt/homebrew/opt/llvm/include"
CMAKE_PREFIX_PATH="/opt/homebrew/opt/llvm"
CC="/opt/homebrew/opt/llvm/bin/clang"
CXX="/opt/homebrew/opt/llvm/bin/clang++"
PATH=/opt/homebrew/opt/llvm/bin:$PATH

COMPILER=(
  -s compiler=clang
  -s compiler.version=21
  -s compiler.libcxx=libc++
  -s compiler.cppstd=gnu20
  --build=missing
)

build_conan() {
  BUILD_TYPE="$1"
  BUILD="$2"
  
  conan install . -s build_type=$BUILD_TYPE "${COMPILER[@]}" --output-folder=$BUILD
  cmake -S . -B $BUILD -DCMAKE_TOOLCHAIN_FILE="$BUILD/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=$BUILD_TYPE
}



build_conan Debug build/debug
#build_conan Release build/release



cmake --build build/debug
#cmake --build build/release
