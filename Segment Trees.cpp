#include <vector>
using namespace std;

truct SegmentTree {
        vector<int> segment_tree, a;
        int N = 1;

        SegmentTree(int n, vector<int> &v) {
            segment_tree.resize(4 * MAXN);

            while (N * 2 < n) N *= 2;
            N *= 2;
            a.resize(N);
            for (int i = 0; i < n; ++i) a[i] = v[i];

            for (int i = 0; i < N; i++) {
                segment_tree[i + N] = a[i]; // ЗАПОЛНЯЕМ ЛИСТЬЯ
            }
            for (int i = N - 1; i >= 1; i--) {
                segment_tree[i] = segment_tree[i * 2] + segment_tree[i * 2 + 1]; // ЗАПОЛНЯЕМ ОСТАЛЬНЫЕ ВЕРШИНЫ
            }
        }


        void update(int ind, int x) { // ОБНОВЛЕНИЕ ОДНОГО ЭЛЕМЕНТА ЗА О(logN)
            segment_tree[ind + N] = x; // ОБНОВЛЕНИЕ ЭЛЕМЕНТА В ЛИСТЬЯХ
            int tmp = (ind + N) / 2;
            while (tmp) {
                segment_tree[tmp] = segment_tree[tmp * 2] + segment_tree[tmp * 2 + 1]; // ОБНОВЛЯЕМ ПРЕДКА
                tmp /= 2; // ПЕРЕХОДИМ К ПРЕДКУ
            }
        }

        int get_sum(int v, int vl, int vr, int l, int r) { // v - НОМЕР ВЕРШИНЫ, vl vr - ГРАНИЦЫ ВЕРШИНЫ
            // В ИСХОДНОМ ОТРЕЗКЕ
            // l r ГРАНИЦЫ ЗАПРОСА
            if (vl >= r || vr <= l) return 0; // ЕСЛИ ОТРЕЗКИ НЕ ПЕРЕСЕКАЮТСЯ, ТО ВЕРНЕМ 0
            if (vl >= l && vr <= r) return segment_tree[v]; // ЕСЛИ ВЛОЖЕН
            return get_sum(v * 2, vl, (vl + vr) / 2, l, r) + // ЕСЛИ ПЕРЕСЕКАЮТСЯ
                   + get_sum(v * 2 + 1, (vl + vr) / 2, vr, l, r);
            // ВРЕМЯ РАБОТЫ O(logN)
        }
};

struct Seg_Tree_with_keeping_values {
    int n;
    int a[100005];
    vector<int> tree[400005];

    void build_tree(int v, int tl, int tr) {
        if (tl == tr) {
            tree[v].push_back(a[tl]);
        } else {
            int tm = (tl + tr) / 2;
            build_tree(v * 2, tl, tm);
            build_tree(v * 2 + 1, tm + 1, tr);

            tree[v].resize(tr - tl + 1);
            merge(tree[v * 2].begin(), tree[v * 2].end(),
                  tree[v * 2 + 1].begin(), tree[v * 2 + 1].end(),
                  tree[v].begin());
        }
    }

    int get_count(int l, int r, int x, int v, int tl, int tr) {
        if (l <= tl && tr <= r) {
            return tree[v].end() - lower_bound(tree[v].begin(), tree[v].end(), x);
        }
        if (tr < l || r < tl) {
            return 0;
        }
        int tm = (tl + tr) / 2;
        return get_count(l, r, x, v * 2, tl, tm) +
               + get_count(l, r, x, v * 2 + 1, tm + 1, tr);
    }
};

struct Seg_Tree_with_with_sum_query {
    int n;
    int a[100005];
    int tree[400005];
    int upd[400005];

    void build_tree(int v, int tl, int tr) {
        if (tl == tr) {
            tree[v] = a[tl];
        } else {
            int tm = (tl + tr) / 2;
            build_tree(v * 2, tl, tm);
            build_tree(v * 2 + 1, tm + 1, tr);
            tree[v] = tree[v * 2] + tree[v * 2 + 1];
        }
    }

    void push(int v, int tl, int tr) {
        if (upd[v] != 0 && v * 2 + 1 < 400005) {
            upd[v * 2] = upd[v * 2 + 1] = upd[v];
            upd[v] = 0;

            int tm = (tl + tr) / 2;
            tree[v * 2] = (tm - tl + 1) * upd[v * 2];
            tree[v * 2 + 1] = (tr - tm) * upd[v * 2 + 1];
        }
    }

    int get_sum(int l, int r, int v, int tl, int tr) {
        if (l <= tl && tr <= r) {
            return tree[v];
        }
        if (tr < l || r < tl) {
            return 0;
        }

        push(v, tl, tr);
        int tm = (tl + tr) / 2;
        return get_sum(l, r, v * 2, tl, tm) +
               + get_sum(l, r, v * 2 + 1, tm + 1, tr);
    }

    void assign(int l, int r, int val, int v, int tl, int tr) {
        if (l <= tl && tr <= r) {
            tree[v] = val * (tr - tl + 1);
            upd[v] = val;
            return;
        }
        if (tr < l || r < tl) {
            return;
        }
        push(v, tl, tr);
        int tm = (tl + tr) / 2;
        assign(l, r, val, v * 2, tl, tm);
        assign(l, r, val, v * 2 + 1, tm + 1, tr);
        tree[v] = tree[v * 2] + tree[v * 2 + 1];
    }
};





struct Max_query_seg_tree {

    struct data {
        int sum, pref, suff, ans;
    };
    data combine(data l, data r) {
        data res;
        res.sum = l.sum + r.sum;
        res.pref = max(l.pref, l.sum + r.pref);
        res.suff = max(r.suff, r.sum + l.suff);
        res.ans = max(max(l.ans, r.ans), l.suff + r.pref);
        return res;
    }

    data make_data(int value) {
        data res;
        res.sum = value;
        res.pref = res.suff = res.ans = max(0, value);
        return res;
    }

    int N = 1;
    vector<data> tree;


    void build(vector<int> &a) {
        tree.resize(MAXN);
        for (int i = 0; i < N; i++) {
            tree[i + N] = make_data(a[i]);
        }
        for (int i = N - 1; i >= 1; i--) {
            tree[i] = combine(tree[i * 2], tree[i * 2 + 1]);
        }
    }

    void update(int v, int vl, int vr, int pos, int new_val) {
        if (vl + 1 == vr) tree[v] = make_data(new_val);
        else {
            int vm = (vl + vr) / 2;
            if (pos < vm) update(v * 2, vl, vm, pos, new_val);
            else update(v * 2 + 1, vm, vr, pos, new_val);
            tree[v] = combine(tree[v * 2], tree[v * 2 + 1]);
        }
    }

    data get_query(int v, int vl, int vr, int l, int r) {
        if (vl >= r || vr <= l) return make_data(-1e9);
        if (vl >= l && vr <= r) return tree[v];
        return combine(
                get_query(v * 2, vl, (vl + vr) / 2, l, r),
                get_query(v * 2 + 1, (vl + vr) / 2, vr, l, r)
        );
    }
};

const ll MINN = -1e9;
struct seg_tree_with_max {
    int N = 1;
    vector<int> tree;

    seg_tree_with_max(int n, vector<int> &a) {
        while (N * 2 < n) N *= 2;
        N *= 2;
        tree.resize(N * 2);
        for (int i = 0; i < n; ++i) tree[i + N] = a[i];
        for (int i = N - 1; i > 0; --i) tree[i] = max(tree[i * 2], tree[i * 2 + 1]);
    }

    void update(int ind, int a) {
        tree[ind + N] = a;
        int ver = (ind + N) / 2;
        while (ver) {
            tree[ver] = max(tree[ver * 2], tree[ver * 2 + 1]);
            ver /= 2;
        }
    }

    int get_max(int v, int vl, int vr, int l, int r) {
        if (vl >= l && vr <= r) return tree[v];
        if (vl >= r || vr <= l) return MINN;
        int vm = (vl + vr) / 2;
        return max(get_max(v * 2, vl, vm, l, r), get_max(v * 2 + 1, vm, vr, l, r));
    }

    int get_max_wrap(int l, int r) {
        return get_max(1, 0, N, l, r);
    }

};

struct Seg_tree_with_changing_inter {
    int N = 1;
    vector<ll> tree;
    vector<ll> upd;

    Seg_tree_with_changing_inter(int n, vector<ll> &a) {
        while (N * 2 < n) N *= 2;
        N *= 2;
        tree.resize(N * 2);
        upd.resize(N * 2);
        for (int i = 0; i < N; i++) {
            tree[i + N] = a[i];
        }
        for (int i = N - 1; i > 1; i--) {
            tree[i] = max(tree[i * 2], tree[i * 2 + 1]);
        }
    }

    void push(int v) {
        upd[v * 2] += upd[v];
        upd[v * 2 + 1] += upd[v];
        tree[v * 2] += upd[v];
        tree[v * 2 + 1] += upd[v];
        upd[v] = 0;
    }

    void update(int v, int tl, int tr, int l, int r, ll x) {
        if (tl >= l && tr <= r) {
            upd[v] += x;
            tree[v] += x;
            return;
        }
        if (tl >= r || tr <= l) {
            return;
        }
        int tm = (tl + tr) / 2;
        push(v);
        update(v * 2, tl, tm, l, r, x);
        update(v * 2 + 1, tm, tr, l, r, x);
        tree[v] = max(tree[v * 2], tree[v * 2 + 1]);
    }

    ll get(int v, int tl, int tr, int ind) {
        if (ind < tl || ind >= tr) return 0;
        if (tl + 1 == tr) return tree[v];
        int tm = (tl + tr) / 2;
        push(v);
        if (ind < tm) return get(v * 2, tl, tm, ind);
        return get(v * 2 + 1, tm, tr, ind);

    }
};