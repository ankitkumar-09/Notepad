# Notepad

A simple yet functional text editor application built with Qt and C++. This notepad application provides essential text editing features with a clean and intuitive graphical user interface.

## Description

This project is a lightweight notepad application developed using the Qt framework and C++. It offers basic text editing capabilities including creating, opening, saving, and editing text files. The application demonstrates the implementation of Qt Widgets, file I/O operations, and GUI development principles.

### Key Features

- Create new text documents
- Open existing text files
- Save and Save As functionality
- Text editing with standard cut, copy, paste operations
- Cross-platform compatibility (Windows, macOS, Linux)
- Clean and user-friendly interface

## Prerequisites

Before building and running this project, ensure you have the following installed:

- **Qt Framework** (Qt 5 or Qt 6)
  - Qt Widgets module
  - Qt Core module
  - Qt GUI module
- **C++ Compiler**
  - GCC (Linux/macOS)
  - MinGW or MSVC (Windows)
  - Clang (macOS/Linux)
- **Build Tools**
  - CMake (version 3.10 or higher) or
  - Make
  - qmake (included with Qt)

### Installing Qt

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools
```

**macOS (using Homebrew):**
```bash
brew install qt
```

**Windows:**
Download and install Qt from the [official Qt website](https://www.qt.io/download)

## Getting Started

### Cloning the Repository

Clone this repository to your local machine:

```bash
git clone https://github.com/ankitkumar-09/Notepad.git
cd Notepad
```

## Building the Project

The project supports multiple build systems. Choose the method that works best for your setup:

### Method 1: Using Qt Creator (Recommended)

1. Open Qt Creator
2. Click **File > Open File or Project**
3. Navigate to the cloned repository and select one of the following:
   - `CMakeLists.txt` (if using CMake)
   - `*.pro` file (if using qmake)
4. Configure the project with your Qt kit
5. Click the **Build** button (hammer icon) or press **Ctrl+B** (Cmd+B on macOS)
6. Run the application by clicking the **Run** button (play icon) or press **Ctrl+R** (Cmd+R on macOS)

### Method 2: Using CMake (Command Line)

```bash
# Create a build directory
mkdir build
cd build

# Configure the project with CMake
cmake ..

# Build the project
cmake --build .

# Alternatively, use make directly
make

# Run the application
./Notepad
```

### Method 3: Using qmake (Command Line)

```bash
# Generate Makefile from .pro file
qmake

# Build the project
make

# Run the application
./Notepad
```

### Method 4: Using Makefile Directly

If a Makefile is provided in the repository:

```bash
# Build the project
make

# Run the application
./Notepad

# Clean build files
make clean
```

## Project Structure

```
Notepad/
├── src/                    # Source files (.cpp)
├── include/                # Header files (.h)
├── ui/                     # Qt UI files (.ui)
├── resources/              # Resource files (icons, images)
├── CMakeLists.txt          # CMake build configuration
├── Makefile                # Makefile for building
├── *.pro                   # Qt project file (qmake)
└── README.md               # This file
```

## Usage

Once the application is running:

1. **Create a New File**: Click `File > New` or press `Ctrl+N`
2. **Open a File**: Click `File > Open` or press `Ctrl+O`
3. **Save a File**: Click `File > Save` or press `Ctrl+S`
4. **Save As**: Click `File > Save As` to save with a new name
5. **Edit Text**: Use standard text editing operations (cut, copy, paste, undo, redo)
6. **Exit**: Click `File > Exit` or press `Ctrl+Q`

## Platform-Specific Notes

### macOS
- If you encounter Qt library issues, ensure Qt is properly installed via Homebrew or Qt installer
- You may need to set Qt paths in your environment:
  ```bash
  export PATH="/usr/local/opt/qt/bin:$PATH"
  export LDFLAGS="-L/usr/local/opt/qt/lib"
  export CPPFLAGS="-I/usr/local/opt/qt/include"
  ```

### Linux
- Ensure Qt development packages are installed
- On some distributions, you may need to install additional dependencies:
  ```bash
  sudo apt-get install build-essential libgl1-mesa-dev
  ```

### Windows
- Use Qt Creator with MinGW or MSVC compiler
- Ensure Qt bin directory is in your system PATH

## Troubleshooting

**Problem**: CMake cannot find Qt
- **Solution**: Set `CMAKE_PREFIX_PATH` to your Qt installation directory:
  ```bash
  cmake -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x.x/gcc_64 ..
  ```

**Problem**: Makefile not found
- **Solution**: Generate the Makefile first using qmake or CMake

**Problem**: Application won't run - missing Qt libraries
- **Solution**: Ensure Qt libraries are in your system's library path or use Qt's deployment tools:
  - Linux: `ldd ./Notepad` to check dependencies
  - macOS: `otool -L ./Notepad`
  - Windows: Use `windeployqt.exe`

## Contributing

Contributions are welcome! If you'd like to improve this project:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is available under the MIT License. See the LICENSE file for more details.

## Acknowledgments

- Built with the [Qt Framework](https://www.qt.io/)
- Inspired by classic text editor applications

## Author

**Ankit Kumar**
- GitHub: [@ankitkumar-09](https://github.com/ankitkumar-09)

## Support

If you encounter any issues or have questions, please open an issue on the [GitHub repository](https://github.com/ankitkumar-09/Notepad/issues).

---

**Note**: This is a learning project demonstrating Qt GUI development and C++ programming concepts. Feel free to use it as a reference for your own Qt applications.