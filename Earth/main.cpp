#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <unistd.h> // For usleep on Unix-based systems

#define HEIGHT 40
#define WIDTH 160
#define FILENAME "image.txt"
#define RADIUS 30 // Radius of the sphere
#define RES_FACTOR_HEIGHT 1
#define RES_FACTOR_WIDTH 2
#define FPS 20

inline float dot_product(const std::vector<float>& v31, const std::vector<float>& v32)
{
    return v31[0] * v32[0] + v31[1] * v32[1] + v31[2] * v32[2];
}

class Map
{
public:
    size_t _height;
    size_t _width;
    std::string _file_name;
    std::vector<char> _map_pixels;

public:
    Map(size_t height, size_t width, std::string filename)
        : _height(height), _width(width), _file_name(filename),
          _map_pixels(height * width, ' ') {}

    std::vector<char> read_map()
    {
        std::ifstream file(_file_name);
        if (!file.is_open())
        {
            std::cerr << "ERROR: Unable to open file " << _file_name << std::endl;
            throw std::runtime_error("Unable to open file");
        }

        std::string line;
        size_t idx = 0;
        while (std::getline(file, line) && idx < _map_pixels.size())
        {
            for (char c : line)
            {
                _map_pixels[idx++] = c;
            }
        }

        file.close();
        return _map_pixels;
    }

    void print_map()
    {
        for (size_t i = 0; i < _height; i++)
        {
            for (size_t j = 0; j < _width; j++)
            {
                std::cout << _map_pixels[i * _width + j];
            }
            std::cout << std::endl;
        }
    }
};

class Renderer
{
private:
    std::vector<float> source = {0, 100, 0};
    size_t radius;
    size_t width;
    size_t height;
    size_t sphereWidth;
    size_t sphereHeight;

public:
    Renderer(size_t r, size_t w, size_t h)
        : radius(r), width(w), height(h), sphereWidth(2 * w - 1), sphereHeight(h) {}

    void map_to_sphere(const Map& map, float angle)
    {
        std::vector<std::vector<char>> sphere(sphereHeight, std::vector<char>(sphereWidth, ' '));
        std::vector<std::vector<float>> z_buffer(sphereHeight, std::vector<float>(sphereWidth, -100.0f)); // Initialize with minimum float value

        float cos_angle = cos(angle);
        float sin_angle = sin(angle);


        for (size_t i = 0; i < map._height; i++)
        {
            float lat = (M_PI / map._height) * i - M_PI / 2;
            for (size_t j = 0; j < map._width; j++)
            {
                float lon = (2 * M_PI / map._width) * j - M_PI;
                float x = radius * cos(lat) * sin(lon);
                float z = radius * sin(lat);
                float y = radius * cos(lat) * cos(lon);

                // Rotate around z-axis
                float new_x = x * cos_angle - y * sin_angle;
                float new_y = x * sin_angle + y * cos_angle;
                float new_z = z;

                std::vector<float> pos = {new_x, new_y, new_z};

                float l = dot_product(pos, source);

                int screen_x = static_cast<int>(new_x + width / 2 + radius);
                int screen_y = static_cast<int>((new_z + radius) * height / (2 * radius));

                if (screen_x >= 0 && screen_x < sphereWidth && screen_y >= 0 && screen_y < sphereHeight && l > 0)
                {
                    if (new_x >= z_buffer[screen_y][screen_x])
                    {
                        sphere[screen_y][screen_x] = map._map_pixels[i * map._width + j];
                        z_buffer[screen_y][screen_x] = new_x;
                    }
                }
            }
        }

        // Hide cursor
        std::cout << "\033[?25l";

        // Print the sphere with the modified print_map function
        print_map(sphere);

        // Show cursor
        std::cout << "\033[?25h";
    }

    void print_map(const std::vector<std::vector<char>>& sphere)
    {
        for (size_t y = 0; y < sphereHeight; ++y)
        {
            for (size_t x = 0; x < sphereWidth; ++x)
            {
                if (sphere[y][x] == '+')
                {
                    std::cout << "\033[32m" << '+' << "\033[0m"; // Green color
                }
                else if (sphere[y][x] == '.')
                {
                    std::cout << "\033[34m" << '.' << "\033[0m"; // Blue color
                }
                else
                {
                    std::cout << sphere[y][x];
                }
            }
            std::cout << "\n";
        }
    }
};

int main()
{
    Map map(HEIGHT, WIDTH, FILENAME);
    map.read_map();
    map.print_map();

    Renderer renderer(RADIUS, RADIUS * RES_FACTOR_WIDTH, RADIUS * RES_FACTOR_HEIGHT);

    float angle = 0.0f;
    float delta_angle = 0.01f; // Adjust this value to control rotation speed

    while (true)
    {
        system("clear"); // Clear the console (Unix-based systems)

        renderer.map_to_sphere(map, angle);

        angle -= delta_angle;

        usleep(100000 / FPS); // Delay for animation (Unix-based systems)
    }

    return 0;
}
