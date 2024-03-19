# Game of Life Simulation

This C++ program simulates Conway's Game of Life, a cellular automaton devised by the mathematician John Conway. The simulation takes place on a grid, where each cell can be in one of two states: alive or dead. The state of each cell evolves over time based on a set of rules, creating dynamic patterns.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [How to Use](#how-to-use)
- [Customization](#customization)
- [Implementation Details](#implementation-details)
- [License](#license)

## Introduction

The Game of Life operates on a two-dimensional grid of cells. The program initializes the grid with random alive and dead cells based on a specified randomness threshold. The simulation then progresses through multiple turns, updating the state of each cell according to specific rules.

## Features

- **Grid Visualization:** The program displays the grid with alive and dead cells, using colored output.
- **Dynamic Evolution:** The state of each cell evolves over time based on Conway's Game of Life rules.
- **Customization:** Users can set the grid size, randomness threshold, and observe the simulation for a specified number of turns.

## Prerequisites

To run this program, you need:
- C++ compiler
- Terminal or command prompt

## How to Use

1. **Compile the Code:**
   - Open a terminal or command prompt.
   - Navigate to the directory containing the source code.
   - Compile the code using your preferred C++ compiler.

2. **Run the Program:**
   - Execute the compiled executable.
   - Follow the prompts to enter the grid size and randomness threshold.

3. **Observe the Simulation:**
   - The program will display the evolving state of the grid over multiple turns.
   - The simulation will automatically progress for 100 turns.

## Customization

- **Grid Size:** Set the size of the grid by entering the number of rows and columns.
- **Randomness Threshold:** Adjust the threshold (0-5) to control the initial randomness of alive and dead cells.
- **Simulation Turns:** The default is set to 100 turns, but you can modify the range in the `main` function.

## Implementation Details

- **Grid Class:** The `grid` class provides a flexible implementation of a 2D grid with various constructors for initialization.
- **Update Function:** The `update` function applies the Game of Life rules to evolve the state of each cell based on its neighbors.
- **Visualization:** The `show_grid` function uses colored output to visualize the grid, with alive cells marked with a different color.

## Video Playback

https://github.com/Rrrinav/Ascii-art/assets/152062141/c392e888-9c90-4bcc-b88c-99675edad927

