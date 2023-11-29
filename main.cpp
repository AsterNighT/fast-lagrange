#include "nttinterpolator.h"
std::vector<int> x;
std::vector<std::vector<int>> y;
int main()
{
    NTT::NTTInterpolator *ntt = new NTT::NTTInterpolator();
    int n = 1300;
    x = std::vector<int>(n);
    for (int i = 0; i < 4; i++)
        y.push_back(std::vector<int>(n));

    for (int i = 1; i <= n; i++)
    {
        x[i] = rand() % NTT::mod;
        for (int j = 0; j < 4; j++)
            y[j][i] = rand() % NTT::mod;
    }
    long start = clock();
    ntt->init_with_params(n, x);
    std::vector<std::vector<int>> ans = ntt->fast_lagrange(y);
    printf("%ld\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
    for (int i = 0; i < ans.size(); i++)
    {
        if (y[i][1] != NTT::F(ans[i], x[1]))
            printf("Bad\n");
    }
    return 0;
}