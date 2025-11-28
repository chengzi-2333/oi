// {P5788}
#include <bits/stdc++.h>

int n;
std::stack<std::pair<int, int>> st;
std::vector<int> ans;

signed main() {
#ifndef ONLINE_JUDGE
    freopen("monotonous-stack.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n;
    ans.resize(n);
    for (int a, i = 1; i <= n; i++) {
        std::cin >> a;
        while (!st.empty() && st.top().second < a) {
            ans[st.top().first - 1] = i;
            st.pop();
        }
        st.emplace(i, a);
    }
    std::copy(ans.begin(), ans.end(), std::ostream_iterator<int>(std::cout, " "));
}
