#include <vector>
using namespace std;

vector<edge> MST_kres(vector<pair<int, int>> coords) {
    sort(edges.begin(), edges.end(), cmp);
    vector<edge> msu;
    ll res = 0.0;
    DSU dsu = DSU(n);
    for (edge e : edges) {
        int v = e.v, u = e.u;
        if (!dsu.check(v, u)) {
            dsu.join(v, u);
            msu.push_back(e);
            res += e.w;
        }
    }
    return msu;
}

double MST_primm(vector<pair<int, int>> &coords, vector<bool> &used) {
    //Ввод графа...

    double mst_weight = 0;     //Текущий вес остова.

    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> q;

    q.push({0.0, 0});     //Начнём с вершины 0.

    while (!q.empty()) {
        pair<double, int> c = q.top();
        q.pop();

        double dst = c.first; int v = c.second;

        if (used[v]) {      //вершина уже добавлена в остов
            continue;
        }
        used[v] = true;
        mst_weight += dst;

        for (int i = 0; i < coords.size(); ++i) {
            double len_vu = eucludian_dist(coords[i], coords[v]);

            if (!used[i]) {
                q.push({len_vu, i});    //Заметьте: мы учитываем только длину ребра.
            }
        }
    }

    return mst_weight;
}