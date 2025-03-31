/*
Undo the destiny.
*/
#include <bits/stdc++.h>
using namespace std;
#include "test.h"
#define ll long long
#define FO(x) {freopen(#x".in","r",stdin);freopen(#x".out","w",stdout);}
#define pii pair<int,int>
#define pll pair<ll,ll>
#define mp make_pair
const int N = 110;
bool vis[N][N][N];
int f[N][N][N];
int main() {
    for (int i = 1;i <= 10;++i) {
        for (int j = 1;j <= 10;++j) {
            vis[i][j][0] = 1;
        }
    }
    for (int i = 1;i <= 10;++i) {
        for (int j = 1;j <= 10;++j) {
            for (int k = 1;k <= 10;++k) {
                if (i == j || j == k || k == j) continue;
                vis[i][j][k] = 1;
                cerr << i << " " << j << " " << k << "\n";
                cerr << i << " " << k << " " << k-1 << "\n";
                cerr << k << " " << j << " " << k-1 << "\n";
                assert(vis[i][k][k-1] && vis[k][j][k-1]);
            }
        }
    }
}
/*
1
-G--
---E
12 2
-G--
-G--
*/