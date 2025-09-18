#include <bits/stdc++.h>
using namespace std;

const int N = 1e6 + 10;
struct node {
    int d, v, a;
} a[N];  // 每辆车的信息
struct range {
    int b, e;
} b[N];    // 每辆车超速被从第几个监控到第几个监控拍到了
int p[N];  // 监控的信息
int n, m, len, v;  // n辆车,m个监控,路长1en,限速v
int cnt, res, T;
int pf(int x) {
    return x * x;
}
// di：初始位置,vi：初始速度,ai：加速度,ed：结束位置
// 判读到结束位置是否超速
bool check(int di, int vi, int ai, int ed) {
    return pf(vi) + 2 * ai * (ed - di) > pf(v);
}
// 找到监控到第i辆车超速的第1个监控和最后一个监控
pair<int, int> find(int di, int vi, int ai) {
    int t = lower_bound(p + 1, p + m + 1, di) - p;  // 找到>=di位置的第1个监控
    if (ai > 0) {                                   // 加速
        // 二分出第1个超速的pi位置
        int l = t, r = m, mid;
        while (l <= r) {
            mid = l + r >> 1;
            if (check(di, vi, ai, p[mid]))
                r = mid - 1;  // 只要超速就向左找
            else
                l = mid + 1;
        }
        return {l, m};
    }
    if (ai < 0) {  // 减速
        // 二分出最后一个超速的pi位置
        int l = t, r = m, mid;
        while (l <= r) {
            mid = l + r >> 1;
            if (check(di, vi, ai, p[mid]))
                l = mid + 1;  // 只要超速就向右找
            else
                r = mid - 1;
        }
        return {t, l - 1};
    }
}
// 按右端点升序,保证先出现被包含的区间
bool cmp(range r1, range r2) {
    return r1.e < r2.e;
}
int solve() {  // 计算最多可以删除多少个监控
    sort(b + 1, b + cnt + 1, cmp);
    // 按右端点升序
    int res = 0, last = -1;
    for (int i = 1; i <= cnt; i++) {
        if (b[i].b > last) {
            res++;
            last = b[i].e;
        }
    }
    return m - res;
}
int main() {
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d%d", &n, &m, &len, &v);
        // 读入每辆车的信息
        for (int i = 1; i <= n; i++) {
            scanf("%d%d%d", &a[i].d, &a[i].v, &a[i].a);
        }
        // 读入监控位置
        for (int i = 1; i <= m; i++)
            scanf("%d", &p[i]);
        // 计算有几辆车超速
        cnt = 0;
        int c0 = 0, c1 = 0;  // c0：匀速,c1：加速
        for (int i = 1; i <= n; i++) {
            if (a[i].a == 0)
                c0++;
            else if (a[i].a > 0)
                c1++;
            if (a[i].d > p[m])
                continue;  // 没有监控能拍到
            // 特判特殊性质A：匀速的情况
            if (a[i].v > v && a[i].a == 0) {
                int st = lower_bound(p + 1, p + m + 1, a[i].d) - p;
                b[++cnt] = {st, m};
                continue;
            }
            if (a[i].v <= v && a[i].a <= 0)
                continue;  // 初始速度<=超速速度,且匀速或者减速
            // 加速行驶,到了最后一个测试点,也没有超速
            if (a[i].a > 0 && !check(a[i].d, a[i].v, a[i].a, p[m]))
                continue;
            // 找到>=a[i].d的第1个测速点
            int t = lower_bound(p + 1, p + m + 1, a[i].d) - p;
            // 减速行驶,到了>=a[i].d后面第1个测速点,没有超速
            if (a[i].a < 0 && !check(a[i].d, a[i].v, a[i].a, p[t]))
                continue;
            pair<int, int> pos = find(
                a[i].d, a[i].v, a[i].a);  // 找到监控到超速的第1个点和最后一个点
            // cout<<i<<","<<pos.first<<""<<pos.second<<endl;
            cnt++;
            b[cnt] = {pos.first, pos.second};
        }
        // 匀速、加速的情况,可以关闭监控的数量为㎡-1或m,只要开最后一个
        if (c0 == n || c1 == n) {
            if (cnt == 0)
                res = m;  // 没有车超速,都关了
            else
                res = m - 1;  // 有车超速,开最后一个
        } else
            res = solve();  // 计算最多可以删除多少个监控
        printf("%d %d\n", cnt, res);
    }
    return 0;
}
