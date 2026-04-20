#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include "RLogSU/logger.hpp"
#include "common/matrix_iostream.hpp"
#include "assets/config.hpp"
#include "matrix/matrix.hpp"

namespace dumb_math::matrix::tests::correctness::e2e {

template <typename Func>
requires std::invocable<Func, const Matrix&, const Matrix&, Matrix&>
void MatrixE2eMul(Func mul_func, const assets::config::MatrixShopItem& m1_info,
                                 const assets::config::MatrixShopItem& m2_info,
                                 const std::filesystem::path&          dest_path)
{
    std::ifstream m1_source   (m1_info.filepath);
    std::ifstream m2_source   (m2_info.filepath);
    std::ofstream destination (dest_path);

    if (!m1_source)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to open '{}'", m1_info.filepath.string()));
    if (!m2_source)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to open '{}'", m2_info.filepath.string()));
    if (!destination)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to open '{}'", dest_path.string()));

    Matrix m1       (m1_info.rows, m1_info.cols);
    Matrix m2       (m2_info.rows, m2_info.cols);
    Matrix m1_mul_m2(m1.rows(),    m2.cols());
    
    using namespace tests::iostream;
    m1_source >> m1;
    m2_source >> m2;

    if (!m1_source)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to parce matrix m1[{}x{}] from '{}'", 
                                                            m1.rows(), m1.cols(), m1_info.filepath.string()));
     if (!m2_source)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to parce matrix m2[{}x{}] from '{}'", 
                                                            m2.rows(), m2.cols(), m2_info.filepath.string()));

    mul_func(m1, m2, m1_mul_m2);

    destination << m1_mul_m2;
    
    destination.close();
    if (!destination)
        RLSU_THROW<std::runtime_error>(RLSU_FORMAT("unable to write/flush matrix m1_mul_m2[{}x{}] to '{}'", 
                                                            m1_mul_m2.rows(), m1_mul_m2.cols(), dest_path.string()));
}

} // namespace dumb_math::matrix::tests::correctness::e2e