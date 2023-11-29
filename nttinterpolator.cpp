#include "nttinterpolator.h"

namespace NTT
{
    std::vector<int> w[C + 1];
    int rev[N << 1];
    void init_rev(int lim)
    {
        for (int i = 0; i < lim; i++)
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) * (lim >> 1));
    }
    void init_w()
    {
        for (int i = 1; i <= C; i++)
            w[i].resize((1 << (i - 1)));
        int wn = fast_power(G, (mod - 1) / (1 << C));
        w[C][0] = 1;
        for (int i = 1; i < (1 << (C - 1)); i++)
            w[C][i] = mul(w[C][i - 1], wn);
        for (int i = C - 1; i; i--)
            for (int j = 0; j < (1 << (i - 1)); j++)
                w[i][j] = w[i + 1][j << 1];
    }

// kd is the ntt direction, -1 for intt
void ntt(std::vector<int> &f, int lim, int kd) {
  for (int i = 0; i < lim; i++)
    if (i > rev[i])
      std::swap(f[i], f[rev[i]]);
  for (int mid = 1, l = 1; mid < lim; mid <<= 1, l++)
    for (int i = 0, a0, a1; i < lim; i += (mid << 1))
      for (int j = 0; j < mid; j++) {
        a0 = f[i + j], a1 = mul(f[i + j + mid], w[l][j]);
        f[i + j] = add(a0, a1), f[i + j + mid] = sub(a0, a1);
      }
  if (kd == -1) {
    std::reverse(f.begin() + 1, f.begin() + lim);
    for (int i = 0, inv = fast_power(lim, mod - 2); i < lim; i++)
      f[i] = mul(f[i], inv);
  }
}

    inline std::vector<int> operator+(const std::vector<int> &a, const std::vector<int> &b)
    {
        std::vector<int> c(std::max(a.size(), b.size()), 0);
        for (int i = 0; i < c.size(); i++)
            c[i] = add(a[i], b[i]);
        return c;
    }
    inline std::vector<int> operator-(const std::vector<int> &a, const std::vector<int> &b)
    {
        std::vector<int> c(std::max(a.size(), b.size()), 0);
        for (int i = 0; i < c.size(); i++)
            c[i] = sub(a[i], b[i]);
        return c;
    }
    inline std::vector<int> operator*(std::vector<int> a, std::vector<int> b)
    {
        int deg = a.size() + b.size() - 1, lim = 1;
        if (deg <= 128)
        {
            std::vector<int> c(deg, 0);
            for (int i = 0; i < a.size(); i++)
                for (int j = 0; j < b.size(); j++)
                    c[i + j] = add(c[i + j], mul(a[i], b[j]));
            return c;
        }
        while (lim < deg)
            lim <<= 1;
        init_rev(lim);
        a.resize(lim), ntt(a, lim, 1);
        b.resize(lim), ntt(b, lim, 1);
        for (int i = 0; i < lim; i++)
            a[i] = mul(a[i], b[i]);
        ntt(a, lim, -1), a.resize(deg);
        return a;
    }
    inline std::vector<int> Inv(std::vector<int> a, int deg)
    {
        std::vector<int> b, c(1, fast_power(a[0], mod - 2));
        for (int lim = 4; lim < (deg << 2); lim <<= 1)
        {
            b = a, b.resize(lim >> 1);
            init_rev(lim);
            b.resize(lim), ntt(b, lim, 1);
            c.resize(lim), ntt(c, lim, 1);
            for (int i = 0; i < lim; i++)
                c[i] = mul(c[i], sub(2, mul(b[i], c[i])));
            ntt(c, lim, -1), c.resize(lim >> 1);
        }
        c.resize(deg);
        return c;
    }
    inline std::vector<int> operator/(std::vector<int> a, std::vector<int> b)
    {
        int lim = 1, deg = a.size() - b.size() + 1;
        std::reverse(a.begin(), a.end());
        std::reverse(b.begin(), b.end());
        while (lim < deg)
            lim <<= 1;
        b = Inv(b, lim), b.resize(deg);
        a = a * b, a.resize(deg);
        std::reverse(a.begin(), a.end());
        return a;
    }
    inline std::vector<int> operator%(std::vector<int> a, std::vector<int> b)
    {
        std::vector<int> c = a - (a / b) * b;
        c.resize(b.size() - 1);
        return c;
    }
    inline std::vector<int> deriv(std::vector<int> a)
    {
        for (int i = 0; i < a.size() - 1; i++)
            a[i] = mul(a[i + 1], i + 1);
        a.pop_back();
        return a;
    }
    int F(const std::vector<int> a, int x)
    {
        int p = 1, res = 0;
        for (int i = 0; i < a.size(); i++, p = mul(p, x))
            res = add(res, mul(a[i], p));
        return res;
    }
#define lc (u << 1)
#define rc ((u << 1) | 1)
#define mid ((l + r) >> 1)

void NTTInterpolator::build() { build_inner(1, 1, n); }

void NTTInterpolator::build_inner(int u, int l, int r) {
  if (l == r) {
    f[u].push_back(mod - x[l]), f[u].push_back(1);
    return;
  }
  build_inner(lc, l, mid), build_inner(rc, mid + 1, r);
  f[u] = f[lc] * f[rc];
}

void NTTInterpolator::calc(std::vector<std::vector<int>> &y) {
  return calc_inner(1, 1, n, deriv_f, y);
}
void NTTInterpolator::calc_inner(int u, int l, int r, std::vector<int> res,
                                 std::vector<std::vector<int>> &y) {
  if (l == r) {
    for (int i = 0; i < poly_count; i++)
      g[i][l] = mul(fast_power(F(res, x[l]), mod - 2), y[i][l]);
    return;
  }
  calc_inner(lc, l, mid, res % f[lc], y),
      calc_inner(rc, mid + 1, r, res % f[rc], y);
}
std::vector<std::vector<int>> NTTInterpolator::get_ans() {
  std::vector<std::vector<int>> res;
  for (int i = 0; i < poly_count; i++) {
    res.push_back(get_ans_inner(1, 1, n, g[i]));
  }
  return res;
}

std::vector<int> NTTInterpolator::get_ans_inner(int u, int l, int r,
                                                std::vector<int> &g) {
  if (l == r)
    return std::vector<int>(1, g[l]);
  std::vector<int> ansl = get_ans_inner(lc, l, mid, g),
                   ansr = get_ans_inner(rc, mid + 1, r, g);
  return ansl * f[rc] + ansr * f[lc];
}

void NTTInterpolator::init() { init_w(); }

void NTTInterpolator::init_with_params(int n, std::vector<int> &x) {

  for (auto i = 0; i < (N << 2); i++)
    this->f[i].clear();
  this->n = n;
  this->x = x;
  this->build();
  this->deriv_f = deriv(f[1]);
}

std::vector<std::vector<int>>
NTTInterpolator::fast_lagrange(std::vector<std::vector<int>> y) {
  poly_count = y.size();
  g = std::vector<std::vector<int>>(poly_count, std::vector<int>(n + 1, 0));
  calc(y);
  return get_ans();
}

} // namespace NTT
