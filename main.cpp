#include "nttinterpolator.h"
#include <thread>
#include <chrono>
int run();

const int threads = 32;

int main()
{
    srand(time(0));
    NTT::NTTInterpolator::init();
    auto start = std::chrono::steady_clock::now();
    std::thread t[threads];
    for (int i = 0; i < threads; i++)
        t[i] = std::thread(run);
    for (int i = 0; i < threads; i++)
        t[i].join();
    auto elapsed = std::chrono::steady_clock::now() - start;
    printf("%ld ms\n", elapsed.count() / 1000000);
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
    for(uint64_t i = 0; i < 1250; i++){
        ntt->init_with_params(n, x);
        std::vector<std::vector<uint64_t>> ans = ntt->fast_lagrange(y);
    }
    // for (uint64_t i = 0; i < ans.size(); i++)
    // {
    //     for (uint64_t j = 1; j <= n; j++)
    //         if (y[i][j] != NTT::F(ans[i], x[j]))
    //             printf("%lu %lu %lu %lu %lu Bad\n", i, j, x[j], y[i][j], NTT::F(ans[i], x[j]));
    // }
    return 0;
}
