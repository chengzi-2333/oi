#include <bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10, M = 1e4 + 10;
struct node {
    int t, s, e;
} a[M];
int bus[N];   // 车到每个站的时间
int last[N];  // 每个站最后一个人的到站时间
int c[N];     // 每个站点会影响到最多哪个站点
int sum[N];   // 到每个站点人数的前缀和 , 方便求区间和
int dis[N];   // 每个站到下一站的时间
int n, m, k, ans;
void get1() {
    int mi = INT_MAX;
    // 枚举哪一段时间 -1 了
    for (int idx = 1; idx <= n - 1; idx++) {
        // 每一站公交车的到站时间
        for (int i = 1; i <= n; i++) {
            bus[i] = max(bus[i - 1], last[i - 1]) + dis[i - 1];
            if (i - 1 == idx &&
                dis[i - 1] != 0) {  // 如果上 - 个点到这个点用了加速器
                bus[i]--;
            }
        }
        ans = 0;
        // 计算所有人消耗的旅行时间
        for (int i = 1; i <= m; i++) {
            ans += bus[a[i].e] - a[i].t;
        }
        mi = min(mi, ans);
    }
    cout << mi;
}
int main() {
    scanf("%d%d%d", &n, &m, &k);
    for (int i = 1; i <= n - 1; i++) {
        scanf("%d", &dis[i]);  // dis[i] ：第 i 站 -> 第 i+1 站的时间
    }
    for (int i = 1; i <= m; i++) {  // m 个人的数据
        scanf("%d%d%d", &a[i].t, &a[i].s, &a[i].e);
        sum[a[i].e]++;                             // 计算到每一站的人数
        last[a[i].s] = max(last[a[i].s], a[i].t);  // 每 - 站最后 - 个到的人
    }
    // 计算每一站下车人数的前缀和 , 每一站公交车的到站时间
    for (int i = 1; i <= n; i++) {
        sum[i] += sum[i - 1];
        bus[i] = max(bus[i - 1], last[i - 1]) + dis[i - 1];
    }
    // 计算所有人消耗的旅行时间
    for (int i = 1; i <= m; i++) {
        ans += bus[a[i].e] - a[i].t;
    }
    if (k == 0) {  // 特判没有加速器的情况 (10 分 )
        cout << ans;
        return 0;
    }
    // 特判 k=1 的情况 , 暴力的尝试将每段路程 -1 分钟后的总时间 (30 分 )
    if (k == 1) {
        get1();
        return 0;
    }
    // 逐个计算每个加速器的最佳使用位置
    while (k--) {
        // 第 n 站只能影响自己 , 第 n-1 站用了加速器 , 必然减少到第 n 站的时间
        c[n - 1] = n;
        for (int i = n - 2; i >= 1; i--) {
            // 开到下一站不需要等待 , 注意如果相等缩短后任需车等人
            // 比如下一站最晚的人 6 点到 , 车本来是 7 点到大家要等一个小时
            // 用加速器加速提前一个小时到 , 大家就少等了一个小时
            if (bus[i + 1] > last[i + 1]) {  // 人等车
                c[i] = c[i + 1];
            } else {
                c[i] = i + 1;
            }
        }
        // 计算每一站如果使用加速器将被影响的人数
        int ma = INT_MIN, pos;  // pos ：在哪一站使用加速器
        for (int i = 1; i < n; i++) {
            if (dis[i] > 0 && sum[c[i]] - sum[i] > ma) {
                ma = sum[c[i]] - sum[i];
                pos = i;
            }
        }
        ans -= ma;
        dis[pos]--;
        // 重新计算到站时间 pos+1
        for (int i = pos + 1; i <= n; i++) {
            bus[i] = max(bus[i - 1], last[i - 1]) + dis[i - 1];
        }
    }
    printf("%d", ans);
    return 0;
}
