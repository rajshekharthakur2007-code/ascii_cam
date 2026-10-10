# ascii-cam

Real-time webcam to ASCII art renderer in C++ using OpenCV. The camera frame
is split into pixel tiles, and each tile is mapped to a character based on its
average saturation (plus a small hue weight so that solid colors stay
distinguishable).

## Requirements
- C++17 compiler, CMake 3.10+
- OpenCV 4.x
- A webcam
- A terminal that supports ANSI escape codes, at least 80x31 for a 640x480 camera

On Debian/Ubuntu/Mint:
    sudo apt install build-essential cmake libopencv-dev

## Build and run
    mkdir build && cd build
    cmake .. && cmake --build .
    ./ascii_cam

Press `q` or Esc in the OpenCV window to quit.

## How it works
Each 9x16 pixel tile becomes one character. The tile's average of
`S + 0.05*H` (HSV) picks a character from a 5-level ramp: ` `, `*`, `x`, `#`, `M`.
Saturation is used instead of brightness because it brings out detail in
shadows and folds on white or gray objects, and the hue term separates
different solid colors.

## Notes
- Thresholds were tuned on one webcam and room. Different cameras or
  lighting may look different.
- On Windows, use Windows Terminal (older consoles need virtual terminal mode).

## Roadmap
- [ ] Adjustable aspect ratio / fixed character grid
- [ ] More character layers for finer texture
- [ ] Selectable source channel