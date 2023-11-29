#include "nttinterpolator.h"
std::vector<uint64_t> x;
std::vector<std::vector<uint64_t>> y;
int main()
{
    srand(time(0));
    NTT::NTTInterpolator::init();
    NTT::NTTInterpolator *ntt = new NTT::NTTInterpolator();
    uint64_t n = 16000;
    x = std::vector<uint64_t>(n+1);
    for (uint64_t i = 0; i < 4; i++)
        y.push_back(std::vector<uint64_t>(n+1));

    for (uint64_t i = 1; i <= n; i++)
    {
        x[i] = i % NTT::mod;
        for (uint64_t j = 0; j < 4; j++)
            y[j][i] = (i+j) % NTT::mod;
    }
    long start = clock();
    ntt->init_with_params(n, x);
    std::vector<std::vector<uint64_t>> ans = ntt->fast_lagrange(y);
    printf("%ld\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
    for (uint64_t i = 0; i < ans.size(); i++)
    {
        for (uint64_t j = 1; j <= n; j++)
            if (y[i][j] != NTT::F(ans[i], x[j]))
                printf("%lu %lu %lu %lu %lu Bad\n", i, j, x[j], y[i][j], NTT::F(ans[i], x[j]));
    }
    return 0;
}