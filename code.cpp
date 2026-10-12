#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

// Segment tree for range maximum query with lazy propagation
class SegmentTree {
private:
    vector<int> tree;
    vector<int> lazy;
    int n;

    void push(int node) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node];
            if (node * 2 + 1 < tree.size()) {
                lazy[node * 2] += lazy[node];
                lazy[node * 2 + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void update_range(int node, int node_left, int node_right, int l, int r, int val) {
        push(node);
        if (r < node_left || node_right < l) return;
        if (l <= node_left && node_right <= r) {
            lazy[node] += val;
            push(node);
            return;
        }
        int mid = (node_left + node_right) / 2;
        update_range(node * 2, node_left, mid, l, r, val);
        update_range(node * 2 + 1, mid + 1, node_right, l, r, val);
        tree[node] = max(tree[node * 2], tree[node * 2 + 1]);
    }

    int query_range(int node, int node_left, int node_right, int l, int r) {
        if (r < node_left || node_right < l) return 0;
        push(node);
        if (l <= node_left && node_right <= r) return tree[node];
        int mid = (node_left + node_right) / 2;
        int left_val = query_range(node * 2, node_left, mid, l, r);
        int right_val = query_range(node * 2 + 1, mid + 1, node_right, l, r);
        return max(left_val, right_val);
    }

public:
    SegmentTree(int size) {
        n = size;
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }

    void update(int l, int r, int val) {
        if (l <= r) {
            update_range(1, 0, n - 1, l, r, val);
        }
    }

    int query(int l, int r) {
        if (l > r) return 0;
        return query_range(1, 0, n - 1, l, r);
    }
};

int main() {
    int x, k, p;
    scanf("%d %d %d", &x, &k, &p);
    
    // Create segment tree for the given number of stations
    SegmentTree st(x);
    
    // Process each booking request
    for (int i = 0; i < p; i++) {
        int u, v, n;
        scanf("%d %d %d", &u, &v, &n);
        
        // Convert to 0-indexed
        u--; v--;
        
        // Check if the journey is valid
        if (v <= u) {
            printf("N\n");
            continue;
        }
        
        // Query the maximum occupancy in the range [u, v-1]
        int max_occupancy = st.query(u, v - 1);
        
        // Check if we can book n seats
        if (max_occupancy + n <= k) {
            // Update the occupancy for the range [u, v-1]
            st.update(u, v - 1, n);
            printf("T\n");
        } else {
            printf("N\n");
        }
    }
    
    return 0;
}