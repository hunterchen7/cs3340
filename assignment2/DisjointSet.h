#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <vector>

class DisjointSet {
private:
    std::vector<int> parent, rank;
    int numSets;
    bool final;

public:
    DisjointSet(int n);
    void make_set(int i);
    int find_set(int i);
    void union_sets(int i, int j);
    int final_sets();
};

#endif // DISJOINTSET_H
