#include <bits/stdc++.h>

template <typename T>
inline void fast_read(T& x) {
    char c;
    x = 0;
    while (!isdigit(c = getchar()));
    do x = x * 10 + (c - '0');
    while (isdigit(c = getchar()));
}

template <typename T, typename... A>
inline void fast_read(T& x, A&... a) {
    fast_read(x), fast_read(a...);
}


int n, m, k, ans;
std::vector<int> dis, sum, lst, arr, pre;
std::vector<std::tuple<int, int, int>> plan;

int main() {
    freopen("bus.in", "r", stdin);
    freopen("bus.out", "w", stdout);
    fast_read(n, m, k);

    plan.resize(m), dis.resize(n);
    sum.resize(n+1), lst.resize(n+1), arr.resize(n+1);
    pre.resize(n);

    for (auto d=dis.begin()+1; d<dis.end(); d++) fast_read(*d);
    for (auto& [t, u, v]: plan) {
        fast_read(t, u, v);
        sum[v]++;
        lst[u] = std::max(lst[u], t);
    }
    std::partial_sum(sum.begin()+1, sum.end(), sum.begin()+1);
    
    for (int i=1; i<=n; i++) 
        arr[i] = std::max(arr[i-1], lst[i-1]) + dis[i-1];
    for (const auto& [t, u, v]: plan) ans += arr[v] - t;

    while (k--) {
        pre[n-1] = n;
        for (int i=n-2; i; i--) {
            pre[i] = (arr[i+1]>lst[i+1]) ? pre[i+1] : i+1;
        }
        int ma = 0, pos = 0;
        for (int i=1; i<n; i++) {
            if (dis[i] > 0 && sum[pre[i]] - sum[i] > ma) {
                ma = sum[pre[i]] - sum[i];
                pos = i;
            }
        }
        ans -= ma;
        dis[pos]--;
        for (int i=pos+1; i<=n; i++) 
            arr[i] = std::max(arr[i-1], lst[i-1]) + dis[i-1];
    }
    
    std::cout << ans << std::endl;
}
