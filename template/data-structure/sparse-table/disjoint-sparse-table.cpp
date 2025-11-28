// {U300099}
// TODO: translate and rewrite
#include <bits/stdc++.h>

/*! T is the type of the elements
 * Monoid is the operation functor type
 * identity is the identity element of the Monoid (e.g. 0 for addition and inf
 * for minimum)
 */
// T为操作数类型，Monoid为支持结合律且有单位元的运算，identity为此运算单位元
template <typename T, typename Monoid, T identity>
class DisjointSparseTable {
   public:
    explicit DisjointSparseTable(std::vector<T> arr) {
        // Find the highest cnt such that pow2 = 2^cnt >= x
        // 齐次化处理，使初始数组大小为2的幂次
        int pow2 = 1, cnt = 0;
        for (; pow2 < (int)arr.size(); pow2 *= 2, ++cnt);

        arr.resize(pow2, identity);
        sum.resize(cnt, std::vector<T>(pow2));

        for (int level = 0; level < (int)sum.size(); ++level) {
            for (int block = 0; block < 1 << level; ++block) {
                // The first half of the block contains suffix sums,
                // the second half contains prefix sums
                const auto start = block << (sum.size() - level);
                const auto end = (block + 1) << (sum.size() - level);
                const auto middle = (end + start) / 2;

                auto val = arr[middle];
                sum[level][middle] = val;
                for (int x = middle + 1; x < end; ++x) {
                    val = operate(arr[x], val);
                    sum[level][x] = val;
                }

                val = arr[middle - 1];
                sum[level][middle - 1] = val;
                for (int x = middle - 2; x >= start; --x) {
                    val = operate(arr[x], val);
                    sum[level][x] = val;
                }
            }
        }
    }

    /*! Returns Monoid sum over range [l, r)*/
    T query(int l, int r) const {
        assert(l < r);
        // Convert half open interval to closed interval
        --r;
        if (r == l - 1) {
            return identity;
        }
        if (l == r) {
            return sum.back()[l];
        }
        // Position of the leftmost different bit from the right
        const auto pos_diff =
            (sizeof(long long) * CHAR_BIT) - 1 - __builtin_clzll(l ^ r);
        const auto level = sum.size() - 1 - pos_diff;
        return operate(sum[level][l], sum[level][r]);
    }

   private:
    std::vector<std::vector<T>> sum;
    Monoid operate;
};

template <typename F>
void test() {
    // Tests the DisjointSparseTable
    std::vector<int> data{6, 2, 4, 1, 7, 3, 4, 2, 7, 2, 4, 1, 6};
    DisjointSparseTable<int, F, 0> sp{data};
    for (size_t start = 0; start < data.size(); ++start) {
        for (size_t end = start + 1; end <= data.size(); ++end) {
            assert(sp.query(start, end) ==
                   accumulate(begin(data) + start, begin(data) + end, 0, F()));
        }
    }
}

constexpr long long MOD = 1e9 + 7;
int n, q, ans;
std::vector<long long> a;

struct op {
    long long operator()(long long a, long long b) const {
        return ((a % MOD) * (b % MOD)) % MOD;
    };
};

int main() {
#ifndef ONLINE_JUDGE
    freopen("disjoint-sparse-table.in", "r", stdin);
#endif  // ONLINE_JUDGE
    std::cin.tie(nullptr)->sync_with_stdio(false);
    std::cin >> n >> q;
    std::copy_n(std::istream_iterator<long long>(std::cin), n,
                std::back_insert_iterator<std::vector<long long>>(a));
    DisjointSparseTable<long long, op, 1> sp{a};
    for (int l, r; q; q--) {
        std::cin >> l >> r;
        ans ^= sp.query(l - 1, r) % MOD;
    }
    std::cout << ans << std::endl;
}
