# AbeArgs

AbeArgs is a lightweight C++ library for simplified command-line argument parsing. It is designed to be easy to integrate into your C++ projects, with minimal dependencies and a straightforward API.

## Features

- Simple and intuitive argument parsing
- Cross-platform support (Windows, Linux, macOS)
- Debug and Release builds
- Unit tests using CppUnit (for Debug builds)

## Getting Started

### Prerequisites

- CMake 3.15 or newer
- C++20 compatible compiler (tested with GCC, MSVC, Clang)
- [CppUnit](https://sourceforge.net/projects/cppunit/) (for running tests, included as a third-party dependency)

### Building the Library

1. **Clone the repository:**
   ```sh
   git clone https://github.com/yourusername/abeargs.git
   cd abeargs
   ```

2. **Configure and build with CMake:**
   ```sh
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

   This will build the static library `libAbeArgs.a` (or `AbeArgs.lib` on Windows) in the `abeargs_lib` directory.

### Running Unit Tests

Unit tests are only built in Debug mode.

```sh
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
./abeargs_tests/AbeArgsTests
```

### Integrating AbeArgs Into Your Project

1. **Add the library and include directory:**

   - Copy the contents of [`abeargs_lib`](abeargs_lib) to your project, or add this repository as a submodule.
   - Add the following to your `CMakeLists.txt`:

     ```cmake
     add_subdirectory(path/to/abeargs/abeargs_lib)
     target_link_libraries(your_target PRIVATE AbeArgs)
     target_include_directories(your_target PRIVATE path/to/abeargs/abeargs_lib)
     ```

2. **Include and use in your code:**

   ```cpp
   #include "abeargs.h"

   int main(int argc, char* argv[]) {
       abeargs::Parser parser(argc, argv);
       // Define and parse your arguments
   }
   ```

### Example

```cpp
#include "abeargs.h"

int main(int argc, char* argv[]) {
    abeargs::Parser parser(argc, argv);
    parser.add_argument("--input").required();
    parser.add_argument("--verbose").flag();

    if (!parser.parse()) {
        parser.print_help();
        return 1;
    }

    std::string input = parser.get("--input");
    bool verbose = parser.get("--verbose");
    // ... use arguments ...
}
```

## Building and Installing cppunit with Multiple Compilers

The following steps guide you through building and installing `cppunit` in a way that allows you to link it with different C++ compilers. This process creates an isolated build and installation directory, ensuring that your main system environment remains unaffected. The steps are as follows:

1. **Create a Build Directory:**  
   This keeps build artifacts separate from your source code, making it easier to manage and clean up.

2. **Navigate to the Build Directory:**  
   All subsequent build commands will be executed from within this directory.

3. **Configure the Build:**  
   The `configure` script is run with custom compiler flags (`-g -O0` for debugging) and the `--enable-debug` option. The `--prefix` option specifies a local installation directory inside the build folder, allowing you to install multiple versions or builds for different compilers without conflicts.

4. **Compile the Source:**  
   The `make` command builds the library using the selected compiler and flags.

5. **Install the Library:**  
   The `make install` command installs the built library and headers into the specified prefix directory, making it easy to link against this version from your projects.

6. **Archive the Files:**
   Compress the folders/files into a .zip or .tar.gz archive to be included in the thirdparty libs.

By repeating these steps with different compiler environments (e.g., by setting the `CXX` environment variable before running `configure`), you can maintain separate builds of `cppunit` for each compiler you wish to support.

```
0) begin in the cppunit root directory
1) mkdir build
2) cd build
3) ../configure CXXFLAGS='-g -O0' --enable-debug --prefix=$(pwd)/build/
4) make
5) make install
6) archive the folders/files for inclusion in the thirdparty libs folder
```

## License

This project is licensed under the [Universal Permissive License v 1.0](https://oss.oracle.com/licenses/upl/).

## Author

Abe Mishler

---

For more details, see the source files in [`abeargs_lib`](abeargs_lib).