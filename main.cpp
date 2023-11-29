#include "nttinterpolator.h"
std::vector<int> x;
std::vector<std::vector<int>> y;
int main()
{
    srand(time(0));
    NTT::NTTInterpolator *ntt = new NTT::NTTInterpolator();
    int n = 32000;
    x = std::vector<int>(n+1);
    for (int i = 0; i < 4; i++)
        y.push_back(std::vector<int>(n+1));

    for (int i = 1; i <= n; i++)
    {
        x[i] = i % NTT::mod;
        for (int j = 0; j < 4; j++)
            y[j][i] = (i+j) % NTT::mod;
    }
    long start = clock();
    ntt->init_with_params(n, x);
    std::vector<std::vector<int>> ans = ntt->fast_lagrange(y);
    printf("%ld\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
    for (int i = 0; i < ans.size(); i++)
    {
        for (int j = 1; j <= n; j++)
            if (y[i][j] != NTT::F(ans[i], x[j]))
                printf("%d %d %d %d %d Bad\n", i, j, x[j], y[i][j], NTT::F(ans[i], x[j]));
    }
    return 0;
}