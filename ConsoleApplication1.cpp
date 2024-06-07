#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// Graph class
class Graph {
public:
    int V; // Number of vertices
    vector<vector<int>> adj; // Adjacency list

    Graph(int V);//this is the test graph
    void addEdge(int v, int w);
    bool is2VertexStronglyBiconnected();
private:
    void dfs(int v, vector<bool>& visited, stack<int>& st);
    void fillOrder(int v, vector<bool>& visited, stack<int>& st);
    Graph getTranspose();
    void fillSCCs(int v, vector<bool>& visited, vector<int>& scc);
};

Graph::Graph(int V) {
    this->V = V;
    adj.resize(V);
}

void Graph::addEdge(int v, int w) {
    adj[v].push_back(w);
}

Graph Graph::getTranspose() {
    Graph gT(V);
    for (int v = 0; v < V; ++v) {
        for (int w : adj[v]) {
            gT.addEdge(w, v);
        }
    }
    return gT;
}

void Graph::dfs(int v, vector<bool>& visited, stack<int>& st) {
    visited[v] = true;
    for (int w : adj[v]) {
        if (!visited[w]) {
            dfs(w, visited, st);
        }
    }
    st.push(v);
}

void Graph::fillOrder(int v, vector<bool>& visited, stack<int>& st) {
    visited[v] = true;
    for (int w : adj[v]) {
        if (!visited[w]) {
            fillOrder(w, visited, st);
        }
    }
    st.push(v);
}
//this is jens_schmidt algorithm
void Graph::fillSCCs(int v, vector<bool>& visited, vector<int>& scc) {
    visited[v] = true;
    scc.push_back(v);
    for (int w : adj[v]) {
        if (!visited[w]) {
            fillSCCs(w, visited, scc);
        }
    }
}

bool Graph::is2VertexStronglyBiconnected() {
    stack<int> st;
    vector<bool> visited(V, false);
    for (int v = 0; v < V; ++v) {
        if (!visited[v]) {
            fillOrder(v, visited, st);
        }
    }

    Graph gT = getTranspose();

    visited.assign(V, false);
    while (!st.empty()) {
        int v = st.top();
        st.pop();

        if (!visited[v]) {
            vector<int> scc;
            gT.fillSCCs(v, visited, scc);
            // Check if the strongly connected component (SCC) has more than 2 vertices
            if (scc.size() > 2) {
                return false;
            }
        }
    }

    return true;
}

int main() {
    //here we can use SNAP data for example social media data
    Graph g(6);
    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 3);

    if (g.is2VertexStronglyBiconnected()) {
        cout << "The graph is 2-vertex strongly biconnected." << endl;
    }
    else {
        cout << "The graph is not 2-vertex strongly biconnected." << endl;
    }

    return 0;
}
