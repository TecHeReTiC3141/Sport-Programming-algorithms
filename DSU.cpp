int roots;
struct DSU {

    vector<int> p;

    vector<int> rang;

    explicit DSU(int n = 10) {
        p.resize(n);
        for (int i = 0; i < n; ++i) {
            p[i] = i;
        }
        rang.resize(n, 1);
    }

    int root(int x) {
        if (p[x] == x) return x;
        return p[x] = root(p[x]);
    }

    void join(int j, int k) {
        j = root(j);
        k = root(k);

        if (rang[j] < rang[k]) {
            p[j] = k;
            rang[k] += rang[j];
        } else {
            p[k] = j;
            rang[j] += rang[k];
        }
    }

    bool check(int x, int y) {
        return root(x) == root(y);
    }
};