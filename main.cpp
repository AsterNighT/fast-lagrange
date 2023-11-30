#include "nttinterpolator.h"
#include <thread>

int run();

int main()
{
    srand(time(0));
    NTT::NTTInterpolator::init();
    std::thread t[16];
    for (int i = 0; i < 16; i++)
        t[i] = std::thread(run);
    for (int i = 0; i < 16; i++)
        t[i].join();
}

int run()
{
    std::vector<uint64_t> x;
    std::vector<std::vector<uint64_t>> y;
    NTT::NTTInterpolator *ntt = new NTT::NTTInterpolator();
    uint64_t n = 16300;
    x = std::vector<uint64_t>(n + 1);
    y.clear();
    for (uint64_t i = 0; i < 4; i++)
        y.push_back(std::vector<uint64_t>(n + 1));

    for (uint64_t i = 1; i <= n; i++)
    {
        x[i] = i;
        for (uint64_t j = 0; j < 4; j++)
            y[j][i] = rand() % NTT::mod;
    }
    // for (uint64_t i = 0; i < 4; i++)
    //     std::random_shuffle(y[i].begin() + 1, y[i].end());
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
