#pragma once

#include <vector>
#include "option_params.hpp"

struct option_params_batch
{
    std::vector<double> r;
    std::vector<double> sigma;
    std::vector<double> s0;
    std::vector<double> k;
    std::vector<double> t;

    void push_back(const option_params& p)
    {
        r.push_back(p.r);
        sigma.push_back(p.sigma);
        s0.push_back(p.s0);
        k.push_back(p.k);
        t.push_back(p.t);
    }
};