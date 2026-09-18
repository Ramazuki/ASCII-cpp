#pragma once
#include <map>
#include <string>
#include <vector>

namespace plotter
{

struct PlotterConfig
{
    int width = 0;
    int height = 0;
    char background_char = ' ';
    std::vector<char> palette;
    std::string plotter_type; // "basic" или "grayscale"
};

class Config
{
public:
    static PlotterConfig LoadFromFile(const std::string& filename);
    static PlotterConfig LoadFromString(std::istream& json_str);
    static bool ValidateConfig(const PlotterConfig& config);
    static PlotterConfig DefaultConfig();

private:
    static std::vector<char> ParsePalette(const std::string& palette_str);
};

} // namespace plotter