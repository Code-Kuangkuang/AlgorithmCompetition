#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int N = 1e5 + 10;
const int INF = 1e9;

struct SegmentTree {
    int n;
    vector<ll> tree;
    vector<ll> lazy;

    SegmentTree(int n = 0) {
        init(n);
    }
    
    void init(int n_) {
        n = n_;
        tree.assign(4 * n, 0);
        lazy.assign(4 * n, 0);
    }

    // build Tree
    void build(int u, int l, int r, const vector<ll> &a) {
        if (l == r) {
            tree[u] = a[l];
            return ;
        } else {
            int mid = (l + r) >> 1;
            build(u << 1, l, mid, a);
            build(u << 1 | 1, mid + 1, r, a);
            tree[u] = tree[u << 1] + tree[u << 1 | 1];
        }
    }

    // apply the interval increment to the current node;
    void apply(int u, int l, int r, ll val) {
        tree[u] += val * (r - l + 1);
        lazy[u] += val;
    }

    // pushdown the lazy tag
    void push_down(int u, int l, int r) {
        if (lazy[u] == 0)   return ;
        int mid = (l + r) >> 1;
        ll tag = lazy[u];
        apply(u << 1, l, mid, tag);
        apply(u << 1 | 1, mid + 1, r, tag);
        lazy[u] = 0;
    }

    // add value to the interval 
    void range_add(int u, int l, int r, int L, int R, ll val) {
        if (R < l || r < L) {
            return ;
        }
        if (L <= l && r <= R) {
            apply(u, l, r, val);
            return ;
        }
        push_down(u, l, r);
        int mid = (l + r) >> 1;
        if (L <= mid)   range_add(u << 1, l, mid, L, R, val);
        if (R > mid)    range_add(u << 1 | 1, mid + 1, r, L, R, val);
         tree[u] = tree[u << 1] + tree[u << 1 | 1];
    }


    // update by a single value
    void update_singleValue(int u, int l, int r, int pos, ll val) {
        if (l == r) {
            tree[u] = val;
            lazy[u] = 0;
            return ;
        }
        push_down(u, l, r);
        int mid = (l + r) >> 1;
        if (pos <= mid) update_singleValue(u << 1, l, mid, pos, val);
        else    update_singleValue(u << 1 | 1, mid + 1, r, pos, val);
        tree[u] = tree[u << 1] + tree[u << 1 | 1];
    }

    // query by the interval of [L,R]
    ll range_query(int u, int l, int r, int L, int R) {
        if (R < l || L > r) { // Not included at all
            return 0;
        }
        if (L <= l && r <= R) { // Completely included
            return tree[u];
        }
        push_down(u, l, r);
        int mid = (l + r) >> 1;
        ll res = 0;
        if (L <= mid) res += range_query(u << 1, l, mid, L, R);
        if (R >  mid) res += range_query(u << 1 | 1, mid + 1, r, L, R);
        return res;
    }

};


int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    int n, m;
    cin >> n >> m;
    vector<ll> a(n + 1);
    for (int i = 1; i <= n; i ++) {
        cin >> a[i];
    }
    SegmentTree st(n);
    st.build(1, 1, n, a);
    while(m --) {
        int op;
        cin >> op;
        if (op == 1) {
            int x, y;
            ll k;
            cin >> x >> y >> k;
            st.range_add(1, 1, n, x, y, k);
        } else if (op == 2){
            int x, y;
            cin >> x >> y;
            cout << st.range_query(1, 1, n, x, y) << endl;
        } else if (op == 3) {
            int pos;
            ll k;
            cin >> pos >> k;
            st.update_singleValue(1, 1, n, pos, k);
        }
    }
    return 0;
}