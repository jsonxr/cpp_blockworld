from conan import ConanFile
from conan.errors import ConanInvalidConfiguration


class Blockworld(ConanFile):
    generators = "CMakeDeps", "CMakeToolchain"
    settings = "os", "compiler", "build_type", "arch"

    def requirements(self):
        # Common dependencies for all platforms
        self.requires("entt/3.13.0")
        self.requires("expected-lite/0.6.3")
        self.requires("glad/0.1.36")
        self.requires("glm/cci.20230113")
        self.requires("nlohmann_json/3.11.3")
        self.requires("stb/cci.20230920")
        self.requires("tinygltf/2.8.19")
        self.requires("ms-gsl/4.0.0")
        #self.requires("fmt/10.1.1")

        # Desktop-only dependencies
        if self.settings.os != "Emscripten":
            self.requires("boost/1.84.0")
            self.requires("glfw/3.4")
        # Note: For Emscripten, GLFW is provided by -s USE_GLFW=3 flag
    # def build_requirements(self):
    #     self.tool_requires("cmake/3.28")

    def layout(self):
        self.folders.generators = ""
