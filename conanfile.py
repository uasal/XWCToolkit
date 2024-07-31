import os
import shutil

from conan import ConanFile
from conan.tools.build import check_min_cppstd, check_min_cstd
from conan.tools.cmake import  CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.system.package_manager import Apt
from conan.errors import ConanInvalidConfiguration


class XWCRecipe(ConanFile):
    name = "xwc"
    version = "1.0"

    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of hello package here>"
    topics = ("Astronomy", "XWC", "AO", "framework")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"NEED_CUDA": [True, False],
               "NEED_CACAO": [True, False],
               "MAGAOX_NOEDT": [True, False]}

    default_options = {"NEED_CUDA": False,
                       "NEED_CACAO": False,
                       "MAGAOX_NOEDT": True}

    exports_sources = "CMakeLists.txt"

    # def source(self):
    #     # Please, be aware that using the head of the branch instead of an immutable tag
    #     # or commit is a bad practice and not allowed by Conan
    #     get(self, "https://github.com/conan-io/libhello/archive/refs/heads/main.zip",
    #               strip_root=True)

    def validate(self):
        check_min_cppstd(self, "17")
        # check_min_cstd(self, "11")

    # def layout(self):
    #     cmake_layout(self, src_folder="src")

    def system_requirements(self):
        apt = Apt(self)
        # First checks if these libraries exists; it will try to update the package manager database before checking and installing
        apt.install(["libpthread-stubs0-dev", "libudev-dev", "liblapack-dev", "liblapacke-dev", "libncurses5-dev", "libncursesw5-dev"], update=True, check=True)

    def requirements(self):
        self.requires("eigen/3.3.7")
        self.requires("zlib/1.2.11")
        self.requires("openblas/0.3.24")
        self.requires("cfitsio/3.470")
        self.requires("llvm-openmp/13.1.0")
        self.requires("flatbuffers/23.5.26")
        # Alert user about additional system dependencies
        if shutil.which("cmake") is None:
            self.output.warn("Warning: 'milk' is required for full functionality but doesn't appear to be installed or on the PATH. Please install it manually.")
        if self._check_folder_existence(search_folder="/usr/local/lib", target_folder="mx"):
            self.output.warn("Warning: 'mxlib' is required for full functionality but doesn't appear to be installed or on the PATH. Please install it manually.")
        if self._check_folder_existence(search_folder="/usr/local/include", target_folder="xrif"):
            self.output.warn("Warning: 'xrif' is required for full functionality but doesn't appear to be installed or on the PATH. Please install it manually.")
        # if self._check_folder_existence(search_folder="/usr/local/include", target_folder="instGraph"):
        #     self.output.warn("Warning: 'instGraph' is required for full functionality but doesn't appear to be installed or on the PATH. Please install it manually.")

    def _check_folder_existence(self, search_folder="/usr/local/lib", target_folder=""):
        include_dirs = ["usr/local/lib"]
        for include_dir in include_dirs:
            if os.path.exists(os.path.join(include_dir, target_folder)):
                return True
        return False

    def build_requirements(self):
        self.tool_requires("cmake/[>=3.16.0 <4.0.0]")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        # cmake.configure(source_folder="source")
        cmake.build()

    def package(self):
        cmake = CMake(self)
        # cmake.configure(source_folder="source")
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["XWC"]        