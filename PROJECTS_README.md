# Raylib Multi-Project Workspace

This workspace supports multiple raylib projects sharing the same raylib build system.

## Project Structure

```
rlNoiseGrid/
├── external/           # Shared dependencies (raylib, raygui)
├── projects/          # Individual projects
│   ├── NoiseGrid/     # Your original NoiseGrid project
│   │   ├── src/       # Source files
│   │   ├── inc/       # Header files
│   │   └── CMakeLists.txt
│   └── NewProject/    # New project template
│       ├── src/       # Source files
│       ├── inc/       # Header files
│       └── CMakeLists.txt
├── CMakeLists.txt     # Main workspace CMake (builds all projects)
├── CMakeLists_new.txt # New multi-project CMake
├── build_all.sh       # Build all projects script
├── switch_project.sh  # Switch between projects script
└── src/               # Legacy location (preserved)
```

## Usage

### Option 1: Build All Projects
```bash
# Use the new multi-project CMake
cp CMakeLists_new.txt CMakeLists.txt
./build_all.sh
```

### Option 2: Switch Between Projects
```bash
# Switch to NoiseGrid
./switch_project.sh NoiseGrid
cmake -B build && cmake --build build

# Switch to NewProject  
./switch_project.sh NewProject
cmake -B build && cmake --build build
```

### Option 3: Build Specific Projects
```bash
# Build only specific projects
cmake -DBUILD_NOISEGRID=ON -DBUILD_NEWPROJECT=OFF -B build
cmake --build build
```

## Adding New Projects

1. Create directory: `projects/YourProject/`
2. Add `src/` and `inc/` subdirectories
3. Create `CMakeLists.txt` in project directory
4. Add project to main `CMakeLists_new.txt`

## Benefits

- ✅ Original NoiseGrid remains fully buildable
- ✅ Shared raylib build system
- ✅ Independent project development
- ✅ Easy project switching
- ✅ Scalable to many projects
- ✅ Preserves all existing code

## Migration Notes

- Original `src/` directory preserved for backup
- All existing functionality maintained
- Build outputs go to `build/bin/`
- Each project has its own executable name
