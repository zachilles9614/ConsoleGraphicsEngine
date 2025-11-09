#pragma once

#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct vec3d
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct triangle
{
    std::array<vec3d, 3> p{};

    wchar_t sym = L' ';
    short col = 0;
};

struct mesh
{
    std::vector<triangle> tris;

    bool LoadFromObjectFile(const std::string &sFilename)
    {
        std::ifstream f(sFilename);
        if (!f.is_open())
            return false;

        std::vector<vec3d> verts;

        std::string line;
        while (std::getline(f, line))
        {
            if (line.empty())
                continue;

            std::stringstream s(line);
            char temp;

            if (line[0] == 'v')
            {
                vec3d v{};
                s >> temp >> v.x >> v.y >> v.z;
                verts.push_back(v);
            }
            else if (line[0] == 'f')
            {
                int f[3];
                s >> temp >> f[0] >> f[1] >> f[2];

                triangle tri;
                tri.p = { verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] };
                tris.push_back(tri);
            }
        }
        return true;
    }
};
