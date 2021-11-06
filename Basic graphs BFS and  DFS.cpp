bool bfs(int s, vector<vector<int>> &a, vector<int> &dols, vector<bool> &used) {
    queue<int> q;
    q.push(s);
    used[s] = true;
    dols[s] = 1;
    while (!q.empty()) {
        int next = q.front();
        q.pop();
        for (int i : a[next]) {
            if (!used[i]) {
                dols[i] = dols[next] == 1 ? 2 : 1;
                q.push(i);
                used[i] = true;
            } else {
                if (dols[i] == dols[next]) return false;
            }
        }
    }
    return true;
}

bool cycle = false;
void dfs(int v, vector<vector<int>> &gr, vector<int> &colors) {
    for (int u : gr[v]) {
        if (!colors[u]) {
            colors[u] = 3 - colors[v];
            dfs(u, gr, colors);
        }
        else if (colors[u] == colors[v]) {
            cycle = true;
        }
    }

}