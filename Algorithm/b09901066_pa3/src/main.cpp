#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include "../lib/tm_usage.h"

using namespace std;

enum Color
{
    WHITE = 0,
    GRAY = 1,
    BLACK = 2
};

int **w;       // adjacency matrix
int **w_undir; // adjacency matrix (undirected version)
int **edge;
bool **tree; // record if the edge is used. tree[x][y] means x->y
// bool **check; // check if the ans is correct. tree[x][y] means x->y
int *pos;
vector<vector<int> > adj;
int heapSize = 0;
int n = 0, m = 0;

class Vertex
{
public:
    Vertex(int _self)
    {
        self = _self;
        key = INT_MIN;
        pi = -1;
        color = WHITE;
        visited = false;
    }
    int key, pi, self;
    bool visited;
    Color color;
};
vector<Vertex> V;

void printTree()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << tree[i][j] << " ";
        cout << endl;
    }
}
void MaxHeapify(vector<Vertex> &data, int root)
{
    int left = 2 * root + 1;
    int right = 2 * root + 2;
    int largest = root;
    if (left < heapSize && data[left].key > data[root].key)
        largest = left;
    if (right < heapSize && data[right].key > data[largest].key)
        largest = right;
    if (largest != root)
    {
        // cout<<"root "<<root<<" "<<" largest "<<largest<<endl;
        swap(data[root], data[largest]);
        swap(pos[data[root].self], pos[data[largest].self]);
        MaxHeapify(data, largest);
    }
}
void BuildMaxHeap(vector<Vertex> &data)
{
    heapSize = data.size(); // initialize heap size
    for (int i = (heapSize - 1) / 2; i >= 0; i--)
        MaxHeapify(data, i);
}
Vertex ExtractMax(vector<Vertex> &data, vector<Vertex> &res)
{
    Vertex max = data[0];
    data[0].visited = true;
    res.push_back(max);
    // cout<<res[res.size()-1].self<<endl;
    // data.erase(data.begin());

    int s = heapSize;
    swap(data[0], data[s - 1]);
    swap(pos[data[0].self], pos[data[s - 1].self]);
    heapSize--;
    MaxHeapify(data, 0);

    return max;
}
void GoUp(vector<Vertex> &Q, int idx)
{
    int parent = idx / 2;
    if (parent >= 0 && Q[parent].key < Q[idx].key)
    {
        swap(Q[parent], Q[idx]);
        swap(pos[Q[parent].self], pos[Q[idx].self]);
        GoUp(Q, parent);
    }
}
void Prim(vector<Vertex> &Q, vector<Vertex> &res)
{
    heapSize = n;
    BuildMaxHeap(Q);
    while (heapSize != 0)
    {
        Vertex max = ExtractMax(Q, res);
        int u = max.self;
        int s = adj[u].size();
        for (int i = 0; i < s; i++)
        {
            int v = adj[u][i]; // v implies "self"
            int p = pos[v];
            if (!Q[p].visited && p < heapSize && w_undir[u][v] > Q[p].key)
            {
                Q[p].pi = u;
                Q[p].key = w_undir[u][v];
                GoUp(Q, p);
            }
        }
    }
}

bool DFS_Visit(int k, int color[])
{
    color[k] = GRAY;
    for (int i = 0; i < n; i++)
    {
        if (tree[k][i])
        {
            if (color[i] == GRAY)
                return true;
            if (color[i] == WHITE && DFS_Visit(i, color))
                return true;
        }
    }
    color[k] = BLACK;
    return false;
}
bool DFS()
{
    int *color = new int[n];
    for (int i = 0; i < n; i++)
        color[i] = WHITE;

    for (int i = 0; i < n; i++)
        if (color[i] == WHITE)
            if (DFS_Visit(i, color) == true)
                return true;

    return false;
}

void Greedy()
{
    // for(int i=0; i<m; i++)
    //     cout<<edge[i][2]<<" ";
    // cout<<endl;
    for (int i = m-1; i >=0; i--)
    {
        if (edge[i][2] <= 0)
            return;
        if (!tree[edge[i][0]][edge[i][1]])
        {
            tree[edge[i][0]][edge[i][1]] = true;
            if (DFS()) // check cycle
                tree[edge[i][0]][edge[i][1]] = false;
            // printTree();
        }
    }
}

int main(int argc, char *argv[])
{
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;

    ////////// read input //////////
    char dir[200]; // direction
    char v[200];   // num of vertices
    char e[200];   // num of edges
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);
    fin.getline(dir, 200);
    fin.getline(v, 200);
    fin.getline(e, 200);

    n = atoi(v);
    m = atoi(e);
    int num;
    vector<int> data;
    while (fin >> num)
        data.push_back(num);

    ////////// initialization //////////
    adj.resize(n);
    w = new int *[n];
    w_undir = new int *[n];
    edge = new int *[m];
    tree = new bool *[n];
    pos = new int[n];
    // check = new bool *[n];
    for (int j = 0; j < n; j++)
    {
        w[j] = new int[n];
        w_undir[j] = new int[n];
        tree[j] = new bool[n];
        // check[j] = new bool[n];
    }
    for (int j = 0; j < m; j++)
        edge[j] = new int[3];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
        {
            w[i][j] = INT_MIN;
            w_undir[i][j] = INT_MIN;
            tree[i][j] = false;
            // check[i][j] = false;
        }
    for (int i = 0; i < m; i++)
    {
        edge[i][0] = data[i * 3];
        edge[i][1] = data[i * 3 + 1];
        edge[i][2] = data[i * 3 + 2];

        w[edge[i][0]][edge[i][1]] = edge[i][2];
        adj[edge[i][0]].push_back(edge[i][1]);
        adj[edge[i][1]].push_back(edge[i][0]);
        w_undir[edge[i][0]][edge[i][1]] = edge[i][2];
        w_undir[edge[i][1]][edge[i][0]] = edge[i][2];
    }
    // for(int i=0; i<n; i++){
    //     for(int j=0; j<adj[i].size(); j++)
    //         cout<<adj[i][j]<<" ";
    //     cout << endl;
    // }
    //////////sort edge by the weight from high to low//////////
    int key, i;

    if (dir[0] == 'd')
        for (int j = 1; j < m; j++)
        {
            key = edge[j][2];
            i = j - 1;
            while (edge[i][2] > key && i > 0)
            {
                swap(edge[i + 1], edge[i]);
                i--;
            }
        }
    //////////Prim's//////////
    vector<Vertex> Q;
    vector<Vertex> res;
    for (int i = 0; i < n; i++)
    {
        Vertex u(i);
        Q.push_back(u);
        pos[i] = i;
    }
    Q[0].key = 0;
    Q[0].visited = true;
    Prim(Q, res);
    // cout<<res.size()<<endl;
    // for(int i=0; i<res.size(); i++)
    //     cout<<res[i].self<<endl;

    //////////record if the edge is used//////////
    for (int i = 1; i < n; i++)
    {
        int y = res[i].self;
        int x = res[i].pi;
        // cout << "x:" << x << " y:" << y << endl;
        // cout<<w[x][y]<<endl;
        if (w[x][y] > INT_MIN)
            tree[x][y] = true;
        if (w[y][x] > INT_MIN && w[x][y] < w[y][x])
        {
            tree[x][y] = false;
            tree[y][x] = true;
        }
        if (dir[0] == 'u')
            tree[y][x] = true;
    }
    // printTree();

    //////////Greedy//////////
    if (dir[0] == 'd')
        Greedy();

    //////////fout result//////////
    // cout << "Does the tree contain cycles? " << DFS() << endl;
    // printTree();

    long long int total = 0;
    for (int i = 0; i < m; i++)
        if (!tree[edge[i][0]][edge[i][1]])
            total += edge[i][2];
    fout << total << endl;
    cout << "total: " << total << endl;

    int num_remove = 0;
    for (int i = 0; i < m; i++)
        if (!tree[edge[i][0]][edge[i][1]])
        {
            fout << edge[i][0] << " " << edge[i][1] << " " << edge[i][2] << endl;
            num_remove++;
        }
    // else
    // check[edge[i][0]][edge[i][1]]=true;

    cout << "remove: " << num_remove << endl;
    // cout<<DFS_Check()<<endl;

    for (int i = 0; i < n; i++)
        delete[] w[i];
    delete[] w;

    tmusg.getPeriodUsage(stat);
    cout << "The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms" << endl;

    fin.close();
    fout.close();
    return 0;
}