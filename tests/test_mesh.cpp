#include "mesh.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace
{
void Expect(bool condition, const std::string &message)
{
    if (!condition)
        throw std::runtime_error(message);
}

void TestLoadSimple()
{
    const auto temp_file = std::filesystem::temp_directory_path() / "mesh_test_simple.obj";
    std::ofstream out(temp_file);
    out << "v 0 0 0\n";
    out << "v 0 1 0\n";
    out << "v 1 0 0\n";
    out << "f 1 2 3\n";
    out.close();

    mesh m;
    Expect(m.LoadFromObjectFile(temp_file.string()), "LoadFromObjectFile should succeed for valid file");
    Expect(m.tris.size() == 1, "Expected exactly one triangle to be loaded");

    const triangle &tri = m.tris.front();
    Expect(tri.p[0].x == 0.0f && tri.p[0].y == 0.0f && tri.p[0].z == 0.0f, "First vertex mismatch");
    Expect(tri.p[1].x == 0.0f && tri.p[1].y == 1.0f && tri.p[1].z == 0.0f, "Second vertex mismatch");
    Expect(tri.p[2].x == 1.0f && tri.p[2].y == 0.0f && tri.p[2].z == 0.0f, "Third vertex mismatch");

    std::filesystem::remove(temp_file);
}

void TestLoadIgnoresEmptyLines()
{
    const auto temp_file = std::filesystem::temp_directory_path() / "mesh_test_empty_lines.obj";
    std::ofstream out(temp_file);
    out << "\n";
    out << "v 0 0 0\n\n";
    out << "v 0 0 1\n";
    out << "\n";
    out << "v 0 1 0\n";
    out << "f 1 2 3\n";
    out.close();

    mesh m;
    Expect(m.LoadFromObjectFile(temp_file.string()), "LoadFromObjectFile should succeed when file has empty lines");
    Expect(m.tris.size() == 1, "Empty lines should not introduce additional triangles");

    std::filesystem::remove(temp_file);
}

void TestLoadMissingFile()
{
    mesh m;
    Expect(!m.LoadFromObjectFile("/path/that/does/not/exist.obj"), "LoadFromObjectFile should return false for missing files");
}
} // namespace

int main()
{
    try
    {
        TestLoadSimple();
        TestLoadIgnoresEmptyLines();
        TestLoadMissingFile();
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Test failure: " << ex.what() << '\n';
        return 1;
    }

    std::cout << "All mesh tests passed!" << std::endl;
    return 0;
}
