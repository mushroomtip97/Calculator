# Calculator

A Windows-style desktop calculator written in C++ and Qt 6 Widgets.

Started as a learning project. Current version has five calculator modes,
keyboard input, a history panel, mode icons, and nine visual themes.

---

## Features

### Standard
- Basic arithmetic: `+`, `-`, `*`, `/`
- Percentage, square, square root, reciprocal, sign change
- Backspace, clear entry, clear all
- Keyboard input
- Memory operations: MC, MR, M+, M-, MS

### Scientific
- `sin`, `cos`, `tan`
- `log`, `ln`
- Powers, factorial, absolute value
- `pi`, `e`, `10^x`
- Memory operations

### Programmer
- Bases: HEX, DEC, OCT, BIN
- Bitwise operations: AND, OR, XOR, NOT, NAND, NOR, shifts
- Word sizes: QWORD, DWORD, WORD, BYTE
- Live value representation in all bases

### Graphing
- Multiple equations with automatic colors
- Equation list: add, remove, clear, recolor
- Expression translator with implicit multiplication (`2x`)
- Drag-to-pan
- Mouse-wheel zoom that follows the cursor
- Mouse coordinate readout
- Zoom buttons with reset

### Date calculation
- Difference between two dates
- Add or subtract years, months, and days

### Application
- History list
- Mode icons that are tinted for dark themes
- Window and executable icons
- Themes: Light, Dark, Blue, High contrast, Silver, Gold,
  Ink light, Ink blue, Ink pure

---

## Building

Requirements:

- Qt 6.5 or newer: `Core`, `Widgets`, `Qml`, `Test`
- CMake 3.19 or newer
- C++17 compiler

```bash
cmake -S . -B build -G Ninja -DCMAKE_PREFIX_PATH=/path/to/Qt
cmake --build build
```

Run tests:

```bash
ctest --test-dir build --output-on-failure
```

In Qt Creator, open `CMakeLists.txt`, select a Qt 6 kit, then build and run
the `Calculator` target.

---

## Project structure

```text
Calculator-Project/
├─ CMakeLists.txt
├─ README.md
├─ resources/
│  ├─ app.ico
│  ├─ app.qrc
│  ├─ app.rc
│  └─ icons/
├─ tests/
│  ├─ tst_clcengine.cpp
│  └─ tst_programmerengine.cpp
└─ src/
   ├─ main.cpp
   ├─ app/
   │  ├─ mainwindow.*
   │  ├─ Themes.h
   │  ├─ Icons.h
   │  ├─ TintColor.h
   │  └─ UiHelper.h
   ├─ core/
   │  ├─ clcengine.*
   │  ├─ programmerengine.*
   │  └─ StringUtils.h
   ├─ modes/
   │  ├─ standardcalculator.*
   │  ├─ scientificcalculator.*
   │  ├─ programmercalculator.*
   │  └─ datecalculator.*
   └─ graph/
      ├─ graphscene.*
      └─ graphingcalculator.*
```

| Folder | Purpose |
|---|---|
| `src/app` | Main window, modes, themes, icons |
| `src/core` | Calculation engines and shared helpers |
| `src/modes` | One widget per calculator mode |
| `src/graph` | Graph scene and graphing page |
| `tests` | Engine unit tests |
| `resources` | Icons and Windows resources |

---

## Controls

| Input | Action |
|---|---|
| `0`-`9`, `.` | Digits |
| `+`, `-`, `*`, `/` | Operators |
| `Enter` / `=` | Calculate |
| `Backspace` | Delete last digit |
| `Esc` / `Delete` | Clear |
| Mouse drag on graph | Pan |
| Mouse wheel on graph | Zoom around cursor |
| Double-click on graph | Recenter |

---

## TODO

### Architecture

- [ ] Reduce remaining duplication between calculator pages
- [ ] Move memory-button logic into a shared base class
- [ ] Create a shared display-update helper
- [ ] Move `Themes.h` data into a `ThemeManager` class

### Features

- [ ] Save selected theme with `QSettings`
- [ ] Persist history between sessions
- [ ] Edit an existing graph equation
- [ ] Add copy/paste result menu
- [ ] Support more math functions in the graph translator

### Quality

- [ ] Add tests for date calculations
- [ ] Test the graph expression translator
- [ ] Cache the compiled JavaScript function per graph equation
- [ ] Run a static analyzer
- [ ] Build and test in Release mode

---

## Comments

Comments explain **why**, not **what**. Planned work uses `// TODO:`,
known bugs use `// FIXME:`.

---

## Icon credits

Mode and application icons are from [Icons8](https://icons8.com).

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).

