#include <iostream>
#include <cmath>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

const char *color_reset = "\033[0m";
const string colors[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m", "\033[37m"}; // Red, Green, Yellow, Blue, Magenta, Cyan, White

#define height 60
#define width 80

const float pi = 3.14159265358;
const float theta_spacing = 0.07;
const float phi_spacing = 0.02;

const float R1 = 1;
const float R2 = 2;
const float K2 = 5;
const float K1 = 25;

const size_t buffer_size = height * width;

float zBuffer[height][width];
char output[height][width];

// We'll be plotting this on x-y plane
void render_frame(float x_rotat, float z_rotat)
{
    // Calculate sin and cos of angles by which we're going to make rotations on x and z axis.
    float cos_x = cos(x_rotat), sin_x = sin(x_rotat);
    float cos_z = cos(z_rotat), sin_z = sin(z_rotat);

    // Set output to ' ' i.e empty and z_buffer to 0 i.e every point (empty for now) is at infinity
    memset(output, ' ', buffer_size);
    memset(zBuffer, 0, buffer_size * sizeof(float));

    // Increase the theta till 2pi, theta marks main circle using (rad * cos(pi), rad * sin(pi))
    for (float theta = 0; theta < 2 * pi; theta += theta_spacing)
    {
        // Precompute these values for faster calculations and less redundancy
        float cos_theta = cos(theta), sin_theta = sin(theta);

        // Increase the phi till 2pi to make torus, this is for revolution of circle along y axis made using (rad * cos(pi), rad * sin(pi))
        for (float phi = 0; phi < 2 * pi; phi += phi_spacing)
        {
            // Precompute again
            float cos_phi = cos(phi), sin_phi = sin(phi);

            // x and y co-ordinates of circle using (rad * cos(pi), rad * sin(pi))
            float circlex = R2 + R1 * cos_theta; // extra r1 to mark offset of circle from y axis
            float circley = R1 * sin_theta;

            // Final 3d co-ordinates of circle using rotation matrices for rotation along y (phi), x (x_rotat), z (z_rotat) in this frame
            float x = circlex * (cos_z * cos_phi + cos_x * sin_z * sin_phi) - circley * sin_x * sin_z;
            float y = circlex * (sin_z * cos_phi - cos_x * cos_z * sin_phi) + circley * sin_x * cos_z;
            float z = K2 + (sin_x * circlex * sin_phi + circley * cos_x);
            float z_index = 1 / z; // "one over z"

            // x and y projection.  note that y is negated here, because goes up in 3D space but down on 2D displays.
            int xp = static_cast<int>(width / 2 + K1 * z_index * x);
            int yp = static_cast<int>(height / 2 - K1 * z_index * y);

            /*
            Luminance = normal vector of torus X light source i.e (0,1,−1) in our case.
            Normal vector will be same as eqn. of torus but without any magnitude fuckery in first circle.
            Hence we start with is just (cos θ, sin θ, 0) and apply same rotations apply the same rotations
            */

            float luminance = cos_phi * cos_theta * sin_z - sin_x * cos_theta * sin_phi - cos_x * sin_theta + cos_z * (sin_x * sin_theta - cos_theta * cos_x * sin_phi);
            // L ranges from -sqrt(2) to +sqrt(2) because normal vector magnitude is sqrt(2).
            // If it's < 0, the surface is pointing away from us, so we won't bother trying to plot it.
            if (yp >= 0 && yp < height && xp >= 0 && xp < width)
            {
                if (luminance > 0)
                {
                    // test against the z-buffer.  larger 1/z means the pixel is
                    // closer to the viewer than what's already plotted.
                    if (z_index > zBuffer[yp][xp])
                    {
                        zBuffer[yp][xp] = z_index;
                        int luminance_index = luminance * 8;
                        // luminance_index is now in the range 0..11 (8*sqrt(2) = 11.3)
                        // now we lookup the character corresponding to the
                        // luminance and plot it in our output:
                        
                            output[yp][xp] = ".,-~:;=!*#fa"[luminance_index];
                    }
                }
            }
        }
    }
}

int main()
{
    float A = 0.0, B = 0.0;

    // Ask user for their choice of color
    cout << "Choose a color for the animation:\n";
    for (int i = 0; i < sizeof(colors) / sizeof(colors[0]); ++i)
    {
        cout << i + 1 << ". " << colors[i] << "Color " << i + 1 << color_reset << endl;
    }
    int choice;
    cout << "Enter your choice (1-" << sizeof(colors) / sizeof(colors[0]) << "): ";
    cin >> choice;

    // Validate user's choice
    if (choice < 1 || choice > sizeof(colors) / sizeof(colors[0]))
    {
        cerr << "Invalid choice. Exiting...\n";
        return 1;
    }

    cout << colors[choice - 1];

    while (true)
    {
        // Move cursor to the top-left corner of the terminal
        cout << "\033[H";

// hide cursor
#ifdef _WIN32
        HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(consoleHandle, &info);
#else
        cout << "\033[?25l"; // Hide cursor (Linux)
#endif

        render_frame(A, B);

        // Print the output
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                cout << output[j][i];
            }
            cout << "\n";
        }

        // Adjust A and B for animation
        A += 0.1;
        B += 0.08;

#ifdef _WIN32
        // Sleep for a short while to control the animation speed (Windows)
        Sleep(0.3);
#else
        // Sleep for a short while to control the animation speed (Linux)
        usleep(30000);
#endif
    }

    cout << color_reset;

    return 0;
}
