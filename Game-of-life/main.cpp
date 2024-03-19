#include "grid.h"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#define ALIVE "\033[1;32m█\033[0m"
#define DEAD "\033[1;30m█\033[0m"

// Define a simple range class
class range {
public:
  range(int end) : current(0), end(end) {}

  int operator*() const { return current; }
  const range &operator++() {
    ++current;
    return *this;
  }
  bool operator!=(const range &other) const { return current < other.end; }

private:
  int current;
  int end;
};

template <typename T> void show_grid(const grid<T> &grd) {
  for (size_t row : grd.rows()) {
    for (size_t col : grd.cols()) {
      std::cout << (grd.at(row, col) ? ALIVE : DEAD);
    }
    std::cout << "\n";
  }
}

void update(grid<char> &Grid) {
  grid<char> prevGrid(Grid);
  for (size_t row : Grid.rows()) {
    for (size_t col : Grid.cols()) {
      int alive_neighbour = 0;
      auto neighbour = prevGrid.get_neighbours(row, col);
      for (auto ind : neighbour) {
        alive_neighbour += prevGrid.at(ind.row, ind.col);
      }

      if (prevGrid.at(row, col)) {
        if (alive_neighbour < 2 || alive_neighbour > 3) {
          Grid.at(row, col) = 0;
        }
      } else {
        if (alive_neighbour == 3) {
          Grid.at(row, col) = 1;
        }
      }
    }
  }
}

void clearScreen() { std::cout << "\033[2J\033[1;1H"; }

void sleepMilliseconds(int milliseconds) {
  std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main() {
  size_t r;
  size_t c;
  int randomThreshold;

  std::cout << "Enter grid size (row x col): ";
  std::cin >> r >> c;

  std::cout << "Enter randomness threshold (0-9): ";
  std::cin >> randomThreshold;

  grid<char> Grid(r, c, [randomThreshold]() {
    return (char)(rand() % 10 > randomThreshold);
  });

  clearScreen();

  for (;;) {
    show_grid(Grid);
    update(Grid);

    clearScreen();
    sleepMilliseconds(80);
  }

  return 0;
}
