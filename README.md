# Calliterra [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Calliterra is a 3D rendering engine for Windows built with C++ and DirectX (Direct3D 11). Currently it is under active development with the intention for it to act as a foundational engine to support an application to showcase a minecraft style world generation with *"realistic"* graphics. I use this project as a way to learn Direct3D 11, with plans to learn and implement Direct3D 12 in the future.


## Building
Currently I only support building with Visual Studio 2022 using Premake5.

<ins>To Get started:</ins>

1. Clone the repository: `git clone --recursive https://github.com/Yhariin/Calliterra.git`
2. Rune the Premake5.lua script (This requires having premake installed) `Premake5 vs2022`
3. Open `Calliterra.sln` and build the solution in whatever configuration desired (`Debug`, `Release`, `Distribution`)

