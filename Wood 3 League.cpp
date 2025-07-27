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
  int agent_id; // Unique identifier for this agent
  int player;
  int shoot_cooldown;
  int optimal_range;
  int soaking_power;
  int splash_bombs;
  AgentState states; // Added to keep track of agent's state
};

unordered_map<int, Agent> agents;

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
    int agent_id;       // Unique identifier for this agent
    int player;         // Player id of this agent
    int shoot_cooldown; // Number of turns between each of this agent's shots
    int optimal_range;  // Maximum manhattan distance for greatest damage output
    int soaking_power;  // Damage output within optimal conditions
    int splash_bombs;   // Number of splash bombs this can throw this game
    cin >> agent_id >> player >> shoot_cooldown >> optimal_range >> soaking_power >> splash_bombs;
    cin.ignore();

    // Initialize agent state with default values
    AgentState agent_states = {0, 0, 0, 0, 0};
    // Store agent information in the agents map
    agents[agent_id] = {agent_id, player, shoot_cooldown, optimal_range, soaking_power, splash_bombs, agent_states};
  }

  int width;  // Width of the game map
  int height; // Height of the game map
  cin >> width >> height;
  cin.ignore();

  vector<vector<int>> map(height, vector<int>(width, 0)); // Initialize the map
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      int x; // X coordinate, 0 is left edge
      int y; // Y coordinate, 0 is top edge
      int tile_type;
      cin >> x >> y >> tile_type;
      cin.ignore();
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
      if (target_agent.agent_id == -1 || target_agent.states.wetness < wetness)
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

      cout << agent.agent_id << "; SHOOT " << target_agent.agent_id << endl;
    }
  }
}