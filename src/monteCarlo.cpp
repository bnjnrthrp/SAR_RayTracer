#include "../include/common.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>

struct sample {
    double x;
    double p_x;
};

bool compare_by_x(const sample& a, const sample& b) {
    return a.x < b.x;
}

double icd(double d) {
    return 8.0 * std::pow(d, 1.0/3.0);
}

double f(const vec3& d) {
    auto cosine_squared = d.z() * d.z();
    return cosine_squared;
}

double pdf(const vec3& d) {
    return 1 / (4 * pi);
}


double pdf(double x) {
    return (3.0 / 8.0) * x * x;
}

void monteCarlo() {
    const unsigned int N = 10000;
    sample samples[N];
    double sum = 0.0;

    for (unsigned int i = 0; i < N; i++) {
        double x = random_double(0, 2 * pi);
        double sin_x = std::sin(x);
        double p_x = exp(-x / (2 * pi)) * sin_x * sin_x;
        sum += p_x;

        sample this_sample = { x, p_x };
        samples[i] = this_sample;
    }

    std::sort(std::begin(samples), std::end(samples), compare_by_x);

    double half_sum = sum / 2.0;
    double halfway_point = 0.0;
    double accum = 0.0;

    for (unsigned int i = 0; i < N; i++) {
        accum += samples[i].p_x;
        if (accum >= half_sum) {
            halfway_point = samples[i].x;
            break;
        }
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Average = " << (sum / N) << std::endl;
    std::cout << "Area under curve = " << 2 * pi * sum / N << std::endl;
    std::cout << "Halfway = " << halfway_point << std::endl;
}

void pdfExplicit() {
    int N = 1;
    auto sum = 0.0;

    for (int i = 0; i < N; i++) {
        auto z = random_double();
        if (z == 0.0)
            continue;
        auto x = icd(z);
        sum += x * x / pdf(x);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << (sum / N) << std::endl;
}

void sphere_importance() {

    int N = 1000000;
    auto sum = 0.0;
    for (int i = 0; i < N; i++) {
        vec3 d = random_unit_vector();
        auto f_d = f(d);
        sum += f_d / pdf(d);
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << sum / N << '\n';
}


int main() {
    switch (3) {
    case 1: monteCarlo(); break;
    case 2: pdfExplicit(); break;
    case 3: sphere_importance(); break;
    }
}