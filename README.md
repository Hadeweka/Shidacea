This repository was migrated and might be deleted in the future.
Please refer to https://github.com/Shidacea/Shidacea for the new repository.

# Shidacea 2D Game Engine

![BuildUbuntu](https://github.com/Hadeweka/Shidacea/workflows/BuildUbuntu/badge.svg)
![BuildWindows](https://github.com/Hadeweka/Shidacea/workflows/BuildWindows/badge.svg)

<img src="https://img.shields.io/github/v/release/Hadeweka/Shidacea"> <img src="https://img.shields.io/github/release-date/Hadeweka/Shidacea">

<img src="https://img.shields.io/github/license/Hadeweka/Shidacea"> 

What you see is the game engine Shidacea, decoupled from my game Inuhh Shinvasion 2. It can be used for rapid development and full applications.

This repository is an engine, a template for game projects and also a launcher for custom projects.
Depending on your level of knowledge and desire to mess around with my code, you can do several things:

* Download projects from other people and launch them using an executable of Shidacea
* Write your own project using mruby scripts and test them directly
* Compile your project into an executable using Shidacea
* Change the core mruby scripts of Shidacea and add new ones
* Wrap your own methods in C++ using the MrbWrap modules
* Dive into mruby or one of the submodules and modify them
* Disassemble the whole thing, write some assembly code and segfault yourself into oblivion (not recommended)

Overall, this engine was designed to appeal to many people instead of just a single group. 
However, a certain degree of knowledge about Ruby is needed.

# Idea and history

This project was inspired by the RPG Maker series from Enterbrain and complications with previous games written in Ruby. 
It aims to combine the beauty of Ruby for scripting with the speed of C++ for critical procedures while maintaining platform independency.

The original purpose of this engine was to serve as a backbone to Inuhh Shinvasion 2, but it quickly became its own thing.
Even if the original idea was to create a Jump'n'run game, it can technically be used for any 2D project.

The final version will include the following components:

* Shidacea - The main engine
* Launshi - A launcher for custom projects
* Hyashi - A level editor

# Requirements

The following programs need to be installed:

* Compiler with C++17 support
* CMake
* Ruby
* Bison

For Windows you need to make sure that the paths leading to ruby.exe and bison.exe are in the PATH variable.
CMake can be obtained using Visual Studio, which is the recommended option.
Also make sure to avoid path names with spaces in them, as mruby might fail to compile otherwise.

Alternatively, you can use pre-compiled Windows binaries for each Shidacea release version.

# Usage

There are two main ways to use Shidacea.

First, you can compile Shidacea without any scripts to a launcher.
If you want to execute a project from someone else, just download its script files and put them into the custom folder.

The second option is to compile your script files together with the Shidacea engine.
This allows for more optimized code at the expense of portability.
To use this, use `-DSHIDACEA_COMPILE_FRONTEND=ON` as option for CMake,
which will compile the frontend project as bytecode directly into the executable.

It is also possible to use `-DSHIDACEA_COMPILE_SDCLIB=ON` to compile the Shidacea library into the executable.
This is done by default in the release versions and recommended unless you want to change the library functions.

Furthermore, the engine core with the SFML bindings can be disabled with `-DSHIDACEA_EXCLUDE_SFML=OFF`.
This is only necessary if you want to use your own framework for Shidacea.

## Load scripts at runtime

Advantages:
* Platform independent projects
* Precompiled executables can be used for common platforms
* After installing Shidacea, you don't need to touch it anymore

Disadvantages:
* Everyone can see and modify your code without effort
* Evaluating the script files takes longer than using bytecode
* You cannot change the Shidacea core engine directly

## Precompiled scripts

Advantages:
* Faster evaluation of script files
* Obfuscated scripts
* You have full control over the engine and can change core aspects

Disadvantages:
* Your projects need to be recompiled every time you change a script
* You need to have at least basic knowledge about C++ compiling and CMake
* Your project needs to be recompiled for each platform

# Properties

* Support for many different platforms (including Windows and Linux)
* Scripting support using mruby
* Fast SFML based game engine with mruby bindings
* High flexibility allowing for custom builds
* Modding support

# Features

* Simple state machine for scenes
* Basic game classes and physics
* Resource management system
* Simple event handling with pseudo-parallel routines
* Intuitive and simple system for entity properties
* Simple entity scripting
* Fast collision detection routines
* Wrapped ImGui methods for debugging

# Roadmap

Note that any version below 1.0.0 is a development version and will likely contain breaking changes.

The elements in this roadmap are only for orientation and are likely to change in the future.

## Version 0.2.1 (Summer 2020)
* [ ] Automated release builds
* [ ] Documentation page

## Version 0.3.0 - Dynamics update (Late summer 2020)
* [ ] Animations
* [ ] More transformations (like rotation methods for triangles)
* [ ] Basic GUI methods
* [ ] Sound looping
* [ ] Ellipse shape support
* [ ] Point drawing
* [ ] Particle system
* [ ] Installation script
* [ ] Dedicated file specifying asset folders
* [ ] Keyword filters for Launshi
* [ ] Better mod support

## Version 0.4.0 - Map update (Late 2020)
* [ ] Full map support
* [ ] A special level editor written using this engine (will be called Hyashi)
* [ ] Hyashi plugin support

## Version 0.5.0 - Network update (Early 2021)
* [ ] Actually useful network support

## Version 1.0.0 - Full release
* [ ] Full release
* [ ] More example projects

## Possible features in future versions
* [ ] WYSIWYG-type game editor feature for Hyashi
* [ ] Crystal implementation named Shipectral, compatible with Hyashi
* [ ] Resource packages
* [ ] Shader support
* [ ] Support for arbitrary shape transformations for collisions

# History

## Version 0.2.0
* Entirely new engine structure
* Z-Ordering
* Unicode support
* Arbitrary project structures for compilation
* Collision routines for points, lines, circles, AABBs and triangles
* Marshalling support for basic objects
* Basic text input fields
* Filters for Launshi
* Debug routines
* Better resource management
* Shape drawing
* Launshi info window with description and (one) screenshot
* Collision test example
* Shooter example project

# Caveats

## Submodules

If you download this repository, make sure to download the submodules as well, since a simple download won't include them.
The best way to use Shidacea is to fork it and use Git for maintaining it and updating the submodules (e.g. using `git clone --recursive`).

List of current submodules:
* mruby
* SFML
* ImGui
* ImGui-SFML
* MrbWrap
* Launshi
* Collishi

## Loading other mruby script files

The ruby instruction `require` is currently NOT available (adding it as a gem would be possible, but not trivially applicable to the concept of pre-compiled bytecode).
This problem is circumvented by specification of a JSON file in which the scripts are listed in execution order.
For example, the SDCLib and the frontend (e.g. Launshi) require the use of a script order, as well as Launshi projects.
It is possible to name single files and complete directories (ending with a '/').
Note that the order of scripts loaded in a directory needs to be regarded as random, so avoid dependencies between files in a directory if loaded in a batch.

## Linux troubleshooting

This project should (read: SHOULD) run on Linux systems.
However, several libraries are required to do so (see documentation for SFML).
Some problems are discussed below.

### Dependencies are missing when generating or compiling

Make sure that g++, Ruby, Bison and CMake are installed properly. 
Also visit the SFML documentation to check for requirements.

If the filesystem library is missing, g++ needs to be updated to version 8 or higher.
If the problem persists after installing the missing libraries and programs, you may want to reboot and try again.

### Executables trigger a segmentation fault on WSL

Shidacea may not work correctly on WSL despite a successful compilation.
If a segmentation fault occurs on WSL, change the XServer application to another one.
The segmentation fault issue is known to occur on XMing, for example.
The Windows store application X410 works fine, but is not free.
Reverting SFML to an earlier version can also work.

### Shidacea yields other error messages on WSL

Try the solution above first.
If that does not help, the command `export LIBGL_ALWAYS_INDIRECT=` might help, too.
Also make sure to properly connect your WSL with the correct XServer and a PulseAudio server.
If Shidacea runs otherwise flawlessly, some errors are completely normal.

### Other problems

Please report your problem as an issue if your problem is none of the above or none of the solutions worked.
Shidacea aims to be compatible to as many platforms as possible, so please help reaching this goal.
