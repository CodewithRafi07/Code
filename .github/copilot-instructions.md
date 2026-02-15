# Copilot Instructions for Rafi-s_Code

## Overview
This workspace contains a variety of C, C++, and some Python projects, organized by topic, assignment, and output directories. There is no monolithic architecture; instead, the codebase is a collection of independent programs and exercises, mostly for learning and experimentation.

## Directory Structure & Key Components
- `CPP/` and `c/`: Main directories for C++ and C source files, with subfolders for assignments, problems, and outputs.
- `output/`: Each major directory has its own output folder for compiled binaries and data files. Do not mix outputs between projects.
- `Project_1/`, `Codeforce/`, `Data Struct/`: Themed folders for specific projects, competitive programming, and data structure exercises.
- `All_My_Work/`, `Chapter/`, `Chapter_Practice/`: Organizational folders for coursework and personal projects.

## Build & Run Workflows
- **C/C++ Compilation:**
  - Use MinGW (`g++.EXE` for C++, `gcc.EXE` for C) for compiling. Example:
    ```powershell
    C:\MinGW\bin\g++.EXE -Wall -Wextra -g3 d:\Code\CPP\book_class.cpp -o d:\Code\CPP\output\book_class.exe
    ```
  - Output binaries should be placed in the corresponding `output/` directory.
- **No global build system:** Each file or project is compiled individually. There are no Makefiles or CMakeLists.txt.
- **Debugging:** Use the `-g3` flag for debug builds. Debug output is not standardized; inspect the source for print/debug statements.

## Project-Specific Patterns
- **File Naming:**
  - Source files are named by topic or assignment (e.g., `Diamond.c`, `student_class.cpp`).
  - Output files and data (e.g., `.exe`, `.dat`) are stored in `output/` subfolders.
- **No unified main entrypoint:** Each `.c` or `.cpp` file is typically a standalone program.
- **Minimal external dependencies:** Most code is standard C/C++ with no third-party libraries.

## Integration & Cross-Component Communication
- **No inter-project integration:** Projects do not share code or data except for output conventions.
- **Data files:** Some programs read/write `.dat` files in their output folders. Example: `library.dat` in `c/output/`.

## Conventions & Recommendations for AI Agents
- Always place compiled binaries and generated data in the correct `output/` folder for the project.
- When adding new assignments or problems, follow the existing folder and naming conventions.
- Avoid introducing global build scripts or dependencies unless requested.
- Reference existing files for examples of input/output handling and program structure.
- If unsure about a workflow, check for recent terminal commands for compilation patterns.

## Example: Adding a New C++ Assignment
1. Place your `.cpp` file in the relevant assignment folder (e.g., `CPP/Assignment 1/`).
2. Compile using MinGW, outputting to the folder's `output/` directory.
3. Document any custom input/output formats in comments at the top of your file.

---
For questions or unclear conventions, ask the user for clarification or examples from their recent work.
