#pragma once

#include <algorithm>
#include <iostream>
#include <vector>

template<typename T>
void safeRemoveFromVector(std::vector<T>& vec, size_t i)
{
    std::swap(vec[i], vec.back());
    vec.pop_back();
}
