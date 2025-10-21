
#include <bits/stdc++.h>
using namespace std;
const int C = 20, N = 1e5 + 10;
int card[C];  // 购书卡的面额
int sum[N];   // 书价前缀和
int c, n;
// dp[s] ：如果用了 s 状态对应二进制位置的购书卡 , 从前向后最多能买的数的数量
int dp[1 << C];
int val[1 << C];  // 每个状态对应的剩余卡面值总和
int ans = -1;
// 打暴力部分用到的变量
int pm[10];
// 当前排列
bool f[10];  // 标记是否已使用
// 暴力模拟当前排列能否买完所有书 , 返回剩余金额
void get() {
    // i ：枚举卡的下标 ,j ：枚举书的下标
    for (int i = 0, j = 1; i < c; i++) {  // 枚举每张卡
        int s = 0;                        // 当前卡买书花了多少钱
        // 用第 i 张卡 , 把能买的数都买了
        while (j <= n && s + (sum[j] - sum[j - 1]) <= card[pm[i]]) {
            s += (sum[j] - sum[j - 1]);
            j++;
        }
        if (j > n) {  // 已买完所有书
            int r = 0;
            // 计算剩余卡的总金额
            for (int k = i + 1; k < c; k++)
                r += card[pm[k]];
            ans = max(ans, r);
            return;
        }
    }
    // 所有卡用完但书未买完 , 不更新
}
// 针对 c<=6 的暴力求解 , 枚举所有卡的排列
void dfs(int k) {
    if (k == c) {
        get();
        return;
    }
    for (int i = 0; i < c; i++) {
        if (!f[i]) {
            f[i] = true;
            pm[k] = i;
            dfs(k + 1);
            f[i] = false;
        }
    }
}
// 二分找最远能买到的书位置
int find(int st, int p) {
    int l = st, r = n;
    while (l <= r) {
        int mid = (l + r) >> 1;
        if (sum[mid] - sum[st - 1] <= card[p]) {
            l = mid + 1;
        } else
            r = mid - 1;
    }
    return l - 1;
}
int main() {
    cin >> c >> n;  // c 张卡 ,n 本数
    for (int i = 0; i < c; i++)
        cin >> card[i];  // 每张卡的金额
    // n 本书价格的前缀和 , 方便求区间和
    for (int i = 1; i <= n; i++) {
        int p;
        cin >> p;
        sum[i] = sum[i - 1] + p;
    }
    // 小规模数据 , 暴力求解 (40 分 )
    if (c <= 6) {
        dfs(0);
        cout << ans;
        return 0;
    }
    // 预处理每个状态下剩余卡的总面值
    for (int s = 0; s < (1 << c); s++) {
        long long sum = 0;
        for (int i = 0; i < c; i++) {
            if ((s >> i) & 1)
                continue;  // 用过的卡
            sum += card[i];
        }
        val[s] = sum;
    }
    memset(dp, -1, sizeof(dp));
    dp[0] = 0;                            // 初始状态 , 买 0 本书
    for (int s = 0; s < (1 << c); s++) {  // 枚举卡的使用状态
        if (dp[s] == -1)
            continue;   // 这个状态没有得到过 , 无法转移到其他状态
        int x = dp[s];  // 当前状态买到第 x 本书
        // 枚举所有的卡
        for (int j = 0; j < c; j++) {
            if ((s >> j) & 1)
                continue;            // 已使用过的卡跳过
            int y = find(x + 1, j);  // 计算这张卡最多买到哪个位置
            dp[s | (1 << j)] = max(dp[s | (1 << j)], y);  // 状态转移
            if (dp[s | (1 << j)] == n) {                  // 买完所有书
                ans = max(ans, val[s | (1 << j)]);
            }
        }
    }
    cout << ans;
    return 0;
}
