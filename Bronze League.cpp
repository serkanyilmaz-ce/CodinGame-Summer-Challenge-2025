#include <bits/stdc++.h>
using namespace std;

struct AgentInfo {
    int player;
    int shoot_cooldown;
    int optimal_range;
    int soaking_power;
    int splash_bombs;
};

struct AgentState {
    int id, player, x, y;
    int cooldown, splashBombs, wetness;
};

inline int sign(int x) { return (x>0) - (x<0); }
inline int manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int myId, agentDataCount;
    if (!(cin >> myId >> agentDataCount))
        return 0;

    unordered_map<int, AgentInfo> info;
    for (int i = 0; i < agentDataCount; i++) {
        int id, pl, cd, range, power, bombs;
        cin >> id >> pl >> cd >> range >> power >> bombs;
        info[id] = {pl, cd, range, power, bombs};
    }

    int width, height;
    cin >> width >> height;
    vector<vector<int>> grid(height, vector<int>(width));
    for (int i = 0; i < width * height; i++) {
        int x, y, t;
        cin >> x >> y >> t;
        grid[y][x] = t;
    }

    while (true) {
        int agentCount;
        if (!(cin >> agentCount))
            break;

        vector<AgentState> all(agentCount);
        for (int i = 0; i < agentCount; i++) {
            auto &a = all[i];
            cin >> a.id >> a.x >> a.y >> a.cooldown >> a.splashBombs >> a.wetness;
            a.player = info[a.id].player;
        }

        int myAgentCount;
        cin >> myAgentCount;  // skip, we'll filter by player

        vector<AgentState> me, enemies;
        me.reserve(myAgentCount);
        for (auto &a : all) {
            if (a.player == myId) me.push_back(a);
            else                enemies.push_back(a);
        }

        for (auto &a : me) {
            // find closest enemy
            int bestD = INT_MAX, tgtId = -1, tx = 0, ty = 0;
            for (auto &e : enemies) {
                int d = manhattan(a.x, a.y, e.x, e.y);
                if (d < bestD) {
                    bestD = d;
                    tgtId = e.id;
                    tx = e.x; ty = e.y;
                }
            }

            auto &inf = info[a.id];

            // compute move target
            int dx = sign(tx - a.x), dy = sign(ty - a.y);
            int nx = a.x + dx, ny = a.y + dy;
            nx = max(0, min(width - 1, nx));
            ny = max(0, min(height - 1, ny));

            // output id and MOVE
            cout << a.id << ";MOVE " << nx << " " << ny;

            // decide combat action
            if (a.cooldown == 0 && bestD <= inf.optimal_range * 2) {
                cout << ";SHOOT " << tgtId;
            }
            else if (a.splashBombs > 0 && bestD <= 4) {
                cout << ";THROW " << tx << " " << ty;
            }
            // otherwise no combat action (could HUNKER_DOWN here if wanted)

            cout << "\n";
        }

        cout.flush();
    }

    return 0;
}
