#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

const int MAXN = 1000000;

// Segment tree for range maximum query with lazy propagation
int tree[4 * MAXN];
int lazy[4 * MAXN];

void push(int node) {
    if (lazy[node]) {
        tree[node] += lazy[node];
        if (node * 2 < 4 * MAXN) {
            lazy[node * 2] += lazy[node];
        }
        if (node * 2 + 1 < 4 * MAXN) {
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

int main() {
    int x, k, p;
    scanf("%d %d %d", &x, &k, &p);
    
    // Process each booking request
    for (int i = 0; i < p; i++) {
        int u, v, n;
        scanf("%d %d %d", &u, &v, &n);
        
        // Convert to 0-indexed and adjust for segments
        u--; v--;
        
        // Check if the journey is valid
        if (v <= u) {
            printf("N\n");
            continue;
        }
        
        // Query the maximum occupancy in the range [u, v-1]
        int max_occupancy = query_range(1, 0, x - 1, u, v - 1);
        
        // Check if we can book n seats
        if (max_occupancy + n <= k) {
            // Update the occupancy for the range [u, v-1]
            update_range(1, 0, x - 1, u, v - 1, n);
            printf("T\n");
        } else {
            printf("N\n");
        }
    }
    
    return 0;
}