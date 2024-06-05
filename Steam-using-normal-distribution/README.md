# Steam Simulation

This program simulates the generation of steam using ASCII characters. It creates a dynamic steam effect where particles rise upwards, resembling steam rising from a source.

## Features

- Simulates the movement and behavior of steam particles.
- Dynamic generation and movement of particles using random distribution.
- Customizable appearance with varying levels of intensity represented by different ASCII characters.
- Colorful visualization using ANSI escape codes for better visual effect.

## How to Use

1. Compile the program using a C++ compiler. For example, using g++:
    ```bash
    g++ -o steam_simulation steam_simulation.cpp
    ```

2. Run the compiled executable:
    ```bash
    ./steam_simulation
    ```

3. Watch the steam simulation in action. The particles will rise and move dynamically, creating a steam-like effect on the screen.

## Customization

- You can adjust the number of particles (`MAX_PARTICLES`) and the dimensions of the simulation grid (`WIDTH` and `HEIGHT`) to customize the appearance and performance of the simulation.

- You can also modify the mapping of particle intensities to ASCII characters by changing the `map` vector in the `Steam::print()` method. Experiment with different characters to achieve the desired visual effect.

## Dependencies

- This program requires a C++ compiler that supports C++11 or later standards.
- It uses ANSI escape codes for color formatting, which should be supported by most terminal emulators.
