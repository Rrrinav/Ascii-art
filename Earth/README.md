# Ascii Earth Projection

## Overview

This program renders a 3D spherical projection of a 2D rectangular map onto a sphere. It utilizes ASCII characters to represent the map on the sphere's surface, allowing the user to observe the map from different angles.

## Dependencies

- C++ compiler
- Unix-based system (for `usleep()` function and system commands)
- For windows make required changes and use Windows API for C++ i.e. "Windows.h".

## Compilation

To compile and run the program, execute the following commands in your terminal:

```bash
g++ -Wall -std=c++11 -O2 -o main main.cpp -lm -lpthread
./main
```
or

```bash
make main
```

## Mathematics & Geometry

I used simple [spherical coordinate](https://en.wikipedia.org/wiki/Spherical_coordinate_system) system to map a 2D rectangular map to a sphere.



Then I simply rotate it along the desired axis (**z** in this case since spherical coordinates have different conventions).

When required coordinates are found I map them to screen, I used AI in process and quite honestly I am not very sure about some complexities.

After this I just show characters that are going to be visible to eye using z buffering and very simple ray casting. 
