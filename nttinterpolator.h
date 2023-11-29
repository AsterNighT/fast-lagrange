#pragma once
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdio>

namespace NTT
{
    const int mod = 1785857, G = 3;
    inline int add(int a, int b) { return a + b >= mod ? a + b - mod : a + b; }
    inline int sub(int a, int b) { return a >= b ? a - b : a - b + mod; }
    inline int mul(int a, int b) { return 1ll * a * b >= mod ? 1ll * a * b % mod : a * b; }
    inline int fast_power(int a, int b, int res = 1)
    {
        for (; b; b >>= 1, a = mul(a, a))
            (b & 1) ? (res = mul(res, a)) : 0;
        return res;
    }
    // N: poly degree limit
    // C: power limit
    const int N = (1 << 17) + 1, C = 14;
    // Init all powers
    void init_w();
    // Evaluate polynomial with coefficients a at x
    int F(const std::vector<int> a, int x);
    class NTTInterpolator
    {
    public:
        static void init();
        void init_with_params(int n, std::vector<int> &x);
        std::vector<std::vector<int>> fast_lagrange(std::vector<std::vector<int>> y);

    private:
        void build_inner(int u, int l, int r);
        void calc_inner(int u, int l, int r, std::vector<int> res, std::vector<std::vector<int>> &y);
        std::vector<int> get_ans_inner(int u, int l, int r, std::vector<int> &g);
        void build();
        void calc(std::vector<std::vector<int>> &y);
        std::vector<std::vector<int>> get_ans();
        int n;
        int poly_count;
        std::vector<int> x;
        std::vector<int> f[N << 2];
        std::vector<int> deriv_f;
        std::vector<std::vector<int>> g;
    };
}
