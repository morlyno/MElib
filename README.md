# MElib

**MElib** is a collection of C++20 utilities created during the development of **Shark**.  
It provides useful utilities for memory management, reference counting, assertions and more.

# Main Features
### Core
- [`Core`](https://github.com/morlyno/MElib/blob/main/Source/MElib/Core/Core.h) — central header included first in every file.
- [`Intrusive Shared Pointer`](https://github.com/morlyno/MElib/blob/main/Source/MElib/Core/RefCounted.h) — reference-counted smart pointer.
- [`Scoped Pointer`](https://github.com/morlyno/MElib/blob/main/Source/MElib/Core/Scoped.h) — RAII-based unique ownership pointer.
- Assertion and verification macros.
- Implementation of the FNV-1a hash algorithm

### Memory
- [`Allocator`](https://github.com/morlyno/MElib/blob/main/Source/MElib/Memory/Allocator.h) wraps `malloc`/`free`. This will be extended to allow memory tracking by optionally overriding the `new` and `delete` operators.
- [`Buffer`](https://github.com/morlyno/MElib/blob/main/Source/MElib/Memory/Buffer.h#L12) a wrapper around memory with size tracking and utility functions like `Allocate`, `Resize` and `Write`.
- [`OwningBuffer`](https://github.com/morlyno/MElib/blob/main/Source/MElib/Memory/Buffer.h#L48) like `Buffer` but automatically frees memory.
- [`BufferArg`](https://github.com/morlyno/MElib/blob/main/Source/MElib/Memory/Buffer.h#L81) safely pass a Buffer to other functions, either by owning the memory or providing a view.

# Getting Started

### 1. Using [Premake](https://premake.github.io/)
Simply include the [premake5.lua](https://github.com/morlyno/MElib/blob/main/premake5.lua) file (located at the root of this repository) into your own Premake project.  
> **Note:** The build output is configured to go into the `bin/` directory inside your workspace (`%{wks.location}/bin/...`).

### 2. Without Premake
If you're not using Premake, you can integrate MElib into your build system manually:
- Set your C++ standard to **C++20**.
- Add the `Source/` directory to your include paths, since library includes are relative to this folder.

## Additional Configuration
Some features can be enabled or customized by defining specific macros (e.g. `ENABLE_SAFE_BUFFER_ACCESS`).  
You can enable these features by:
- Uncommenting the corresponding definitions in the [`Core`](https://github.com/morlyno/MElib/blob/main/Source/MElib/Core/Core.h#L94) header.
- Or defining them through your build system.
