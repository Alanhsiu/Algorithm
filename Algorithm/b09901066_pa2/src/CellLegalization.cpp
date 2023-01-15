#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include "../lib/tm_usage.h"

using namespace std;

vector<int> x_final;

int W, n;
vector<int> w, x, e;
vector<long long int> accumulatedWidth;

long long int Opt(int W, int n, long long int *old, long long int *cost)
{
    cout << "W = " << W << " "
         << "n = " << n << endl;

    for (int i = 0; i < W; i++)
        if (w[1] > i)
            old[i] = LLONG_MAX;
        else if (x[1] + w[1] > i)
            old[i] = e[1] * abs(x[1] - (i - w[1]));

    int acc = w[1];
    for (int i = 2; i <= n; i++)
    {
        acc = accumulatedWidth[i];
        for (int j = 1; j < acc; j++)
            cost[j] = LLONG_MAX;
        for (int j = acc; j <= W; j++)
            cost[j] = min(cost[j - 1], old[j - w[i]] + e[i] * abs(x[i] - (j - w[i])));
        for (int j = 1; j <= W; j++)
            old[j] = cost[j];
    }

    return cost[W];
}

int main(int argc, char *argv[])
{
    CommonNs::TmUsage tmusg;
    CommonNs::TmStat stat;
    long long int *old = new long long[600010]();
    long long int *cost = new long long[600010]();

    char W1[200];
    char n1[200];
    fstream fin(argv[1]);
    fstream fout;
    fout.open(argv[2], ios::out);
    fin.getline(W1, 200);
    fin.getline(n1, 200);

    W = atoi(W1);
    n = atoi(n1);

    int num;
    vector<int> data;
    while (fin >> num)
        data.push_back(num);

    w.push_back(0);
    x.push_back(0);
    e.push_back(0);

    for (int i = 0; i < n; i++)
    {
        w.push_back(data[i]);
        x.push_back(data[i + n]);
        e.push_back(data[i + 2 * n]);
    }

    // note that the index of accumulated width is from 0 to n, accumulated[n] means the sun of the previous n terms
    accumulatedWidth.push_back(0);
    for (int i = 0; i <= n; i++)
    {
        x_final.push_back(-1);
        accumulatedWidth.push_back(accumulatedWidth[i] + w[i]);
    }
    accumulatedWidth.erase(accumulatedWidth.begin());

    long long int res = Opt(W, n, old, cost);
    cout << "res = " << res << endl;
    fout << res << endl;

    // for(int i=0; i<=W; i++)
    //     cout << old[i] <<' ';
    // cout << endl;
    // for(int i=0; i<=W; i++)
    //     cout << cost[i] <<' ';
    // cout << endl;
    tmusg.getPeriodUsage(stat);
    cout << "The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms" << endl;

    int idx = n;
    int width = W;
    long long int curCost = 0;

    while (idx > 0)
    {
        while (cost[width] == cost[width - 1])
            width--;
        x_final[idx] = width - w[idx];
        // backtracking
        for (int j = width; j > w[idx]; j--)
        {
            curCost = e[idx] * abs(x[idx] - (j - w[idx]));
            old[j - w[idx]] = cost[j] - curCost;
            cost[j] = old[j];
        }
        width -= w[idx];
        idx--;
    }
    long long int total = 0;
    for (int i = 1; i <= n; i++)
        fout << x_final[i] << endl;

    tmusg.getPeriodUsage(stat);
    cout << "The total CPU time: " << (stat.uTime + stat.sTime) / 1000.0 << "ms" << endl;

    fin.close();
    fout.close();
    return 0;
}