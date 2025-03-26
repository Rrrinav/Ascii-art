#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

const int WIDTH         = 80;
const int HEIGHT        = 40;
const int MAX_INTENSITY = 9;

// Map intensity value to an ASCII character (from low to high intensity)
char intensityToChar(int intensity)
{
  const char *palette = " .:-=+*#%@";

  if (intensity < 0) intensity = 0;
  if (intensity > MAX_INTENSITY) intensity = MAX_INTENSITY;

  return palette[intensity];
}

int main()
{
  const double sigma = 10.0;
  const double rho   = 28.0;
  const double beta  = 8.0 / 3.0;
  const double dt    = 0.005;

  // Initial conditions
  double x = 0.1, y = 0.0, z = 0.0;

  // Buffer for intensity values for each screen pixel
  std::vector<int> buffer(WIDTH * HEIGHT, 0);

  // Hide the cursor and clear the screen for a cleaner display
  std::cout << "\x1b[2J" << "\x1b[?25l";

  while (true)
  {
    // Gradually decay the intensity in the buffer (creates a trailing/fading effect)
    for (int i = 0; i < buffer.size(); i++)
      if (buffer[i] > 0) buffer[i]--;

    // Perform several integration steps per frame for a dense attractor plot
    for (int i = 0; i < 100; i++)
    {
      // Compute derivatives using the Lorenz equations
      double dx = sigma * (y - x);
      double dy = x * (rho - z) - y;
      double dz = x * y - beta * z;

      // Euler integration
      x += dx * dt;
      y += dy * dt;
      z += dz * dt;

      // Project the 3D point (x,z) to a 2D screen coordinate.
      // These scaling values are chosen based on typical Lorenz attractor ranges.
      int col = int((x + 20) / 40 * (WIDTH - 1));
      int row = int((z) / 50 * (HEIGHT - 1));

      if (col >= 0 && col < WIDTH && row >= 0 && row < HEIGHT)
      {
        int index = row * WIDTH + col;
        buffer[index] = MAX_INTENSITY;  // Set maximum brightness where the point lands
      }
    }

    // Render the frame by moving the cursor to the top-left
    std::cout << "\x1b[H";
    for (int r = 0; r < HEIGHT; r++)
    {
      for (int c = 0; c < WIDTH; c++) std::cout << intensityToChar(buffer[r * WIDTH + c]);
      std::cout << "\n";
    }

    // Pause briefly to control animation speed
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  // (Unreachable) Restore the cursor if you break the loop
  std::cout << "\x1b[?25h";
  return 0;
}
