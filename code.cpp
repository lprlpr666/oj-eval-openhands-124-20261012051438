#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class SegmentTree {
private:
    vector<int> tree;  // stores maximum value in each segment
    vector<int> lazy;  // lazy propagation for range updates
    int n;

    void push(int node, int left, int right) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node];
            if (left != right) {
                lazy[node * 2] += lazy[node];
                lazy[node * 2 + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void update_range(int node, int left, int right, int l, int r, int val) {
        push(node, left, right);
        if (r < left || right < l) return;
        if (l <= left && right <= r) {
            lazy[node] += val;
            push(node, left, right);
            return;
        }
        int mid = (left + right) / 2;
        update_range(node * 2, left, mid, l, r, val);
        update_range(node * 2 + 1, mid + 1, right, l, r, val);
        tree[node] = max(tree[node * 2], tree[node * 2 + 1]);
    }

    int query_range(int node, int left, int right, int l, int r) {
        push(node, left, right);
        if (r < left || right < l) return INT_MIN;
        if (l <= left && right <= r) return tree[node];
        int mid = (left + right) / 2;
        int left_val = query_range(node * 2, left, mid, l, r);
        int right_val = query_range(node * 2 + 1, mid + 1, right, l, r);
        return max(left_val, right_val);
    }

public:
    SegmentTree(int size) {
        n = size;
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }

    void update(int l, int r, int val) {
        update_range(1, 0, n - 1, l, r, val);
    }

    int query(int l, int r) {
        return query_range(1, 0, n - 1, l, r);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int x, k, p;
    cin >> x >> k >> p;

    // Create segment tree for x-1 segments (from station 1 to x-1)
    SegmentTree st(x);

    for (int i = 0; i < p; i++) {
        int u, v, n;
        cin >> u >> v >> n;
        u--; v--; // Convert to 0-indexed

        // Check if we can book n seats from u to v-1
        // We need to check the maximum occupancy in range [u, v-1]
        if (v > u) {  // Only if there's a valid journey
            int max_occupancy = st.query(u, v - 1);
            if (max_occupancy + n <= k) {
                // Can book - update the occupancy
                st.update(u, v - 1, n);
                cout << "T\n";
            } else {
                cout << "N\n";
            }
        } else {
            // Invalid journey (v <= u)
            cout << "N\n";
        }
    }

    return 0;
}