#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 10;
struct node {
    int l, r;  // 区间管理范围
    // mi ：区间内的 a[i] 的最小值
    // cnt ：区间内的修改次数的和 ( 本题不修改 a[i], 修改的是 cnt)
    int mi, cnt, add;
} tr[N << 2];
// 将右端点相同的问题构建为单向链表
struct node2 {
    int l, r;  // 该问题对应的左右端点
    int idx;   // 第几个问题
    int next;  // 和当前问题右端点相同的前一个问题的位置
} que[N];
int head[N];  // 链表的表头
int n, m;
// last[] ：每个数最后一次的位置 ,p[i] ：在第 i 个数左侧和第 a[i]
// 值相同的最后一个数的位置
int a[N], last[N], p[N];
int ans[N];  // 每个问题的答案
void pushup(int k) {
    tr[k].mi = min(tr[k << 1].mi, tr[k << 1 | 1].mi);  // 区间最小值
    tr[k].cnt = tr[k << 1].cnt + tr[k << 1 | 1].cnt;   // 区间修改次数的和
}
// 建树
void build(int k, int l, int r) {
    tr[k].l = l, tr[k].r = r;
    if (l == r) {
        tr[k].mi = a[l];  // 区间最小值
        tr[k].cnt = 0;    // 默认修改次数为 0
        return;
    }
    int mid = (l + r) >> 1;
    build(k << 1, l, mid);
    build(k << 1 | 1, mid + 1, r);
    pushup(k);
}
// 修改某个结点的值
void change(int k, int v) {  // 注意：本题 a[i] 不变 , 因此 tr[k].mi 不变
    tr[k].cnt += v * (tr[k].r - tr[k].l + 1);
    tr[k].add += v;  // 懒标记也加上这个值
}
// 标记下放
void pushdown(int k) {
    if (tr[k].add) {  // 如果有懒标记
        change(k << 1, tr[k].add);
        change(k << 1 | 1, tr[k].add);
        tr[k].add = 0;
    }
}
// 区间修改：修改区间修改次数
void update(int k, int x, int y, int v) {
    int l = tr[k].l, r = tr[k].r;
    if (l >= x && r <= y) {
        change(k, v);  // 修改结点
        return;
    }
    pushdown(k);  // 标记下放
    int mid = (l + r) >> 1;
    if (x <= mid)
        update(k << 1, x, y, v);
    if (y > mid)
        update(k << 1 | 1, x, y, v);
    pushup(k);
}
// 单点查询：查询修改次数
int query(int k, int p) {
    int l = tr[k].l, r = tr[k].r;
    if (l == r)
        return tr[k].cnt;
    pushdown(k);  // 标记下放
    int mid = (l + r) >> 1;
    if (p <= mid)
        return query(k << 1, p);
    else
        return query(k << 1 | 1, p);
}
// 区间查询：查询区间最小值
int query_min(int k, int x, int y) {
    int l = tr[k].l, r = tr[k].r;
    if (l >= x && r <= y)
        return tr[k].mi;
    pushdown(k);  // 标记下放
    int mid = (l + r) >> 1;
    int res = INT_MAX;
    if (x <= mid)
        res = min(res, query_min(k << 1, x, y));
    if (y > mid)
        res = min(res, query_min(k << 1 | 1, x, y));
    return res;
}
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &a[i]);
        // 求和 a[i] 相同的数在第 i 个数的左侧最后出现的位置
        p[i] = last[a[i]];
        last[a[i]] = i;
    }
    build(1, 1, n);  // 建树
    // q 个问题
    int l, r;
    for (int i = 1; i <= m; ++i) {
        scanf("%d%d", &l, &r);
        que[i] = {l, r, i,
                  head[r]};  // 记录每个问题的左右端点 , 以及是第几个问题
        head[r] = i;         // 更新右端点为 r 的最后一个问题的位置
    }
    // 每次往后移动一个右端点
    for (int i = 1; i <= n; ++i) {
        // 求 [p[i]+1,i-1] 范围内是否有值 <a[i]
        // FIXME: 疑似错误题解：未检查区间是否为空
        if (query_min(1, p[i] + 1, i - 1) < a[i])
            update(1, 1, i, 1);
        else
            update(1, p[i] + 1, i, 1);
        // 处理右端点为 i 的每个询问
        for (int j = head[i]; j; j = que[j].next) {
            ans[que[j].idx] = query(1, que[j].l);
        }
    }
    // 输出答案
    for (int i = 1; i <= m; ++i) {
        printf("%d\n", ans[i]);
    }
    return 0;
}
