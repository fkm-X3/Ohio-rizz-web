# Rusty Browser

A hybrid web browser project acting as a lightweight browser engine. It combines a **Rust** frontend (handling windowing, event loops, and buffer management) with a **C++** backend (handling the core rendering logic).

## Prerequisites

Before building, ensure you have the following installed:

1.  **Rust**: [Install Rust](https://www.rust-lang.org/tools/install) (includes `cargo`).
2.  **C++ Compiler**: A compatible C++ compiler.
    *   **Windows**: Visual Studio Build Tools (MSVC) is recommended.
    *   **Linux/macOS**: `gcc` or `clang`.

## Building the Project

The project uses `cargo` as the build system. The custom `build.rs` script automatically handles compiling the C++ engine code and linking it with the Rust application.

To build the project(in the root directory):

```bash
cargo build
```

## Running the Browser

To run the browser:

```bash
cargo run
```

### Loading Content
By default, the browser attempts to load a file named `verify.html` from the current working directory.
- If `verify.html` exists, it will render its content.
- If not, it falls back to a hardcoded "Hello World" HTML string.

## Project Structure

*   **`src/`**: Contains the Rust source code.
    *   `main.rs`: The entry point. It sets up the window using `winit`, creates a drawing surface with `softbuffer`, and manages the event loop. It calls into the C++ engine to render pixels.
*   **`cpp/`**: Contains the C++ source code for the rendering engine.
    *   `engine.cpp`: The core implementation of the rendering logic (`render_page`).
    *   `engine.hpp`: Header file for the engine (though the Rust interop is primarily defined via `extern "C"` in `main.rs`).
*   **`build.rs`**: A Rust build script that uses the `cc` crate to compile `cpp/engine.cpp` into a static library and link it to the Rust binary.
*   **`Cargo.toml`**: The Rust package manifest, listing dependencies (`winit`, `softbuffer`, `cc`).

## How to Modify

### Modifying the Window/Input (Rust)
Edit `src/main.rs` to change window properties, handling input events, or how the buffer is presented.

### Modifying the Rendering Engine (C++)
Edit `cpp/engine.cpp` to change how HTML is parsed and rendered to pixels.
*   **Note**: The `build.rs` script is configured to watch `cpp/engine.cpp`. Any changes you make to the C++ files will trigger a recompile of the C++ code the next time you run `cargo build` or `cargo run`.