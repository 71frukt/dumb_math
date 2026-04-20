#include <iostream>
#include <filesystem>

#include "RLogSU/logger.hpp"
#include "matrix/matrix.hpp"

namespace dumb_math::matrix::tests::iostream {

std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
{
    os << std::setprecision(std::numeric_limits<float>::max_digits10);

    for (size_t row = 0; row < matrix.rows(); ++row)
    {
        for (size_t col = 0; col < matrix.cols(); ++col)
        {
            os << matrix[row, col] << " ";
        }

        os << std::endl;
    }

    os << std::setprecision(6);
    
    return os;
}


std::istream& operator>>(std::istream& is, Matrix& matrix)
{
    std::vector<float> temp_data;

    size_t cols = 0;
    size_t rows = 0;

    std::string line;

    while (std::getline(is, line))
    {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos)
        {
            if (rows > 0)
                break; 
            
            continue;
        }

        std::stringstream ss(line);
        float value;
        size_t current_cols = 0;

        while (ss >> value)
        {
            temp_data.push_back(value);
            ++current_cols;
        }

        if (!ss.eof() && ss.fail())
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        if (rows == 0)
        {
            cols = current_cols;
        }

        else if (cols != current_cols)
        {
            is.setstate(std::ios_base::failbit);
            return is;
        }

        ++rows;
    }

    if (rows == 0 || cols == 0)
    {
        is.setstate(std::ios_base::failbit);
        return is;
    }

    Matrix tmp(rows, cols);
    size_t index = 0;
    for (size_t r = 0; r < rows; ++r)
    {
        for (size_t c = 0; c < cols; ++c)
        {
            tmp[r, c] = temp_data[index++];
        }
    }

    matrix = std::move(tmp);

    if (is.eof())
    {
        is.clear(is.rdstate() & ~std::ios_base::failbit);
    }

    return is;
}

static void BuildMatrixFromFile(Matrix& matrix, std::filesystem::path source_path)
{
    std::ifstream source(source_path);
    if (!source)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to open '{}'", source_path.c_str()));

    using namespace tests::iostream;
    source >> matrix;
    
    if (!source)
    {
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to parce matrix [{}x{}] in '{}'", matrix.rows(), matrix.cols(), source_path.c_str()));
    }
}

} // namespace dumb_math::matrix::tests::iostream