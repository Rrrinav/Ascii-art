#pragma once
#include <vector>
#include <functional>

struct index
{
    size_t row;
    size_t col;

    index() = default;
    index(size_t ro, size_t co) : row(ro), col(co) {}

    bool is_in_range(const index &min, const index &max)
    {
        return row >= min.row && row < max.row && col >= min.col && col < max.col;
    }

    bool is_in_range(const index &max)
    {
        return row >= 0 && row < max.row && col >= 0 && col < max.col;
    }
};

std::vector<size_t> range(size_t end)
{
    std::vector<size_t> res(end);
    for (auto i = 0u; i < end; ++i)
    {
        res.at(i) = i;
    }
    return res;
}

template <typename T>
class Grid
{

private:
    std::vector<T> _data;
    size_t _rows;
    size_t _cols;

public:
    // Default constructor
    Grid() {}

    // Constructor to initialize grid with specified rows and cols
    Grid(size_t rows, size_t cols) : _rows(rows), _cols(cols)
    {
        _data.resize(rows * cols);
    }

    // Constructor to initialize grid with specified rows, cols, and initial value
    Grid(size_t rows, size_t cols, T init_val) : _rows(rows), _cols(cols)
    {
        _data.resize(rows * cols, init_val);
    }

    // Constructor to initialize grid using a function
    Grid(size_t rows, size_t cols, std::function<T()> initializer) : _rows(rows), _cols(cols)
    {
        _data.resize(rows * cols);
        for (size_t i = 0; i < _data.size(); ++i)
        {
            _data[i] = initializer();
        }
    }

    // Constructor to initialize grid using a function with indices
    Grid(size_t rows, size_t cols, std::function<T(size_t, size_t)> initializer) : _rows(rows), _cols(cols)
    {
        _data.resize(rows * cols);
        for (size_t row = 0; row < rows; ++row)
        {
            for (size_t col = 0; col < cols; ++col)
            {
                _data[row * cols + col] = initializer(row, col);
            }
        }
    }

    // Positional system: Function to point at a specific point in the grid....
    T &at(size_t row, size_t col) { return _data.at(row * _cols + col); }
    const T &at(size_t row, size_t col) const { return _data.at(row * _cols + col); } // Const one

    // Get sizes....
    size_t row_size() const { return _rows; }
    size_t col_size() const { return _cols; }
    size_t total_size() const { return _data.size(); }

    // Get ranges i.e. successive values of rows and columns
    std::vector<size_t> rows() const { return range(_rows); }
    std::vector<size_t> cols() const { return range(_cols); }

    // Index system
    index max_index() const { return index(_rows, _cols); }

    std::vector<index> get_neighbours(size_t ro, size_t co)
    {
        std::vector<index> neighbouring_indices;
        for (auto i = -1; i <= 1; ++i)
        {
            for (auto ii = -1; ii <= 1; ++ii)
            {
                if (i == 0 && ii == 0)
                    continue;

                index Index(ro + i, co + ii);
                if (Index.is_in_range(max_index()))
                {
                    neighbouring_indices.push_back(Index);
                }
            }
        }
        return neighbouring_indices;
    }
};
