# Introduction

The *Visuals Programming Language* (VPL) is a typed, three-address, mutable-register language.
Text reading binds canonical modules directly, the shared verifier checks them, and `VerifiedModule`
publishes immutable compiler input. The GLSL 420 core backend returns code and typed reflection.

Built-in material descriptions live in `VPL/Library/Assets/Programs/*.vpl`. CMake embeds their exact
bytes into a private header in the external build tree; `Programs_create` selects a description,
reads/verifies it, and publishes it. Runtime working directories and source-tree files are not used.

There is no expression AST/symbol frontend or AST semantic pipeline in this module. Any future
higher-level language is a separate module targeting canonical VPL.

# References
- [OpenGL Shading Language Version 4.60.8](https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.60.pdf)

# See Also
- [DESIGN.md](DESIGN.md) - compiler IR design and migration plan.
