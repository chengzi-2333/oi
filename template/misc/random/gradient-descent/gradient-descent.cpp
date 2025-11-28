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

constexpr double eta = 0.001, TIME_LIMIT = 0.98;

inline bool time_check() {
    return (double)std::clock() / CLOCKS_PER_SEC < TIME_LIMIT;
}

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

// x <- x - eta * df/dx
// in this case, -df/dx = <total force of current position>
inline vec2 gradient_descent(vec2 r) {
    while (time_check()) r += calculate(r) * eta;
    return r;
}

signed main() {
#ifndef ONLINE_JUDGE
    freopen("gradient-descent.in", "r", stdin);
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
    gradient_descent(gen_average()).print();
}
