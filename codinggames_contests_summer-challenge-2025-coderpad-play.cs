using System;
using System.Linq;
using System.IO;
using System.Text;
using System.Collections;
using System.Collections.Generic;

/**
 * Win the water fight by controlling the most territory, or out-soak your opponent!
 **/
class Player
{
    static void Main(string[] args)
    {
        string[] inputs;
        int myId = int.Parse(Console.ReadLine()); // Your player id (0 or 1)
        int agentCount = int.Parse(Console.ReadLine()); // Total number of agents in the game
        for (int i = 0; i < agentCount; i++)
        {
            inputs = Console.ReadLine().Split(' ');
            int agentId = int.Parse(inputs[0]); // Unique identifier for this agent
            int player = int.Parse(inputs[1]); // Player id of this agent
            int shootCooldown = int.Parse(inputs[2]); // Number of turns between each of this agent's shots
            int optimalRange = int.Parse(inputs[3]); // Maximum manhattan distance for greatest damage output
            int soakingPower = int.Parse(inputs[4]); // Damage output within optimal conditions
            int splashBombs = int.Parse(inputs[5]); // Number of splash bombs this can throw this game
        }
        inputs = Console.ReadLine().Split(' ');
        int width = int.Parse(inputs[0]); // Width of the game map
        int height = int.Parse(inputs[1]); // Height of the game map
        for (int i = 0; i < height; i++)
        {
            inputs = Console.ReadLine().Split(' ');
            for (int j = 0; j < width; j++)
            {
                int x = int.Parse(inputs[3*j]);// X coordinate, 0 is left edge
                int y = int.Parse(inputs[3*j+1]);// Y coordinate, 0 is top edge
                int tileType = int.Parse(inputs[3*j+2]);
            }
        }

        // game loop
        while (true)
        {
            int agentCount = int.Parse(Console.ReadLine());
            for (int i = 0; i < agentCount; i++)
            {
                inputs = Console.ReadLine().Split(' ');
                int agentId = int.Parse(inputs[0]);
                int x = int.Parse(inputs[1]);
                int y = int.Parse(inputs[2]);
                int cooldown = int.Parse(inputs[3]); // Number of turns before this agent can shoot
                int splashBombs = int.Parse(inputs[4]);
                int wetness = int.Parse(inputs[5]); // Damage (0-100) this agent has taken
            }
            int myAgentCount = int.Parse(Console.ReadLine()); // Number of alive agents controlled by you
            for (int i = 0; i < myAgentCount; i++)
            {

                // Write an action using Console.WriteLine()
                // To debug: Console.Error.WriteLine("Debug messages...");


                // One line per agent: <agentId>;<action1;action2;...> actions are "MOVE x y | SHOOT id | THROW x y | HUNKER_DOWN | MESSAGE text"
                Console.WriteLine("HUNKER_DOWN");
            }
        }
    }
}