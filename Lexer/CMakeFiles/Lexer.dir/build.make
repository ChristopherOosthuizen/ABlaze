# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/chris/Projects/ABlaze

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chris/Projects/ABlaze

# Include any dependencies generated for this target.
include Lexer/CMakeFiles/Lexer.dir/depend.make

# Include the progress variables for this target.
include Lexer/CMakeFiles/Lexer.dir/progress.make

# Include the compile flags for this target's objects.
include Lexer/CMakeFiles/Lexer.dir/flags.make

Lexer/CMakeFiles/Lexer.dir/Lexer.cpp.o: Lexer/CMakeFiles/Lexer.dir/flags.make
Lexer/CMakeFiles/Lexer.dir/Lexer.cpp.o: Lexer/Lexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chris/Projects/ABlaze/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Lexer/CMakeFiles/Lexer.dir/Lexer.cpp.o"
	cd /home/chris/Projects/ABlaze/Lexer && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Lexer.dir/Lexer.cpp.o -c /home/chris/Projects/ABlaze/Lexer/Lexer.cpp

Lexer/CMakeFiles/Lexer.dir/Lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lexer.dir/Lexer.cpp.i"
	cd /home/chris/Projects/ABlaze/Lexer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chris/Projects/ABlaze/Lexer/Lexer.cpp > CMakeFiles/Lexer.dir/Lexer.cpp.i

Lexer/CMakeFiles/Lexer.dir/Lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lexer.dir/Lexer.cpp.s"
	cd /home/chris/Projects/ABlaze/Lexer && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chris/Projects/ABlaze/Lexer/Lexer.cpp -o CMakeFiles/Lexer.dir/Lexer.cpp.s

# Object files for target Lexer
Lexer_OBJECTS = \
"CMakeFiles/Lexer.dir/Lexer.cpp.o"

# External object files for target Lexer
Lexer_EXTERNAL_OBJECTS =

Lexer/libLexer.a: Lexer/CMakeFiles/Lexer.dir/Lexer.cpp.o
Lexer/libLexer.a: Lexer/CMakeFiles/Lexer.dir/build.make
Lexer/libLexer.a: Lexer/CMakeFiles/Lexer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chris/Projects/ABlaze/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libLexer.a"
	cd /home/chris/Projects/ABlaze/Lexer && $(CMAKE_COMMAND) -P CMakeFiles/Lexer.dir/cmake_clean_target.cmake
	cd /home/chris/Projects/ABlaze/Lexer && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Lexer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Lexer/CMakeFiles/Lexer.dir/build: Lexer/libLexer.a

.PHONY : Lexer/CMakeFiles/Lexer.dir/build

Lexer/CMakeFiles/Lexer.dir/clean:
	cd /home/chris/Projects/ABlaze/Lexer && $(CMAKE_COMMAND) -P CMakeFiles/Lexer.dir/cmake_clean.cmake
.PHONY : Lexer/CMakeFiles/Lexer.dir/clean

Lexer/CMakeFiles/Lexer.dir/depend:
	cd /home/chris/Projects/ABlaze && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chris/Projects/ABlaze /home/chris/Projects/ABlaze/Lexer /home/chris/Projects/ABlaze /home/chris/Projects/ABlaze/Lexer /home/chris/Projects/ABlaze/Lexer/CMakeFiles/Lexer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Lexer/CMakeFiles/Lexer.dir/depend

