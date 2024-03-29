# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/development/SkeletalAnimationOpenGL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/development/SkeletalAnimationOpenGL

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/user/development/SkeletalAnimationOpenGL/CMakeFiles /home/user/development/SkeletalAnimationOpenGL//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/user/development/SkeletalAnimationOpenGL/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named animation

# Build rule for target.
animation: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 animation
.PHONY : animation

# fast build rule for target.
animation/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/build
.PHONY : animation/fast

src/Mesh.o: src/Mesh.cpp.o
.PHONY : src/Mesh.o

# target to build an object file
src/Mesh.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/Mesh.cpp.o
.PHONY : src/Mesh.cpp.o

src/Mesh.i: src/Mesh.cpp.i
.PHONY : src/Mesh.i

# target to preprocess a source file
src/Mesh.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/Mesh.cpp.i
.PHONY : src/Mesh.cpp.i

src/Mesh.s: src/Mesh.cpp.s
.PHONY : src/Mesh.s

# target to generate assembly for a file
src/Mesh.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/Mesh.cpp.s
.PHONY : src/Mesh.cpp.s

src/RenderContext.o: src/RenderContext.cpp.o
.PHONY : src/RenderContext.o

# target to build an object file
src/RenderContext.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/RenderContext.cpp.o
.PHONY : src/RenderContext.cpp.o

src/RenderContext.i: src/RenderContext.cpp.i
.PHONY : src/RenderContext.i

# target to preprocess a source file
src/RenderContext.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/RenderContext.cpp.i
.PHONY : src/RenderContext.cpp.i

src/RenderContext.s: src/RenderContext.cpp.s
.PHONY : src/RenderContext.s

# target to generate assembly for a file
src/RenderContext.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/RenderContext.cpp.s
.PHONY : src/RenderContext.cpp.s

src/Shader.o: src/Shader.cpp.o
.PHONY : src/Shader.o

# target to build an object file
src/Shader.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/Shader.cpp.o
.PHONY : src/Shader.cpp.o

src/Shader.i: src/Shader.cpp.i
.PHONY : src/Shader.i

# target to preprocess a source file
src/Shader.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/Shader.cpp.i
.PHONY : src/Shader.cpp.i

src/Shader.s: src/Shader.cpp.s
.PHONY : src/Shader.s

# target to generate assembly for a file
src/Shader.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/Shader.cpp.s
.PHONY : src/Shader.cpp.s

src/Texture.o: src/Texture.cpp.o
.PHONY : src/Texture.o

# target to build an object file
src/Texture.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/Texture.cpp.o
.PHONY : src/Texture.cpp.o

src/Texture.i: src/Texture.cpp.i
.PHONY : src/Texture.i

# target to preprocess a source file
src/Texture.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/Texture.cpp.i
.PHONY : src/Texture.cpp.i

src/Texture.s: src/Texture.cpp.s
.PHONY : src/Texture.s

# target to generate assembly for a file
src/Texture.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/Texture.cpp.s
.PHONY : src/Texture.cpp.s

src/VertexArray.o: src/VertexArray.cpp.o
.PHONY : src/VertexArray.o

# target to build an object file
src/VertexArray.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/VertexArray.cpp.o
.PHONY : src/VertexArray.cpp.o

src/VertexArray.i: src/VertexArray.cpp.i
.PHONY : src/VertexArray.i

# target to preprocess a source file
src/VertexArray.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/VertexArray.cpp.i
.PHONY : src/VertexArray.cpp.i

src/VertexArray.s: src/VertexArray.cpp.s
.PHONY : src/VertexArray.s

# target to generate assembly for a file
src/VertexArray.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/VertexArray.cpp.s
.PHONY : src/VertexArray.cpp.s

src/main.o: src/main.cpp.o
.PHONY : src/main.o

# target to build an object file
src/main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/main.cpp.o
.PHONY : src/main.cpp.o

src/main.i: src/main.cpp.i
.PHONY : src/main.i

# target to preprocess a source file
src/main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/main.cpp.i
.PHONY : src/main.cpp.i

src/main.s: src/main.cpp.s
.PHONY : src/main.s

# target to generate assembly for a file
src/main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/animation.dir/build.make CMakeFiles/animation.dir/src/main.cpp.s
.PHONY : src/main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... animation"
	@echo "... src/Mesh.o"
	@echo "... src/Mesh.i"
	@echo "... src/Mesh.s"
	@echo "... src/RenderContext.o"
	@echo "... src/RenderContext.i"
	@echo "... src/RenderContext.s"
	@echo "... src/Shader.o"
	@echo "... src/Shader.i"
	@echo "... src/Shader.s"
	@echo "... src/Texture.o"
	@echo "... src/Texture.i"
	@echo "... src/Texture.s"
	@echo "... src/VertexArray.o"
	@echo "... src/VertexArray.i"
	@echo "... src/VertexArray.s"
	@echo "... src/main.o"
	@echo "... src/main.i"
	@echo "... src/main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

