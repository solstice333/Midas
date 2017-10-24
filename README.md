# Cone Programming Language
Cone is a statically-type, object-oriented programming language, 
used to build realistic, interactive, [3D web][3dweb] worlds.
It is designed for real-time performance and responsiveness, compile-time safety, 
and programmer flexibility and convenience.

The Cone compiler is currently under development and in very early stages.
The current status and next steps are documented in [PLAN.md][plan].

## Language Features

When finished, Cone will support these features:

- Readable, modular marriage of 3D content and behavior:
  - 'this' blocks and operators for specifying static or runtime content
  - @ operator for snap-together inclusion of Internet-hosted content
  - Content iterators and 'each' control block
- Url-based packaging and resource handling:
  - Dynamic, asynchronous-capable resource loading
  - Extensible url schemes and content type de-serialization
  - Programs are treated as dynamically loadable resources
  - Automatic resource unzip and de-compression
  - Relative url addressing
- Memory, type, and concurrency safe
- [Gradual memory management][gmm]
  - Supports use of multiple GCs in one program
  - Static allocators: stack and Rust-like lexical GC
  - Runtime GCs: reference counting and tracing GC
  - Object pools and arenas
  - Lifetime-checked borrowed references
- Robust type system
  - Sum types, structs, arrays, slices, ranges, aliases
  - Type inference
  - Structural and nominative typing
- Modules, Interfaces, Generics and Classes
  - Multiple inheritance
  - Method override and super
  - Operator overloading
  - Computable properties (get/set functions and closures)
- Lightweight concurrency
  - Co-routines, threads and actors
  - Lockless and locked permissions for safe data sharing
- Functions, Methods and Closures
  - Variadic parameters and multiple return values
  - Implicit return and tail-call optimization
- Extensible pattern matching:
  - Type-defined '~~' match operator
  - 'match' blocks using custom match methods
  - Content extraction during matching
- 'do' block for context management
- Concise, readable code:
  - 'this'-implied prefix operators for method cascading, etc.
  - Control clauses for 'if', 'while', 'each'
  - Parallel assignment
  - Auto-detected off-side rule
- Unicode-aware (UTF8) text strings and variable names
- Fast compilation and convenient packaging

## Examples and Documentation

Language documentation and examples are TBD.
That said, Cone will strongly resemble its dynamically-typed cousin [Acorn][acorn],
which has a [showcase][acornshow] and [reference documentation][acornref].

## Building (Windows)

A Visual C++ solutions file can be created using the VS project file.
The generated object and executable files are created relative to the location of the 
solutions file. The build depends on LLVM being installed and available.

## Building (Linux)

To build on Linux (TBD):

	cmake

## License

The Cone programming language compiler is distributed under the terms of the MIT license. 
See LICENSE and COPYRIGHT for details.

[3dweb]: http://web3d.jondgoodwin.com/faq.html
[gmm]: http://jondgoodwin.com/pling/gmm.pdf
[plan]: https://github.com/jondgoodwin/cone/blob/master/PLAN.md
[acorn]: https://github.com/jondgoodwin/acornvm
[acornshow]: http://web3d.jondgoodwin.com/acorn/showcase.html
[acornref]: http://web3d.jondgoodwin.com/acorn