#pragma once
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdio>
#include <cstdint>

namespace NTT
{
    const uint64_t mod = 1179649, G = 19;
    const uint64_t N = (1 << 15) + 1, C = 17;
    inline uint64_t add(uint64_t a, uint64_t b) { return a + b >= mod ? a + b - mod : a + b; }
    inline uint64_t sub(uint64_t a, uint64_t b) { return a >= b ? a - b : a - b + mod; }
    inline uint64_t mul(uint64_t a, uint64_t b) { return a * b >= mod ? a * b % mod : a * b; }
    inline uint64_t fast_power(uint64_t a, uint64_t b, uint64_t res = 1)
    {
        for (; b; b >>= 1, a = mul(a, a))
            (b & 1) ? (res = mul(res, a)) : 0;
        return res;
    }
    // N: poly degree limit
    // C: power limit
    // Init all powers
    void init_w();
    // Evaluate polynomial with coefficients a at x
    uint64_t F(const std::vector<uint64_t> a, uint64_t x);
    class NTTInterpolator
    {
    public:
        static void init();
        void init_with_params(uint64_t n, std::vector<uint64_t> &x);
        std::vector<std::vector<uint64_t>> fast_lagrange(std::vector<std::vector<uint64_t>> y);

    private:
        void build_inner(uint64_t u, uint64_t l, uint64_t r);
        void calc_inner(uint64_t u, uint64_t l, uint64_t r, std::vector<uint64_t> res, std::vector<std::vector<uint64_t>> &y);
        std::vector<uint64_t> get_ans_inner(uint64_t u, uint64_t l, uint64_t r, std::vector<uint64_t> &g);
        void build();
        void calc(std::vector<std::vector<uint64_t>> &y);
        std::vector<std::vector<uint64_t>> get_ans();
        uint64_t n;
        uint64_t poly_count;
        std::vector<uint64_t> x;
        std::vector<uint64_t> f[N << 1];
        uint64_t rev[N << 1];
        std::vector<uint64_t> deriv_f;
        std::vector<std::vector<uint64_t>> g;
    };
}
