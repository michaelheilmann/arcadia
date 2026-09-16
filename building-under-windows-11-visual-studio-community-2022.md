# Building under Windows 11/Visual Studio Community 2022

## Installing the dependencies
*michaelheilmann.com* requires
- *Microsoft Visual Studio Community 2022* version 17.9.2 or better.
  *Microsoft Visual Studio Community 2022* is available at [https://visualstudio.microsoft.com/vs/community/](https://visualstudio.microsoft.com/vs/community/).
- *Kitware CMake* version 3.29 or better.
  *Kitware CMake* is available at [https://cmake.org/](https://cmake.org/).
- *PowerShell*

## Integrating external libraries


The dependencies are downloaded from the [releases](https://github.com/michaelheilmann/arcadia-dependencies/releases) of
[https://github.com/michaelheilmann/arcadia-dependencies](https://github.com/michaelheilmann/arcadia-dependencies).

Under Linux, of course, such ordeals are not necessary.

## Building the program
The following instructions use CMake presets to perform an out-of-source build. An out-of-source build does not modify the source directory
by storing any build file, intermediate file, or product file in a dedicated build directory outside of the source directory.
An out-of-source build is the recommended way of building *michaelheilmann.com*.

- Checkout [https://github.com/michaelheilmann/arcadia](https://github.com/michaelheilmann/arcadia) into a directory in your file system.
  For the remainder of this manual, we call this directory *source directory* and denote it in command-line prompts by the meta variable `<source-directory>`.
- Create a build directory outside of the source directory, for example as a sibling of the source directory.
  For the remainder of this manual, we call this directory *build directory* and denote it in command-line prompts by the meta variable `<build-directory>`.
- Open a console and enter the build directory `<build-directory>`.
- The file `<source-directory>/CMakePresets.json` provides presets for the target architectures `x64` and `Win32`.
  To generate build files, use one of these configure presets:
    - `cmake -S <source directory> -B ./x64 --preset x64`
    - `cmake -S <source directory> -B ./Win32 --preset Win32`
- The Visual Studio Community 2022 solution file `Arcadia.sln` was generated in the preset build directory.
- **Important**: Ensure the dependencies are available by downloading them from the releases of [https://github.com/michaelheilmann/arcadia-dependencies](https://github.com/michaelheilmann/arcadia-dependencies).
  To facilitate this task, the *CMake* target `Arcadia.InstallDependencies` is provided. Simple execute:
  - `cmake --build ./x64 --target "Arcadia.InstallDependencies"`
  - `cmake --build ./Win32 --target "Arcadia.InstallDependencies"`
- To build, use one of the following commands:
  - `cmake --build ./x64 --config Debug`
  - `cmake --build ./x64 --config Release`
  - `cmake --build ./x64 --config MinSizeRel`
  - `cmake --build ./x64 --config RelWithDebInfo`
  - `cmake --build ./Win32 --config Debug`
  - `cmake --build ./Win32 --config Release`
  - `cmake --build ./Win32 --config MinSizeRel`
  - `cmake --build ./Win32 --config RelWithDebInfo`
  or open the the solution file in `x64/Arcadia.sln` or `Win32/Arcadia.sln`, respectively.

#### Example command line
The typical command line for x64 Debug build files is hence:
```
cd C:\
mkdir Arcadia
cd Arcadia

mkdir Source
cd Source

git clone https://github.com/michaelheilmann/arcadia .
cd ..

mkdir Build
cd Build

cmake -S ./../Source -B ./x64 --preset x64

cmake --build ./x64 --target "Arcadia.InstallDependencies"
cmake --build ./x64 --config Debug
```
