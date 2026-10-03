# Bullet-Hell-C-WIP-
Simple Bullet hell made on C by using the raylib library.
## 🎮 Controls
WASD - move
LeftShift - move slower
## 🔨 Build
raylib 6.0 is downloaded and linked statically by CMake, so the resulting binary
runs without installing raylib (or any other library) on the target machine.

Requirements to **build** (not to play): CMake ≥ 3.25 and a C compiler.

- **Windows**: Visual Studio / Build Tools, or MinGW-w64 (e.g. WinLibs).
- **Linux** (Debian/Ubuntu): `sudo apt install build-essential cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev`

```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The executable ends up in `build/` (`build/Release/` with Visual Studio).
The GitHub Actions workflow also builds both binaries on every push and uploads them as artifacts.
## Screenshot
<img width="1279" height="919" alt="screenshot-2026-09-30_12-36-49" src="https://github.com/user-attachments/assets/e590cea2-50f1-488a-b749-e9d0ac636a3c" />
