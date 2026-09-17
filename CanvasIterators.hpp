#pragma once
#include "Canvas.hpp"
#include <cstddef>
#include <iterator>

namespace plotter
{

class Canvas::RowIterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = char;
    using difference_type = std::ptrdiff_t;
    using pointer = char*;
    using reference = char&;

    RowIterator() noexcept = default;

    RowIterator(Canvas* canvas, int row, int col) noexcept
        : canvas_(canvas)
        , row_(row)
        , col_(col)
    {
    }

    reference operator*() const
    {
        return canvas_->data_[canvas_->Index(col_, row_)];
    }

    pointer operator->() const
    {
        return &**this;
    }

    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }

    RowIterator& operator++()
    {
        ++col_;
        return *this;
    }

    RowIterator operator++(int)
    {
        RowIterator tmp = *this;
        ++col_;
        return tmp;
    }

    RowIterator& operator--()
    {
        --col_;
        return *this;
    }

    RowIterator operator--(int)
    {
        RowIterator tmp = *this;
        --col_;
        return tmp;
    }

    RowIterator& operator+=(difference_type n)
    {
        col_ += static_cast<int>(n);
        return *this;
    }

    RowIterator& operator-=(difference_type n)
    {
        col_ -= static_cast<int>(n);
        return *this;
    }

    RowIterator operator+(difference_type n) const
    {
        RowIterator tmp = *this;
        return tmp += n;
    }

    RowIterator operator-(difference_type n) const
    {
        RowIterator tmp = *this;
        return tmp -= n;
    }

    difference_type operator-(const RowIterator& other) const
    {
        return col_ - other.col_;
    }

    bool operator==(const RowIterator& other) const
    {
        return canvas_ == other.canvas_ && row_ == other.row_ && col_ == other.col_;
    }

    bool operator!=(const RowIterator& other) const
    {
        return !(*this == other);
    }

    bool operator<(const RowIterator& other) const
    {
        return col_ < other.col_;
    }

    bool operator>(const RowIterator& other) const
    {
        return other < *this;
    }

    bool operator<=(const RowIterator& other) const
    {
        return !(other < *this);
    }

    bool operator>=(const RowIterator& other) const
    {
        return !(*this < other);
    }

private:
    Canvas* canvas_ = nullptr;
    int row_ = 0;
    int col_ = 0;
};

// Симметричная форма n + it
inline Canvas::RowIterator operator+(Canvas::RowIterator::difference_type n, const Canvas::RowIterator& it)
{
    return it + n;
}

class Canvas::ColumnIterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = char;
    using difference_type = std::ptrdiff_t;
    using pointer = char*;
    using reference = char&;

    ColumnIterator() noexcept = default;

    ColumnIterator(Canvas* canvas, int col, int row) noexcept
        : canvas_(canvas)
        , col_(col)
        , row_(row)
    {
    }

    reference operator*() const
    {
        return canvas_->data_[canvas_->Index(col_, row_)];
    }

    pointer operator->() const
    {
        return &**this;
    }

    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }

    ColumnIterator& operator++()
    {
        ++row_;
        return *this;
    }

    ColumnIterator operator++(int)
    {
        ColumnIterator tmp = *this;
        ++row_;
        return tmp;
    }

    ColumnIterator& operator--()
    {
        --row_;
        return *this;
    }

    ColumnIterator operator--(int)
    {
        ColumnIterator tmp = *this;
        --row_;
        return tmp;
    }

    ColumnIterator& operator+=(difference_type n)
    {
        row_ += static_cast<int>(n);
        return *this;
    }

    ColumnIterator& operator-=(difference_type n)
    {
        row_ -= static_cast<int>(n);
        return *this;
    }

    ColumnIterator operator+(difference_type n) const
    {
        ColumnIterator tmp = *this;
        return tmp += n;
    }

    ColumnIterator operator-(difference_type n) const
    {
        ColumnIterator tmp = *this;
        return tmp -= n;
    }

    difference_type operator-(const ColumnIterator& other) const
    {
        return row_ - other.row_;
    }

    bool operator==(const ColumnIterator& other) const
    {
        return canvas_ == other.canvas_ && col_ == other.col_ && row_ == other.row_;
    }

    bool operator!=(const ColumnIterator& other) const
    {
        return !(*this == other);
    }

    bool operator<(const ColumnIterator& other) const
    {
        return row_ < other.row_;
    }

    bool operator>(const ColumnIterator& other) const
    {
        return other < *this;
    }

    bool operator<=(const ColumnIterator& other) const
    {
        return !(other < *this);
    }

    bool operator>=(const ColumnIterator& other) const
    {
        return !(*this < other);
    }

private:
    Canvas* canvas_ = nullptr;
    int col_ = 0;
    int row_ = 0;
};

// Симметричная форма n + it
inline Canvas::ColumnIterator operator+(Canvas::ColumnIterator::difference_type n, const Canvas::ColumnIterator& it)
{
    return it + n;
}

class Canvas::PixelIterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = char;
    using difference_type = std::ptrdiff_t;
    using pointer = char*;
    using reference = char&;

    PixelIterator() noexcept = default;

    PixelIterator(Canvas* canvas, size_t pos) noexcept
        : canvas_(canvas)
        , pos_(pos)
    {
    }

    reference operator*() const
    {
        return canvas_->data_[pos_];
    }

    pointer operator->() const
    {
        return &**this;
    }

    reference operator[](difference_type n) const
    {
        return *(*this + n);
    }

    [[nodiscard]] int X() const noexcept
    {
        return static_cast<int>(pos_ % static_cast<size_t>(canvas_->width_));
    }

    [[nodiscard]] int Y() const noexcept
    {
        return static_cast<int>(pos_ / static_cast<size_t>(canvas_->width_));
    }

    PixelIterator& operator++()
    {
        ++pos_;
        return *this;
    }

    PixelIterator operator++(int)
    {
        PixelIterator tmp = *this;
        ++pos_;
        return tmp;
    }

    PixelIterator& operator--()
    {
        --pos_;
        return *this;
    }

    PixelIterator operator--(int)
    {
        PixelIterator tmp = *this;
        --pos_;
        return tmp;
    }

    PixelIterator& operator+=(difference_type n)
    {
        pos_ = static_cast<size_t>(static_cast<difference_type>(pos_) + n);
        return *this;
    }

    PixelIterator& operator-=(difference_type n)
    {
        pos_ = static_cast<size_t>(static_cast<difference_type>(pos_) - n);
        return *this;
    }

    PixelIterator operator+(difference_type n) const
    {
        PixelIterator tmp = *this;
        return tmp += n;
    }

    PixelIterator operator-(difference_type n) const
    {
        PixelIterator tmp = *this;
        return tmp -= n;
    }

    difference_type operator-(const PixelIterator& other) const
    {
        return static_cast<difference_type>(pos_) - static_cast<difference_type>(other.pos_);
    }

    bool operator==(const PixelIterator& other) const
    {
        return canvas_ == other.canvas_ && pos_ == other.pos_;
    }

    bool operator!=(const PixelIterator& other) const
    {
        return !(*this == other);
    }

    bool operator<(const PixelIterator& other) const
    {
        return pos_ < other.pos_;
    }

    bool operator>(const PixelIterator& other) const
    {
        return other < *this;
    }

    bool operator<=(const PixelIterator& other) const
    {
        return !(other < *this);
    }

    bool operator>=(const PixelIterator& other) const
    {
        return !(*this < other);
    }

private:
    Canvas* canvas_ = nullptr;
    size_t pos_ = 0;
};

// Симметричная форма n + it
inline Canvas::PixelIterator operator+(Canvas::PixelIterator::difference_type n, const Canvas::PixelIterator& it)
{
    return it + n;
}

} // namespace plotter
