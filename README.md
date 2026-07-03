# Character Viewer — GLTF/GLB Skinned Character Renderer

A real-time 3D character viewer with skeleton skinning, animation playback, and a built-in keyframe editor. Loads animated characters from glTF 2.0 (`.glb`) files and displays them with OpenGL 3.3 core profile.

## Features

- **GLB/GLTF loading** — cgltf-based, reads vertices, normals, UVs, joint indices, weights, inverse bind matrices, and animation channels
- **Skinned rendering** — GPU skinning with up to 80 bones via uniform array, dual-material support with embedded PBR textures (SRGB) 
- **Animation playback** — forward/backward with spacebar toggle, keyframe interpolation
- **Keyframe editor** — F1 toggles editor panel with bone selection, pose manipulation (WASD/QE), keyframe insertion (K), save/load (Ctrl+S/L)
- **Skeleton visualization** — green bone hierarchy lines, red for selected bone
- **Camera** — orbital with right-click drag, WASD for character movement (non-editor mode)

## Controls

| Key | Action |
|-----|--------|
| Space | Play / Pause animation |
| W / S | Animation direction (forward / reverse) |
| F1 | Toggle editor mode |
| 1-9, 0, -, = | Select bone |
| [ ] | Previous / next bone |
| WASD / QE | Rotate bone (editor mode) |
| Shift | Fine rotation |
| Ctrl | Coarse rotation |
| K | Insert keyframe |
| R | Reset selected bone |
| ← → | Step keyframes |
| Ctrl+S / Ctrl+L | Save / Load animation (`.anim`) |

## Build

### Requirements

- Windows 10+
- Visual Studio 2022 with C++17
- CMake 3.16+
- vcpkg with `glfw3` and `glew` installed

### Setup

```powershell
# Install dependencies
vcpkg install glfw3 glew

# Build
cd character
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# Run (from build/Release so DLLs are found)
cd build\Release
character.exe run_speed.glb
```

## Project Structure

```
character/
├── main.cpp          # Complete viewer (~1400 lines)
├── cgltf.h           # glTF 2.0 loader (single header, 205 KB)
├── stb_image.h       # Image decoder for embedded textures
├── stb_truetype.h    # Font renderer for editor UI
├── CMakeLists.txt    # CMake build file
├── run_speed.glb     # Test model (MakeHuman running animation, 7.5 MB)
├── skybox/           # Skybox cubemap textures
└── *.anim            # Saved keyframe animation files
```

## Technical Highlights

- Single-file implementation (~1400 lines) with no external rendering dependencies beyond GLFW/GLEW
- Custom column-vector 4×4 matrix math and quaternion utilities
- Direct buffer access to GLB data (no cgltf accessor wrapper overhead)
- `compute_skin()` computes `bone_world = parent_world * local` (correct for column-vector convention) then `skin = bone_world * inverse_bind_matrix`
- Animation wraps on reverse playback, clamping interpolation to valid range
- `skin->skeleton` fallback: walks parent chain from first joint when skeleton is NULL
