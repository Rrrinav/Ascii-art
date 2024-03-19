// #include "grid.h"
// #include <chrono>
// #include <cstdlib>
// #include <iostream>
// #include <thread>

// #define ALIVE "\033[1;32m█\033[0m"
// #define DEAD "\033[1;30m█\033[0m"

// // Define a simple range class
// class range {
// public:
//   range(int end) : current(0), end(end) {}

//   int operator*() const { return current; }
//   const range &operator++() {
//     ++current;
//     return *this;
//   }
//   bool operator!=(const range &other) const { return current < other.end; }

// private:
//   int current;
//   int end;
// };

// template <typename T> void show_grid(const grid<T> &grd) {
//   for (size_t row : grd.rows()) {
//     for (size_t col : grd.cols()) {
//       std::cout << (grd.at(row, col) ? ALIVE : DEAD);
//     }
//     std::cout << "\n";
//   }
// }

// void update(grid<char> &Grid) {
//   grid<char> prevGrid(Grid);
//   for (size_t row : Grid.rows()) {
//     for (size_t col : Grid.cols()) {
//       int alive_neighbour = 0;
//       auto neighbour = prevGrid.get_neighbours(row, col);
//       for (auto ind : neighbour) {
//         alive_neighbour += prevGrid.at(ind.row, ind.col);
//       }

//       if (prevGrid.at(row, col)) {
//         if (alive_neighbour < 2 || alive_neighbour > 3) {
//           Grid.at(row, col) = 0;
//         }
//       } else {
//         if (alive_neighbour == 3) {
//           Grid.at(row, col) = 1;
//         }
//       }
//     }
//   }
// }

// void clearScreen() { std::cout << "\033[2J\033[1;1H"; }

// void sleepMilliseconds(int milliseconds) {
//   std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
// }

// int main() {
//   size_t r;
//   size_t c;
//   int randomThreshold;

//   std::cout << "Enter grid size (row x col): ";
//   std::cin >> r >> c;

//   std::cout << "Enter randomness threshold (0-9): ";
//   std::cin >> randomThreshold;

//   grid<char> Grid(r, c, [randomThreshold]() {
//     return (char)(rand() % 10 > randomThreshold);
//   });

//   clearScreen();

//   for (;;) {
//     show_grid(Grid);
//     update(Grid);

//     clearScreen();
//     sleepMilliseconds(80);
//   }

//   return 0;
// }
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <functional>
#include <vector>

constexpr int RANDOM_THRESHOLD_MIN = 0;
constexpr int RANDOM_THRESHOLD_MAX = 9;
constexpr int ALIVE_CELL_THRESHOLD = 3;
constexpr int UNDERPOPULATION_THRESHOLD = 2;
constexpr int OVERPOPULATION_THRESHOLD = 3;
constexpr int SCREEN_CLEAR_DELAY_MS = 80;

// ANSI escape codes for background colors
#define ANSI_COLOR_BLACK "\033[40m"
#define ANSI_COLOR_RED "\033[41m"
#define ANSI_COLOR_GREEN "\033[42m"
#define ANSI_COLOR_YELLOW "\033[43m"
#define ANSI_COLOR_BLUE "\033[44m"
#define ANSI_COLOR_MAGENTA "\033[45m"
#define ANSI_COLOR_CYAN "\033[46m"
#define ANSI_COLOR_WHITE "\033[47m"
#define ANSI_COLOR_RESET "\033[0m"

// Define a simple range class
class Range
{
public:
  Range(int end) : current(0), end(end) {}

  int operator*() const { return current; }
  const Range &operator++()
  {
    ++current;
    return *this;
  }
  bool operator!=(const Range &other) const { return current < other.end; }

private:
  int current;
  int end;
};

// Function to create a range of integers
std::vector<size_t> range(size_t end)
{
  std::vector<size_t> res(end);
  for (auto i = 0u; i < end; ++i)
  {
    res.at(i) = i;
  }
  return res;
}

// Function to display the grid
template <typename T>
void displayGrid(const std::vector<std::vector<T>> &grid, const std::string &aliveColor)
{
  for (const auto &row : grid)
  {
    for (const auto &cell : row)
    {
      std::cout << (cell ? aliveColor + " " : ANSI_COLOR_BLACK " ");
    }
    std::cout << ANSI_COLOR_RESET "\n"; // Reset color at the end of each row
  }
}

// Function to update the grid based on Conway's Game of Life rules
void updateGrid(std::vector<std::vector<char>> &grid)
{
  auto prevGrid = grid;
  const size_t numRows = grid.size();
  const size_t numCols = grid[0].size();

  for (size_t row = 0; row < numRows; ++row)
  {
    for (size_t col = 0; col < numCols; ++col)
    {
      int aliveNeighbors = 0;

      for (int i = -1; i <= 1; ++i)
      {
        for (int j = -1; j <= 1; ++j)
        {
          if (i == 0 && j == 0)
            continue;

          int neighborRow = row + i;
          int neighborCol = col + j;

          if (neighborRow >= 0 && neighborRow < numRows && neighborCol >= 0 && neighborCol < numCols)
          {
            aliveNeighbors += prevGrid[neighborRow][neighborCol] ? 1 : 0;
          }
        }
      }

      if (prevGrid[row][col])
      {
        if (aliveNeighbors < UNDERPOPULATION_THRESHOLD || aliveNeighbors > OVERPOPULATION_THRESHOLD)
        {
          grid[row][col] = 0;
        }
      }
      else
      {
        if (aliveNeighbors == ALIVE_CELL_THRESHOLD)
        {
          grid[row][col] = 1;
        }
      }
    }
  }
}

// Function to clear the console screen
void clearScreen()
{
  std::cout << "\033[2J\033[1;1H"; // Clear screen
}

// Function to sleep for specified milliseconds
void sleepMilliseconds(int milliseconds)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main()
{
  size_t numRows, numCols;
  int randomThreshold;
  std::string aliveColor;

  std::cout << "Enter grid size (row x col): ";
  std::cin >> numRows >> numCols;

  std::cout << "Enter randomness threshold (0-9): ";
  std::cin >> randomThreshold;

  if (randomThreshold < RANDOM_THRESHOLD_MIN || randomThreshold > RANDOM_THRESHOLD_MAX)
  {
    std::cerr << "Randomness threshold must be between 0 and 9." << std::endl;
    return 1;
  }

  aliveColor = ANSI_COLOR_BLUE;
  // Initialize the grid with random values based on the threshold
  std::vector<std::vector<char>> grid(numRows, std::vector<char>(numCols));
  for (size_t row = 0; row < numRows; ++row)
  {
    for (size_t col = 0; col < numCols; ++col)
    {
      grid[row][col] = rand() % 10 > randomThreshold ? 1 : 0;
    }
  }

  clearScreen();

  for (;;)
  {
    displayGrid(grid, aliveColor);
    updateGrid(grid);

    clearScreen();
    sleepMilliseconds(SCREEN_CLEAR_DELAY_MS);
  }
  std::cout << ANSI_COLOR_RESET;
  return 0;
}
