# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2022.3.3\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2022.3.3\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\projects\ALEX_Graph1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\projects\ALEX_Graph1\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/stx_graph_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/stx_graph_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/stx_graph_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stx_graph_test.dir/flags.make

CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.obj: CMakeFiles/stx_graph_test.dir/flags.make
CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.obj: CMakeFiles/stx_graph_test.dir/includes_CXX.rsp
CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.obj: D:/projects/ALEX_Graph1/src/stx-btree/stx_graph_test.cpp
CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.obj: CMakeFiles/stx_graph_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\projects\ALEX_Graph1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.obj -MF CMakeFiles\stx_graph_test.dir\src\stx-btree\stx_graph_test.cpp.obj.d -o CMakeFiles\stx_graph_test.dir\src\stx-btree\stx_graph_test.cpp.obj -c D:\projects\ALEX_Graph1\src\stx-btree\stx_graph_test.cpp

CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\projects\ALEX_Graph1\src\stx-btree\stx_graph_test.cpp > CMakeFiles\stx_graph_test.dir\src\stx-btree\stx_graph_test.cpp.i

CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\projects\ALEX_Graph1\src\stx-btree\stx_graph_test.cpp -o CMakeFiles\stx_graph_test.dir\src\stx-btree\stx_graph_test.cpp.s

# Object files for target stx_graph_test
stx_graph_test_OBJECTS = \
"CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.obj"

# External object files for target stx_graph_test
stx_graph_test_EXTERNAL_OBJECTS =

stx_graph_test.exe: CMakeFiles/stx_graph_test.dir/src/stx-btree/stx_graph_test.cpp.obj
stx_graph_test.exe: CMakeFiles/stx_graph_test.dir/build.make
stx_graph_test.exe: CMakeFiles/stx_graph_test.dir/linklibs.rsp
stx_graph_test.exe: CMakeFiles/stx_graph_test.dir/objects1.rsp
stx_graph_test.exe: CMakeFiles/stx_graph_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\projects\ALEX_Graph1\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable stx_graph_test.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\stx_graph_test.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/stx_graph_test.dir/build: stx_graph_test.exe
.PHONY : CMakeFiles/stx_graph_test.dir/build

CMakeFiles/stx_graph_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\stx_graph_test.dir\cmake_clean.cmake
.PHONY : CMakeFiles/stx_graph_test.dir/clean

CMakeFiles/stx_graph_test.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\projects\ALEX_Graph1 D:\projects\ALEX_Graph1 D:\projects\ALEX_Graph1\cmake-build-debug D:\projects\ALEX_Graph1\cmake-build-debug D:\projects\ALEX_Graph1\cmake-build-debug\CMakeFiles\stx_graph_test.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/stx_graph_test.dir/depend

