#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <locale>
#include <algorithm>
#include <set>
#include <thread>
#include <chrono>

using namespace std;

class Screen {
private:
    int w = 190;
    int h = 48;

    wstring screen_base;

public:
    Screen(wstring b) : screen_base(b) {}

    wstring Draw_pixel(int x, int y, wstring before) {
        if (x < 1 || x > w || y < 1 || y > h) return before;

        int pixel = w * (y - 1) + (x - 1);
        if (before[pixel] == L'█') {
            before[pixel] = L' ';
        } else if (before[pixel] == L' ') {
            before[pixel] = L'█';
        }

        return before;
    };

    wstring Fill_screen() {
        wstring new_screen;
        new_screen.reserve(w * h);

        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                new_screen += L'█';
            }
        }
        return new_screen;
    }

    vector<pair<int, int>> project_cube(float cx, float cy, float cz, float size, float focal_length, float angle) {
        vector<pair<int, int>> cube_pixels;
        set<pair<int, int>> unique_pixels;

        vector<vector<float>> local_vertices = {
            {-size/2, -size/2, -size/2},
            { size/2, -size/2, -size/2},
            { size/2,  size/2, -size/2},
            {-size/2,  size/2, -size/2},
            {-size/2, -size/2,  size/2},
            { size/2, -size/2,  size/2},
            { size/2,  size/2,  size/2},
            {-size/2,  size/2,  size/2}
        };

        float cos_angle = cos(angle);
        float sin_angle = sin(angle);
        float one_minus_cos = 1.0f - cos_angle;

        float r11 = cos_angle + one_minus_cos/3.0f;
        float r12 = one_minus_cos/3.0f - sqrt(1.0f/3.0f)*sin_angle;
        float r13 = one_minus_cos/3.0f + sqrt(1.0f/3.0f)*sin_angle;

        float r21 = one_minus_cos/3.0f + sqrt(1.0f/3.0f)*sin_angle;
        float r22 = cos_angle + one_minus_cos/3.0f;
        float r23 = one_minus_cos/3.0f - sqrt(1.0f/3.0f)*sin_angle;

        float r31 = one_minus_cos/3.0f - sqrt(1.0f/3.0f)*sin_angle;
        float r32 = one_minus_cos/3.0f + sqrt(1.0f/3.0f)*sin_angle;
        float r33 = cos_angle + one_minus_cos/3.0f;

        vector<vector<float>> vertices;
        for (int i = 0; i < local_vertices.size(); i++) {
            float x = local_vertices[i][0];
            float y = local_vertices[i][1];
            float z = local_vertices[i][2];
            float x_rot = r11*x + r12*y + r13*z;
            float y_rot = r21*x + r22*y + r23*z;
            float z_rot = r31*x + r32*y + r33*z;
            vertices.push_back({x_rot + cx, y_rot + cy, z_rot + cz});
        }

        // Рёбра куба
        vector<pair<int, int>> edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };

        vector<pair<int, int>> projected_vertices;
        for (int i = 0; i < vertices.size(); i++) {
            float x = vertices[i][0];
            float y = vertices[i][1];
            float z = vertices[i][2];

            float projection = focal_length / (z + focal_length);
            int proj_x = static_cast<int>(x * projection) + w/2;
            int proj_y = h/2 - static_cast<int>(y * projection);

            projected_vertices.push_back({proj_x, proj_y});
        }

        for (int i = 0; i < edges.size(); i++) {
            int x1 = projected_vertices[edges[i].first].first;
            int y1 = projected_vertices[edges[i].first].second;
            int x2 = projected_vertices[edges[i].second].first;
            int y2 = projected_vertices[edges[i].second].second;

            int dx = abs(x2 - x1);
            int dy = abs(y2 - y1);
            int sx = (x1 < x2) ? 1 : -1;
            int sy = (y1 < y2) ? 1 : -1;
            int err = dx - dy;

            while (true) {
                if (x1 >= 1 && x1 <= w && y1 >= 1 && y1 <= h) {
                    unique_pixels.insert({x1, y1});
                }

                if (x1 == x2 && y1 == y2) break;

                int e2 = 2 * err;
                if (e2 > -dy) {
                    err -= dy;
                    x1 += sx;
                }
                if (e2 < dx) {
                    err += dx;
                    y1 += sy;
                }
            }
        }

        cube_pixels.assign(unique_pixels.begin(), unique_pixels.end());
        return cube_pixels;
    }
};

int main() {
    locale::global(locale(""));
    wcout.imbue(locale());

    Screen screen(L"");

    float angle = 0.5f;
    while (true) {
        wstring filled = screen.Fill_screen();

        vector<pair<int, int>> cube_coords = screen.project_cube(0, 5, 20, 40, 100.0f, angle);

        wstring current_screen = filled;
        for (int i = 0; i < cube_coords.size(); i++) {
            current_screen = screen.Draw_pixel(cube_coords[i].first, cube_coords[i].second, current_screen);
        }

        wcout << L"\033[2J\033[1;1H";
        wcout << current_screen;

        angle += 0.05f;
        if (angle > 2*3.14159f) angle = 0;

        this_thread::sleep_for(chrono::milliseconds(30));
    }

    return 0;
}
