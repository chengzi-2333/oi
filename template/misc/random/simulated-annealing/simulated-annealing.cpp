// {P1337}
#include <bits/stdc++.h>

struct vec2 {
    double x, y;

    vec2() : x(0), y(0) {}

    vec2(double x, double y) : x(x), y(y) {}

    vec2(const vec2& v) : x(v.x), y(v.y) {}

    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    vec2 normalize() const {
        if (x == 0. && y == 0.) return {x, y};
        const auto mag = magnitude();
        return {x / mag, y / mag};
    }

    vec2 operator +(const vec2& v) const {
        return {x + v.x, y + v.y};
    }

    vec2 operator -(const vec2& v) const {
        return {x - v.x, y - v.y};
    }

    vec2 operator *(double k) const {
        return {k * x, k * y};
    }

    bool operator <(const vec2& v) const {
        return x < v.x && y < v.y;
    }

    vec2 operator =(const vec2& v) {
        return {x = v.x, y = v.y};
    }
};

constexpr double k = 0.999, T0 = 47, Tk = 1;
std::random_device dev;
std::mt19937 eng(dev());
std::uniform_real_distribution<double> rng(0.0, 1.0);

int n;
std::vector<std::pair<vec2, double>> forces;

// \sum_{i=1}^{n} \frac{\vec{v_i} - \vec{r}}{|\vec{v_i} - \vec{r}|} w_i
inline vec2 calculate(const vec2& r) {
    vec2 u;
    for (const auto& [vec, weight] : forces) {
        u = u + (vec - r).normalize() * weight;
    }
    return u;
}

vec2 simulate_annealing(const vec2& init) {
    auto t = T0, pds = 9999999.;
    vec2 ans(init), u(init);
    while (t > Tk) {
        auto v = u + vec2(t * (rng(eng) * 2 - 1), t * (rng(eng) * 2 - 1));
        auto dv = calculate(v) - calculate(u);
        double ds = dv.magnitude();
        if (pds > ds) pds = ds, ans = v;
        if (std::exp(-dv.magnitude() / t) > rng(eng)) u = v;
        t *= k;
    }
    for (int i = 1; i <= 1000; i++) {
        auto v = ans + vec2(t * (rng(eng) * 2 - 1), t * (rng(eng) * 2 - 1));
        auto dv = calculate(v) - calculate(u);
        double ds = dv.magnitude();
        if (pds > ds) pds = ds, ans = v;
    }
    return ans;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("simulated-annealing.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cout << std::fixed;
    std::cout.precision(3);
    std::cin >> n;
    forces.resize(n);
    for (auto& [vec, weight] : forces) {
        auto& [x, y] = vec;
        std::cin >> x >> y >> weight;
    }
    const auto [x, y] = simulate_annealing(calculate({}));
    std::cout << x << ' ' << y << std::endl;
}
