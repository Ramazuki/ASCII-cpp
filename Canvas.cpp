#include "Canvas.hpp"
#include "CanvasIterators.hpp"
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <utility>

namespace plotter
{

namespace
{

size_t CheckedSize(int width, int height)
{
    if (width <= 0 || height <= 0)
    {
        throw std::invalid_argument("width and height must be positive, got "
            + std::to_string(width) + "x" + std::to_string(height));
    }
    return static_cast<size_t>(width) * static_cast<size_t>(height);
}

} // namespace

Canvas::Canvas(int width, int height, char background_char)
    : width_(width)
    , height_(height)
    , background_(background_char)
    , data_(CheckedSize(width, height), background_char)
{
}

Canvas::Canvas(Canvas&& other) noexcept
    : width_(other.width_)
    , height_(other.height_)
    , background_(other.background_)
    , data_(std::move(other.data_))
{
    other.width_ = 0;
    other.height_ = 0;
}

Canvas& Canvas::operator=(const Canvas& other)
{
    if (this != &other)
    {
        width_ = other.width_;
        height_ = other.height_;
        background_ = other.background_;
        data_ = other.data_;
    }
    return *this;
}

Canvas& Canvas::operator=(Canvas&& other) noexcept
{
    if (this != &other)
    {
        width_ = other.width_;
        height_ = other.height_;
        background_ = other.background_;
        data_ = std::move(other.data_);

        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

// ============================================================================

int Canvas::Width() const noexcept
{
    return width_;
}

int Canvas::Height() const noexcept
{
    return height_;
}

int Canvas::Size() const noexcept
{
    return width_ * height_;
}

size_t Canvas::Index(int x, int y) const noexcept
{
    return static_cast<size_t>(y) * static_cast<size_t>(width_) + static_cast<size_t>(x);
}

bool Canvas::InBounds(int x, int y) const noexcept
{
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

// ============================================================================

char& Canvas::at(int x, int y)
{
    if (!InBounds(x, y))
    {
        throw std::out_of_range("(" + std::to_string(x) + ", " + std::to_string(y)
            + ") is outside " + std::to_string(width_) + "x" + std::to_string(height_));
    }
    return data_[Index(x, y)];
}

const char& Canvas::at(int x, int y) const
{
    if (!InBounds(x, y))
    {
        throw std::out_of_range("(" + std::to_string(x) + ", " + std::to_string(y)
            + ") is outside " + std::to_string(width_) + "x" + std::to_string(height_));
    }
    return data_[Index(x, y)];
}

char& Canvas::operator()(int x, int y) noexcept
{
    return data_[Index(x, y)];
}

const char& Canvas::operator()(int x, int y) const noexcept
{
    return data_[Index(x, y)];
}

// ============================================================================

void Canvas::Clear(char fill_char)
{
    std::fill(data_.begin(), data_.end(), fill_char);
}

void Canvas::FillRegion(int x1, int y1, int x2, int y2, char fill_char)
{
    if (x1 > x2)
    {
        std::swap(x1, x2);
    }
    if (y1 > y2)
    {
        std::swap(y1, y2);
    }

    // Обрезаем по холсту
    x1 = std::max(x1, 0);
    y1 = std::max(y1, 0);
    x2 = std::min(x2, width_ - 1);
    y2 = std::min(y2, height_ - 1);

    if (x1 > x2 || y1 > y2)
    {
        return;
    }

    const auto row_length = static_cast<size_t>(x2 - x1 + 1);
    for (int y = y1; y <= y2; ++y)
    {
        std::fill_n(data_.data() + Index(x1, y), row_length, fill_char);
    }
}

// ============================================================================

void Canvas::Render(std::ostream& os) const
{
    for (int y = 0; y < height_; ++y)
    {
        os.write(data_.data() + Index(0, y), width_);
        os.put('\n');
    }
    os.flush();
}

void Canvas::SaveToFile(const std::filesystem::path& filepath) const
{
    const std::filesystem::path absolute_path = std::filesystem::absolute(filepath).lexically_normal();

    if (const std::filesystem::path parent = absolute_path.parent_path(); !parent.empty())
    {
        std::filesystem::create_directories(parent);
    }

    std::ofstream output(absolute_path, std::ios::out | std::ios::trunc);
    if (!output)
    {
        throw std::runtime_error("cannot open " + absolute_path.string());
    }

    output << "Canvas " << width_ << 'x' << height_ << '\n'
           << "Background: '" << background_ << "'\n"
           << "Content:\n";
    Render(output);
}

void Canvas::SaveToFile(const std::string& filename) const
{
    SaveToFile(std::filesystem::path(filename));
}

// ============================================================================

Canvas::RowIterator Canvas::RowBegin(int row)
{
    if (row < 0 || row >= height_)
    {
        throw std::out_of_range("row " + std::to_string(row) + " is out of range");
    }
    return { this, row, 0 };
}

Canvas::RowIterator Canvas::RowEnd(int row)
{
    if (row < 0 || row >= height_)
    {
        throw std::out_of_range("row " + std::to_string(row) + " is out of range");
    }
    return { this, row, width_ };
}

Canvas::ColumnIterator Canvas::ColBegin(int col)
{
    if (col < 0 || col >= width_)
    {
        throw std::out_of_range("column " + std::to_string(col) + " is out of range");
    }
    return { this, col, 0 };
}

Canvas::ColumnIterator Canvas::ColEnd(int col)
{
    if (col < 0 || col >= width_)
    {
        throw std::out_of_range("column " + std::to_string(col) + " is out of range");
    }
    return { this, col, height_ };
}

Canvas::PixelIterator Canvas::begin()
{
    return { this, 0 };
}

Canvas::PixelIterator Canvas::end()
{
    return { this, data_.size() };
}

} // namespace plotter
