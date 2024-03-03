#include "DisjointSet.h"

DisjointSet::DisjointSet(int n) : parent(n + 1), rank(n + 1, 0), numSets(n), final(false) {
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
    }
}

void DisjointSet::make_set(int i) {
    if (final) return;
    if (i >= parent.size()) {
        parent.resize(i + 1, 0);
        rank.resize(i + 1, 0);
    }
    parent[i] = i;
    rank[i] = 0;
}

int DisjointSet::find_set(int i) {
    if (i == parent[i]) return i;
    return parent[i] = find_set(parent[i]);
}

void DisjointSet::union_sets(int i, int j) {
    if (final) return;
    int rootI = find_set(i);
    int rootJ = find_set(j);
    if (rootI == rootJ) return;
    parent[rootJ] = rootI;
    --numSets;
}

int DisjointSet::final_sets() {
    final = false;
    return numSets;
}
