#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <climits>

using namespace std;

/**
 * Win the water fight by controlling the most territory, or out-soak your
 *opponent!
 **/

struct AgentState
{
    int x, y;
    int cooldown;
    int splash_bombs;
    int wetness;
};

struct Agent
{
    int id; // Unique identifier for this agent
    int player;
    int shoot_cooldown;
    int optimal_range;
    int soaking_power;
    int splash_bombs;
    AgentState states; // Added to keep track of agent's state
};

unordered_map<int, Agent> agents; // Map of agents indexed by their unique id

// The map is represented as a 2D vector where each element corresponds to a tile
// Tile types: 0 = empty, 1 = low tile, 2 = high tile
vector<vector<int>> map; // Game map represented as a 2D vector

int CheckEnemyCoverTileType(const Agent &attacker, const Agent &enemy)
{

    int cover = 0;
    int delta_x = enemy.states.x - attacker.states.x;
    int delta_y = enemy.states.y - attacker.states.y;
    int dist = abs(delta_x) + abs(delta_y);
    /*
        // Check the tile type of the enemy's position
        if (delta_x > 0 && delta_y == 0) // Enemy is to the right
        {
            if (map[enemy.states.x - 1][enemy.states.y] == 1 || map[enemy.states.x - 1][enemy.states.y] == 2)
                cover = map[enemy.states.x - 1][enemy.states.y];
        }
        else if (delta_x < 0 && delta_y == 0) // Enemy is to the left
        {
            if (map[enemy.states.x + 1][enemy.states.y] == 1 || map[enemy.states.x + 1][enemy.states.y] == 2)
                cover = map[enemy.states.x + 1][enemy.states.y];
        }
        else if (delta_x == 0 && delta_y > 0) // Enemy is below
        {
            if (map[enemy.states.x][enemy.states.y - 1] == 1 || map[enemy.states.x][enemy.states.y - 1] == 2)
                cover = map[enemy.states.x][enemy.states.y - 1];
        }
        else if (delta_x == 0 && delta_y < 0) // Enemy is above
        {
            if (map[enemy.states.x][enemy.states.y + 1] == 1 || map[enemy.states.x][enemy.states.y + 1] == 2)
                cover = map[enemy.states.x][enemy.states.y + 1];
        }
        else if (delta_x > 0 && delta_y > 0) // Enemy is below and to the right
        {
            if (map[enemy.states.x - 1][enemy.states.y - 1] == 1 || map[enemy.states.x - 1][enemy.states.y - 1] == 2)
                cover = map[enemy.states.x - 1][enemy.states.y - 1];
        }
        else if (delta_x < 0 && delta_y < 0) // Enemy is above and to the left
        {
            if (map[enemy.states.x + 1][enemy.states.y + 1] == 1 || map[enemy.states.x + 1][enemy.states.y + 1] == 2)
                cover = map[enemy.states.x + 1][enemy.states.y + 1];
        }
        else if (delta_x > 0 && delta_y < 0) // Enemy is above and to the right
        {
            if (map[enemy.states.x - 1][enemy.states.y + 1] == 1 || map[enemy.states.x - 1][enemy.states.y + 1] == 2)
                cover = map[enemy.states.x - 1][enemy.states.y + 1];
        }
        else if (delta_x < 0 && delta_y > 0) // Enemy is below and to the left
        {
            if (map[enemy.states.x + 1][enemy.states.y - 1] == 1 || map[enemy.states.x + 1][enemy.states.y - 1] == 2)
                cover = map[enemy.states.x + 1][enemy.states.y - 1];
        }
    */

    // Check the tile type of the enemy's position
    if (delta_x > 0) // Enemy is to the right
    {
        if (map[enemy.states.x - 1][enemy.states.y] == 1 || map[enemy.states.x - 1][enemy.states.y] == 2)
            cover = map[enemy.states.x - 1][enemy.states.y];
    }
    else if (delta_x < 0) // Enemy is to the left
    {
        if (map[enemy.states.x + 1][enemy.states.y] == 1 || map[enemy.states.x + 1][enemy.states.y] == 2)
            cover = map[enemy.states.x + 1][enemy.states.y];
    }

    return cover;
}

int CalculateDamage(const Agent &attacker, const Agent &enemy)
{
    int base = attacker.soaking_power;
    // int dist = abs(attacker.states.x - enemy.states.x) + abs(attacker.states.y - enemy.states.y);
    // if (dist > attacker.optimal_range)
    // return 0;

    int cover = CheckEnemyCoverTileType(attacker, enemy);
    if (cover == 2)
        return base / 4;
    if (cover == 1)
        return base / 2;
    return base;
}

Agent GetClosestEnemy(const Agent &my_agent)
{
    Agent best_target = {-1, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0}};
    int best_damage = -1;

    for (const auto &pair : agents)
    {
        const Agent &enemy = pair.second;

        if (enemy.player == my_agent.player || enemy.states.wetness >= 100)
            continue;

        int dmg = CalculateDamage(my_agent, enemy);
        if (dmg > best_damage)
        {
            best_damage = dmg;
            best_target = enemy;
        }
    }

    return best_target;
}

pair<int, int> GetClosestTile(const Agent &my_agent)
{
    int closest_tile_x = my_agent.states.x;
    int closest_tile_y = my_agent.states.y;
    int min_distance = INT_MAX;

    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i][j] == 1 || map[i][j] == 2) // Only consider low or high tiles
            {
                int distance = abs(my_agent.states.x - j) + abs(my_agent.states.y - i); // Manhattan distance
                if (distance == min_distance)
                {
                    // If the distance is the same, prefer the tile with type 2
                    if (map[i][j] == 2 && map[closest_tile_y][closest_tile_x] != 2)
                    {
                        distance = min_distance; // Update distance to match
                        closest_tile_x = j;
                        closest_tile_y = i;
                    }
                }
                // Check if this tile is closer than the current closest tile
                else if (distance < min_distance)
                {
                    min_distance = distance;
                    closest_tile_x = j;
                    closest_tile_y = i;
                }
            }
        }
    }
    return make_pair(closest_tile_x, closest_tile_y);
}

int main()
{
    int my_id; // Your player id (0 or 1)
    cin >> my_id;
    cin.ignore();

    int agent_data_count; // Total number of agents in the game
    cin >> agent_data_count;
    cin.ignore();

    for (int i = 0; i < agent_data_count; i++)
    {
        int id;             // Unique identifier for this agent
        int player;         // Player id of this agent
        int shoot_cooldown; // Number of turns between each of this agent's shots
        int optimal_range;  // Maximum manhattan distance for greatest damage output
        int soaking_power;  // Damage output within optimal conditions
        int splash_bombs;   // Number of splash bombs this can throw this game
        cin >> id >> player >> shoot_cooldown >> optimal_range >> soaking_power >> splash_bombs;
        cin.ignore();

        // Initialize agent state with default values
        AgentState agent_states = {0, 0, 0, 0, 0};
        // Store agent information in the agents map
        agents[id] = {id, player, shoot_cooldown, optimal_range, soaking_power, splash_bombs, agent_states};
    }

    int width;  // Width of the game map
    int height; // Height of the game map
    cin >> width >> height;
    cin.ignore();

    // Initialize the game map with tile types
    map.resize(height, vector<int>(width, 0)); // Initialize the map
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int x, y, tile_type;
            cin >> x >> y >> tile_type;
            cin.ignore();
            map[i][j] = tile_type; // Initialize each tile with its coordinates and type
        }
    }

    // game loop
    while (1)
    {
        int agent_count; // Total number of agents still in the game
        cin >> agent_count;
        cin.ignore();

        Agent target_agent = {-1, 0, 0, 0, 0, 0, {0, 0, 0, 0, 0}}; // Initialize target agent with high wetness

        for (int i = 0; i < agent_count; i++)
        {
            int agent_id;
            int x;
            int y;
            int cooldown; // Number of turns before this agent can shoot
            int splash_bombs;
            int wetness; // Damage (0-100) this agent has taken
            cin >> agent_id >> x >> y >> cooldown >> splash_bombs >> wetness;
            cin.ignore();

            agents[agent_id].states = {x, y, cooldown, splash_bombs, wetness};

            // Skip if the agent is controlled by the player or has high wetness
            if (agents[agent_id].player == my_id)
                continue;
            // Find the agent with the most wetness
            if (target_agent.id == -1 || target_agent.states.wetness < wetness)
            {
                target_agent = agents[agent_id]; // Initialize target agent
            }
        }

        int my_agent_count; // Number of alive agents controlled by you
        cin >> my_agent_count;
        cin.ignore();

        for (const auto &pair : agents)
        {
            const Agent &agent = pair.second;
            if (agent.player != my_id || agent.states.wetness >= 100)
                continue;
            // 1; MOVE 6 3; SHOOT 4
            ::pair<int, int> closest_tile = GetClosestTile(agent); // Get the closest tile to the agent
            Agent target_agent = GetClosestEnemy(agent);
            cout << agent.id << "; ";
            if (closest_tile.first == 1)
                cout << agent.id << "; MOVE " << closest_tile.first - 1 << " " << closest_tile.second << "; SHOOT " << target_agent.id << endl;
            else if (closest_tile.first == 11)
                cout << agent.id << "; MOVE " << closest_tile.first + 1 << " " << closest_tile.second << "; SHOOT " << target_agent.id << endl;
        }
    }
}