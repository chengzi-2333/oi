// {P5490}
#include <bits/stdc++.h>

using namespace std;

// discrete
vector<int> disc(1);

inline int pos(int val) {
    return lower_bound(disc.begin(), disc.end(), val)-disc.begin();
}

inline void discrete() {
    sort(disc.begin()+1, disc.end());
    disc.erase(unique(disc.begin()+1, disc.end()), disc.end());
}

// segment tree
vector<int> cnt, len;

inline int lc(int p) {return p << 1;}
inline int rc(int p) {return p << 1 | 1;}
template<typename M>
inline M middle(M l, M r) {return l+((r-l)>>1);};

void pushup(int p, int l, int r) {
    if (cnt[p]) len[p] = disc[r+1] - disc[l];
    else len[p] = (l == r) ? 0 : len[lc(p)] + len[rc(p)];
}

void update(int s, int t, int k, int l, int r, int p) {
    if (s <= l && t >= r) return cnt[p] += k, pushup(p, l, r);
    auto mid = middle(l, r);
    if (s <= mid) update(s, t, k, l, mid, lc(p));
    if (t > mid) update(s, t, k, mid + 1, r, rc(p));
    pushup(p, l, r);
}

inline void update(int s, int t, int k) {
    update(s, t, k, 1, disc.size()-2, 1);
}


signed main() {
#ifndef ONLINE_JUDGE
    freopen("scanning-area.in", "r", stdin);
#endif
    int n; cin >> n;
    vector<tuple<int, int, int, int>> edge;
    for (int xa, ya, xb, yb; n; n--) {
        cin >> xa >> ya >> xb >> yb;
        edge.emplace_back(ya, xa, xb, 1);
        edge.emplace_back(yb, xa, xb, -1);
        disc.push_back(xa), disc.push_back(xb);
    }

    sort(edge.begin(), edge.end());
    discrete();
    
    cnt.resize(disc.size()*4), len.resize(disc.size()*4);
    long long py = get<0>(edge[0]), sum = 0;
    for (const auto& [y, xa, xb, w]: edge) {
        sum += (y - py) * len[1];
        py = y;
        update(pos(xa), pos(xb)-1, w);
    }
    cout << sum << endl;
}
