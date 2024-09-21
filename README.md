# Tiger Compiler

This repository contains a compiler for the **Tiger** programming language, written in C. The compiler uses **BSON** for parsing and **FLEX** for lexical analysis. Although the project is not yet complete, it already supports several key language features and generates intermediate code that resembles assembly.

## Features

The current version of the compiler supports the following:

- **Parentheses** handling for expressions.
- Basic **arithmetic operations**: addition (`+`), subtraction (`-`), multiplication (`*`), and division (`/`).
- Recognition of **integers** and **functions**.
- Control structures such as **do-while** and **if-else** statements.
- Generation of **intermediate code**, which is similar to assembly.

> Note: The compiler is designed to generate intermediate code, not actual machine-level assembly. This code acts as a middle step in the compilation process.

## Dependencies

To build and run the Tiger compiler, you need the following dependencies installed:

- **BSON** (for parsing)
- **FLEX** (for lexical analysis)
- **C compiler** (such as GCC or Clang)

## Installation and Usage

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/tiger-compiler.git
    cd tiger-compiler
    ```

2. Compile the project:
    ```bash
    make
    ```

3. Run the compiler on a Tiger source file:
    ```bash
    ./tiger_compiler input_file.tiger
    ```

## Example Code

Here’s a sample of Tiger code that the compiler currently supports:

```tiger
let
  var x := 10
in
  if x > 5 then
    do x := x + 1
  else
    do x := x - 1
end
