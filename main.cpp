#include "nttinterpolator.h"
std::vector<int> x;
std::vector<std::vector<int>> y;
int main() {
  NTT::NTTInterpolator::init();
  NTT::NTTInterpolator *ntt = new NTT::NTTInterpolator();

  int n = 1000;
  x = std::vector<int>(n + 1);
  for (int i = 0; i < 4; i++)
    y.push_back(std::vector<int>(n + 1));

  for (int i = 1; i <= n; i++) {
    x[i] = rand() % NTT::mod;
    for (int j = 0; j < 4; j++)
      y[j][i] = rand() % NTT::mod;
  }
  long start = clock();

  for (auto k = 0; k < 8192 * 5; k++) {
    ntt->init_with_params(n, x);
    std::vector<std::vector<int>> ans = ntt->fast_lagrange(y);
  }

  printf("%ld ms\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
  //   for (int i = 0; i < ans.size(); i++) {
  //     for (int j = 1; j <= n; j++) {
  //       if (y[i][j] != NTT::F(ans[i], x[j]))
  //         printf("Bad %d %d\n", i, j);
  //     }
  //   }
  return 0;
}