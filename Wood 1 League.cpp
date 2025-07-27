#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

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

// Map of agents indexed by their unique id
unordered_map<int, Agent> agents;

// The map is represented as a 2D vector where each element corresponds to a tile
// Tile types: 0 = empty, 1 = low tile, 2 = high tile
vector<vector<int>> map;

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
    int agentid_on_center = -1;
    int left_upper = -1, right_upper = -1, left_lower = -1, right_lower = -1;
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
        }

        int my_agent_count; // Number of alive agents controlled by you
        cin >> my_agent_count;
        cin.ignore();

        for (const auto &pair : agents)
        {
            const Agent &agent = pair.second;
            if (agent.player != my_id || agent.states.wetness >= 100)
                continue;
            if (agent.states.x > 4 && agent.states.x < width - 4 && agent.states.y > 4 && agent.states.y < height - 4 && agentid_on_center == -1)
                agentid_on_center = agent.id; // Store the agent on center
            if (agent.id == agentid_on_center)
            {
                if ((left_upper == -1 || left_lower == -1) && agent.states.x != 2)
                    cout << agent.id << "; MOVE " << agent.states.x - 1 << " " << agent.states.y << endl; // Move left
                else if (left_upper == -1)
                {
                    cout << agent.id << "; THROW " << "2 2" << endl; // Throw a splash bomb to the left upper corner
                    left_upper = 1;                                  // Store the agent in the left upper corner
                }
                else if (left_lower == -1)
                {
                    cout << agent.id << "; THROW " << "2 " << height - 3 << endl; // Throw a splash bomb to the left lower corner
                    left_lower = 1;                                               // Store the agent in the left lower corner
                }
                else if ((right_upper == -1 || right_lower == -1) && agent.states.x != width - 3)
                    cout << agent.id << "; MOVE " << agent.states.x + 1 << " " << agent.states.y << endl; // Move right
                else if (right_upper == -1)
                {
                    cout << agent.id << "; THROW " << width - 3 << " 2" << endl; // Throw a splash bomb to the right upper corner
                    right_upper = 1;                                             // Store the agent in the right upper corner
                }
                else if (right_lower == -1)
                {
                    cout << agent.id << "; THROW " << width - 3 << " " << height - 3 << endl; // Throw a splash bomb to the right lower corner
                    right_lower = 1;                                                          // Store the agent in the right lower corner
                }
            }
            else
            {
                cout << agent.id << "; MESSAGE " << "WAITING" << endl; // Throw a splash bomb
                if (agent.states.x < 4 && agent.states.y < 4 && left_upper == -1)
                    left_upper = 1; // Store the agent in the left upper corner
                else if (agent.states.x > width - 4 && agent.states.y < 4 && right_upper == -1)
                    right_upper = 1; // Store the agent in the right upper corner
                else if (agent.states.x < 4 && agent.states.y > height - 4 && left_lower == -1)
                    left_lower = 1; // Store the agent in the left lower corner
                else if (agent.states.x > width - 4 && agent.states.y > height - 4 && right_lower == -1)
                    right_lower = 1; // Store the agent in the right lower corner
            }
        }
    }
}