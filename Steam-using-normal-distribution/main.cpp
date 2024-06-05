#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

#define MAX_PARTICLES 300
#define WIDTH 51
#define HEIGHT 30

// ANSI color escape codes
#define RESET_COLOR "\x1b[0m"
#define BACKGROUND_BLACK "\x1b[40m"
#define BACKGROUND_RESET "\x1b[49m"

class Steam
{
    int particle_count[HEIGHT][WIDTH];

public:
    Steam() { std::fill(&particle_count[0][0], &particle_count[0][0] + WIDTH * HEIGHT, 0); }

    void initialize()
    {
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> dis{WIDTH / 2.0, 7.0};

        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            int x = std::round(dis(gen));
            if (x >= 0 && x < WIDTH)
            {
                particle_count[HEIGHT - 1][x] += 1;
            }
        }
    }

    void update()
    {
        // Move particles upwards and decrement count in each cell
        for (int y = 1; y < HEIGHT; ++y)
        { 
            int random = std::rand() % 2000;

            for (int x = 0; x < WIDTH; ++x)
            {
                int random2 = std::rand() % 1000;
                if (particle_count[y][x] > 0)
                {
                    if (random % 3 == 0 || random2 % 4 == 0)
                        particle_count[y - 1][x] += particle_count[y][x] - 1;
                    else
                        particle_count[y - 1][x] += particle_count[y][x] - 2;
                    particle_count[y][x] = 0;
                }
                else if (particle_count[y][x] == 0)
                {
                    int random3 = std::rand() % 1000;
                    if (random3 % 200 == 0 && y == (HEIGHT - (HEIGHT / 3))) particle_count[y][x] = 1;
                }
            }
        }

        // Clear the bottom and top rows
        for (int x = 0; x < WIDTH; ++x)
        {
            particle_count[HEIGHT - 1][x] = 0;
            particle_count[0][x] = 0;
        }

        // Generate new particles at the bottom using normal distribution
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> dis{WIDTH / 2.0, 6.0};

        for (int i = 0; i < MAX_PARTICLES; i++)
        {
            int x = std::round(dis(gen));
            if (x >= 0 && x < WIDTH)
            {
                particle_count[HEIGHT - 1][x] += 1;
            }
        }
    }

    void print()
    {
        std::vector<std::string> map = {"░", "▒", " ▓", "█"};
        //std::vector<std::string> map = {" ", ".", " {", "S"};
        std::cout << '\n';
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                int count = particle_count[i][j];
                std::string ch = " ";

                if (count > 0)
                {
                    // Map the count to characters
                    if (count <= 3)
                    {
                        ch = map[0];
                    }
                    else if (count <= 6)
                    {
                        ch = map[1];
                    }
                    else if (count <= 9)
                    {
                        ch = map[2];
                    }
                    else
                    {
                        ch = map[3];
                    }
                }
                std::cout << ch;
            }
            std::cout << RESET_COLOR BACKGROUND_RESET << '\n';  // Reset background color
        }
    }
};

int main()
{
    Steam steam;
    steam.initialize();
    int FPS = 10;
    for (;;)
    {
        steam.update();
        steam.print();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));  // Pause for visibility
        std::cout << "\033[2J\033[1;1H";                                     // Clear screen
    }
    return 0;
}
