
# Drawing Robot 

## Table of Contents

1. [Overview](#overview)  
2. [Technical Architecture](#technical-architecture)  
   1. [Lexical Analysis](#lexical-analysis)  
   2. [Syntax Parsing](#syntax-parsing)  
   3. [Rendering](#rendering)  
3. [Features](#features)  
4. [Installation and Build](#installation-and-build)  
5. [Usage](#usage)  
6. [Example Commands](#example-commands)  
7. [Screenshots](#screenshots)  
8. [License](#license)

---

## Overview

This project reads a **drawing program** from a file or text input (e.g., `commands.txt`), tokenizes it using regular expressions, checks its syntax according to a simplified grammar (supporting **nested loops**, **colors**, **pen thickness**, etc.), and then uses **SFML** to render the resulting shapes on-screen. A lightweight **TGUI** interface manages help screens, command input, and error messages.

---

## Technical Architecture

### Lexical Analysis

1. Input programs (e.g., `"F 100 R 90 L 2 [ F 50 R 45 ] COLOR K"`) are read as text.  
2. **Regular expressions** split the text into tokens (`F`, `R`, `L`, `COLOR`, `P`, `[`, `]`, and numeric values).  
3. Although tools like Lex/Flex could be used, this project performs lexical analysis directly in C++ (via `std::regex`).

### Syntax Parsing

1. A **recursive parser** checks **nested structures** such as `L <count> [ … ]`, color commands, and pen commands.  
2. **Context-free grammar** ideas are implemented by manually verifying bracket matching, numeric parameters, and valid commands.  
3. Parse errors (missing brackets, invalid tokens, or unknown commands) produce immediate exceptions.

### Rendering

1. Once tokens pass syntax checks, the program calculates **line segments** using commands like `F <distance>` (move forward) and `R <angle>` (rotate).  
2. **SFML** draws the lines in real time, while **TGUI** provides a GUI panel for help, commands input, or error output.  
3. **Pen thickness** and **color** can be changed on the fly, which influences the line rendering.

---

## Features

- **Nested Loops**  
  - `L <count> [ … ]` supports multiple levels, allowing advanced shapes.  
- **Color Switching**  
  - `COLOR <code>` changes the drawing color (K=Red, Y=Green, M=Blue, S=Black).  
- **Pen Thickness**  
  - `P <value>` sets line thickness in pixels (e.g., `P 5` for thicker lines).
- **Automata-Inspired Approach**  
  - Regular expressions handle tokenization.  
  - A small context-free grammar–like parser checks bracket matching and syntax.

---

## Installation and Build

1. **Clone** the repository:
   ```bash
   git clone https://github.com/elifpulukcu/drawing-robot.git
   cd drawing-robot
   ```
2. **Dependencies**:  
   - **CMake** (3.10+ recommended)  
   - **SFML** (2.5+)  
   - **TGUI** (1.6.x or higher)  
   - A modern **C++17** compiler
3. **Configure and Compile**:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```
4. **Run**:
   ```bash
   ./drawing-robot
   ```
   Ensure `commands.txt` is accessible if needed.

---

## Usage

- Launch the application. A **TGUI**-based GUI may appear, or simply parse `commands.txt` automatically (depending on the implementation).  
- **Enter** commands (e.g., `F 100 R 90 COLOR K`) or load them from a file.  
- The parser analyzes the program; if syntax errors occur, an error panel or console output will show.  
- Valid programs generate shapes (lines, loops, color changes, thick lines) on the SFML window.

---

## Example Commands

These lines can be placed in `commands.txt` or typed interactively:

1. **Nested Loops**  
   ```
   L 36 [ L 4 [ F 100 R 90 ] R 10 ]
   ```
   Draws a “loop of loops” pattern.

2. **Color Spiral**  
   ```
   COLOR S P 3 L 36 [ F 10 R 10 ]
   ```
   Creates a spiral with pen thickness 3, color black.

3. **Two Squares**  
   ```
   L 4 [ F 80 R 90 ] R 45 COLOR Y L 4 [ F 80 R 90 ]
   ```
   Draws one square, rotates 45°, changes color to yellow, and draws another.

4. **Pen Variation**  
   ```
   P 2 COLOR K F 80 R 90 P 5 F 80
   ```
   Starts with thin lines in red, then switches to thicker lines.

5. **Complex**  
   ```
   COLOR M L 12 [ F 50 R 30 COLOR K F 20 R 60 ]
   ```
   A multi-iteration loop switching between blue and red lines.

---

## Screenshots

Include any relevant images to show off the shapes. For instance:

| Example | Command |
|---------|---------|
| <img width="826" alt="image" src="https://github.com/user-attachments/assets/1ecb02f2-a34a-4972-bfcf-fe6f86c43d52" /> | `L 36 [ L 4 [ F 100 R 90 ] R 10 ]` |
| <img width="826" alt="image" src="https://github.com/user-attachments/assets/062792a5-4e9c-4119-a7be-e1e3cec68566" /> | `P 5 COLOR M L 8 [ L 4 [ F 80 R 90 ] R 45 COLOR K F 20 R 15 ]` |

---

## License

This project is distributed under the terms of the [MIT License](./LICENSE).  
Any external libraries (SFML, TGUI) maintain their own respective licenses.  
Contributions are welcome under the same license conditions.
