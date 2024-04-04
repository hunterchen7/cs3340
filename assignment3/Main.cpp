#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

// Structure to represent an edge in the graph
struct Edge {
    int to;
    int weight;

    Edge(int t, int w) : to(t), weight(w) {}
};

class MinHeap {
public:
    vector<int> ids;
    vector<int> keys;
    vector<int> pos;

    // construct with keys and number of elements
    MinHeap(const vector<int>& keys, int n) : ids(n + 1), keys(keys), pos(n + 1, -1) {
        for (int i = 1; i <= n; ++i) {
            ids[i] = i;
            pos[i] = i;
        }
        buildHeap();
    }

    // check if element is in heap
    bool inHeap(int id) {
        return pos[id] != -1;
    }

    // check if heap is empty
    bool isEmpty() const {
        return ids.size() == 1; // Only the dummy element at 0 is present
    }

    // get minimum key
    int minKey() {
        return keys[ids[1]];
    }

    // get minimum id
    int minId() {
        return ids[1];
    }

    // deletes the element with min key from the heap
    void deleteMin() {
        swap(1, (int) (ids.size() - 1));
        pos[ids.back()] = -1;
        ids.pop_back();
        heapify(1);
    }

    // decrease key of element at id to newKey
    void decreaseKey(int id, int newKey) {
        keys[id] = newKey;
        bubbleUp(pos[id]);
    }

    int key(int id) {
        return keys[id];
    }

private:
    // build heap
    void buildHeap() {
        for (int i = (int) ids.size() / 2; i > 0; --i) {
            heapify(i);
        }
    }

    // heapify at index i
    void heapify(int i) {
        int left = 2 * i;
        int right = left + 1;
        int min = i;

        // find min key among i, left, right
        if (left < ids.size() && key(ids[left]) < key(ids[min])) min = left;
        if (right < ids.size() && key(ids[right]) < key(ids[min])) min = right;

        if (min != i) {
            swap(i, min);
            heapify(min);
        }
    }
    
    // bubble up at index i
    void bubbleUp(int i) {
        while (i > 1 && keys[ids[i]] < keys[ids[i / 2]]) {
            swap(i, i / 2);
            i = i / 2;
        }
    }
    
    // swap elements at index i,j
    void swap(int i, int j) {
        std::swap(ids[i], ids[j]);
        pos[ids[i]] = i;
        pos[ids[j]] = j;
    }
};

void readGraph(const string& filename, vector<vector<Edge>>& graph, vector<int>& keys) {
    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return;
    }

    // The first line of the input file contains an integer, n, indicating the number of vertices of the input graph.
    int n;
    file >> n;

    graph.resize(n + 1);
    keys.resize(n + 1, INT_MAX);

    // Each of the remaining lines contains a triple "i j w", where 1 ≤ i, j ≤ n, indicating an
    // edge between vertex i and vertex j with weight w.
    int i, j, w;
    while (file >> i >> j >> w) {
        graph[i].emplace_back(j, w);
        graph[j].emplace_back(i, w);
    }

    file.close();
}

void primMST(const vector<vector<Edge>>& graph) {
    int n = (int) graph.size() - 1;
    vector<int> parent(graph.size(), -1);
    vector<int> keys(graph.size(), INT_MAX);
    vector<bool> inMST(graph.size(), false);

    // The input graph in adjacency list representation: Print the adjacency lists in any
    // reasonable format. Print each edge with its weight.
    std::cout << "Input Graph (Adjacency List Representation): " << "\n";
    for (int i = 1; i <= n; ++i) {
        std::cout << "vertex " << i << ": ";
        for (const Edge& edge : graph[i]) {
            std::cout << "(" << edge.to << ", " << edge.weight << ")";
            // if not last print comma
            if (&edge != &graph[i].back()) std::cout << ", ";
        }
        std::cout << "\n";
    }

    MinHeap minHeap(keys, n);

    keys[1] = 0;
    minHeap.decreaseKey(1, 0);
    int totalWeight = 0;

    std::cout << "\nMinimum Spanning Tree: " << "\n";

    while (!minHeap.isEmpty()) {
        int u = minHeap.minId();
        minHeap.deleteMin();

        inMST[u] = true;

        for (const Edge& edge : graph[u]) {
            int v = edge.to;
            int weight = edge.weight;
            if (!inMST[v] && weight < minHeap.key(v)) {
                parent[v] = u;
                keys[v] = weight;
                if (minHeap.inHeap(v)) minHeap.decreaseKey(v, weight);
            }
        }

        // The minimum spanning tree: Print the edges of the minimum spanning tree, in the
        //  order in which they are produced by the Prim’s algorithm. For each minimum spanning
        //  tree edge (i, j), where i is the parent of j in the minimum spanning tree, print “(i, j) : w”
        //  in a separate line, where w is the weight of the edge {i, j}.
        if (parent[u] == -1) continue; // don't print root node since it has no parent
        std::cout << "(" << parent[u] << ", " << u << "): " << keys[u] << "\n";
        totalWeight += keys[u];
    }

    // The weight of the minimum spanning tree: Print the weight in a separate line with suitable description
    std::cout << "Total Weight of MST: " << totalWeight << "\n";
}

int main() {
    string filename = "assignment3/mst_graph_medium.txt";

    vector<vector<Edge>> graph;
    vector<int> keys;

    readGraph(filename, graph, keys);

    primMST(graph);

    return 0;
}
