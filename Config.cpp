#include "Config.hpp"
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include "json.h"

namespace plotter
{

namespace
{

const json::Node& RequireField(const json::Dict& fields, const std::string& key)
{
    const auto it = fields.find(key);
    if (it == fields.end())
    {
        throw std::invalid_argument("missing required field '" + key + "'");
    }
    return it->second;
}

int ReadInt(const json::Dict& fields, const std::string& key)
{
    const json::Node& node = RequireField(fields, key);
    if (!node.IsInt())
    {
        throw std::invalid_argument("field '" + key + "' must be an integer");
    }
    return node.AsInt();
}

const std::string& ReadString(const json::Dict& fields, const std::string& key)
{
    const json::Node& node = RequireField(fields, key);
    if (!node.IsString())
    {
        throw std::invalid_argument("field '" + key + "' must be a string");
    }
    return node.AsString();
}

} // namespace

PlotterConfig Config::LoadFromFile(const std::string& filename)
{
    std::ifstream input(filename);
    if (!input)
    {
        throw std::runtime_error("cannot open " + filename);
    }
    return LoadFromString(input);
}

PlotterConfig Config::LoadFromString(std::istream& json_str)
{
    const json::Document document = json::Load(json_str);
    if (!document.GetRoot().IsDict())
    {
        throw std::invalid_argument("root must be a JSON object");
    }
    const json::Dict& fields = document.GetRoot().AsDict();

    PlotterConfig config;
    config.width = ReadInt(fields, "width");
    config.height = ReadInt(fields, "height");

    const std::string background = ReadString(fields, "background_char");
    if (background.size() != 1)
    {
        throw std::invalid_argument("field 'background_char' must be exactly one character");
    }
    config.background_char = background.front();

    config.plotter_type = ReadString(fields, "plotter_type");

    if (const auto it = fields.find("palette"); it != fields.end())
    {
        if (!it->second.IsString())
        {
            throw std::invalid_argument("field 'palette' must be a string");
        }
        config.palette = ParsePalette(it->second.AsString());
    }

    if (!ValidateConfig(config))
    {
        throw std::invalid_argument("invalid configuration values");
    }

    return config;
}

bool Config::ValidateConfig(const PlotterConfig& config)
{
    if (config.width <= 0 || config.height <= 0)
    {
        return false;
    }
    return config.plotter_type == "basic" || config.plotter_type == "grayscale";
}

PlotterConfig Config::DefaultConfig()
{
    PlotterConfig config;
    config.width = 100;
    config.height = 50;
    config.background_char = '.';
    config.palette = ParsePalette(" .:-=+*#%@");
    config.plotter_type = "grayscale";

    return config;
}

std::vector<char> Config::ParsePalette(const std::string& palette_str)
{
    return { palette_str.begin(), palette_str.end() };
}

} // namespace plotter
