// {P1337}
#include <bits/stdc++.h>

struct vec2 {
    double x, y;

    vec2() : x(0), y(0) {}

    vec2(double x, double y) : x(x), y(y) {}

    vec2(const vec2& v) : x(v.x), y(v.y) {}

    double magnitude() const { return std::sqrt(x * x + y * y); }

    vec2 normalize() const {
        if (x == 0. && y == 0.) return {x, y};
        const auto mag = magnitude();
        return {x / mag, y / mag};
    }

    vec2 operator+(const vec2& v) const { return {x + v.x, y + v.y}; }

    vec2 operator-(const vec2& v) const { return {x - v.x, y - v.y}; }

    vec2 operator*(double k) const { return {k * x, k * y}; }

    vec2 operator/(double k) const { return {x / k, y / k}; }

    vec2 operator-() const { return {-x, -y}; }

    bool operator<(const vec2& v) const {
        return this->magnitude() < v.magnitude();
    }

    vec2& operator=(const vec2& v) {
        x = v.x, y = v.y;
        return *this;
    }

    vec2& operator+=(const vec2& v) {
        x += v.x, y += v.y;
        return *this;
    }

    void print() const { std::cout << x << ' ' << y << '\n'; }
};

constexpr double k = 0.999, T0 = 10000, Tk = 0.001;
std::random_device dev;
std::mt19937 eng(dev());
std::uniform_real_distribution<double> rng(0.0, 1.0);

int n;
std::vector<std::pair<vec2, double>> forces;

inline vec2 gen_average() {
    double tot_weight = 0;
    vec2 u;
    for (const auto& [vec, weight] : forces) {
        u += vec * weight;
        tot_weight += weight;
    }
    return u / tot_weight;
}

// minimize this function (approach to 0)
// \sum_{i=1}^{n} \frac{\vec{v_i} - \vec{r}}{|\vec{v_i} - \vec{r}|} w_i
inline vec2 calculate(const vec2& r) {
    vec2 u;
    for (const auto& [vec, weight] : forces) {
        u += (vec - r).normalize() * weight;
    }
    return u;
}

vec2 simulate_annealing(const vec2& init) {
    double t = T0;
    vec2 ans(init), u(init), v(init);
    while (t > Tk) {
        v = u + vec2(t * (rng(eng) * 2 - 1), t * (rng(eng) * 2 - 1));
        auto fu = calculate(u), fv = calculate(v), fans = calculate(ans);
        if (fv < fans) ans = v;
        auto dE = fv - fu;
        if (std::exp(-dE.magnitude() / t) > rng(eng)) u = v;
        t *= k;
    }
    for (int i = T0; i; i--) {
        auto v = ans + vec2(t * (rng(eng) * 2 - 1), t * (rng(eng) * 2 - 1));
        if (calculate(v) < calculate(ans)) ans = v;
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
    simulate_annealing(gen_average()).print();
}
