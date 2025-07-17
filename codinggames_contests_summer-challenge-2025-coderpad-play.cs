using System;
using System.Linq;
using System.IO;
using System.Text;
using System.Collections;
using System.Collections.Generic;


public class Agent
{
    public int id;
    public int x;
    public int y;
    public int cooldown;
    public int splashbombs;
    public int wetness;

    public BTNode Tree;

    public void Move(int x, int y)
    {
        Console.WriteLine($"{id}; MOVE {x} {y}");
    }
}

public enum NodeStatus
{
    Running,
    Success,
    Failure
}

public abstract class BTNode
{
    public abstract NodeStatus Execute();
}

// Последовательность (Sequence) — выполняет детей, пока один не вернёт Failure
public class SequenceNode : BTNode
{
    private List<BTNode> _children = new List<BTNode>();

    public SequenceNode(List<BTNode> children)
    {
        _children = children;
    }

    public override NodeStatus Execute()
    {
        foreach (var child in _children)
        {
            var status = child.Execute();
            if (status != NodeStatus.Success)
                return status;
        }
        return NodeStatus.Success;
    }
}

// Действие (Action)
public class ActionNode : BTNode
{
    private Func<NodeStatus> _action;

    public ActionNode(Func<NodeStatus> action)
    {
        _action = action;
    }

    public override NodeStatus Execute()
    {
        return _action();
    }
}


// Выбор (Selector) — выполняет детей, пока один не вернёт Success
public class SelectorNode : BTNode
{
    private List<BTNode> _children = new List<BTNode>();

    public SelectorNode(List<BTNode> children)
    {
        _children = children;
    }

    public override NodeStatus Execute()
    {
        foreach (var child in _children)
        {
            var status = child.Execute();
            if (status != NodeStatus.Failure)
                return status;
        }
        return NodeStatus.Failure;
    }
}

// Условие (Condition)
public class ConditionNode : BTNode
{
    private Func<bool> _condition;

    public ConditionNode(Func<bool> condition)
    {
        _condition = condition;
    }

    public override NodeStatus Execute()
    {
        return _condition() ? NodeStatus.Success : NodeStatus.Failure;
    }
}

// Инвертор (Inverter) — меняет Success на Failure и наоборот
public class InverterNode : BTNode
{
    private BTNode _child;

    public InverterNode(BTNode child)
    {
        _child = child;
    }

    public override NodeStatus Execute()
    {
        var status = _child.Execute();
        return status == NodeStatus.Success ? NodeStatus.Failure : 
               status == NodeStatus.Failure ? NodeStatus.Success : 
               NodeStatus.Running;
    }
}

// Повтор (Repeater) — повторяет ребёнка N раз или бесконечно
public class RepeaterNode : BTNode
{
    private BTNode _child;
    private int _times;
    private int _current;

    public RepeaterNode(BTNode child, int times = -1)
    {
        _child = child;
        _times = times;
    }

    public override NodeStatus Execute()
    {
        if (_times < 0 || _current < _times)
        {
            _child.Execute();
            _current++;
            return NodeStatus.Running;
        }
        return NodeStatus.Success;
    }
}

// Пример агента с простым поведением:
// 1. Если враг близко → атаковать.
// 2. Если здоровья мало → отступать.
// 3. Иначе → патрулировать.

public class BehaviorTreeBuilder
{
    public BTNode Build(Agent agent)
    {
        // Листья (действия и условия)
        var isId1 = new ConditionNode(() => agent.id == 1);
        var move61 = new ActionNode(() =>
        {
            agent.Move(6, 1);
            return NodeStatus.Success;
    });
        var isId2 = new ConditionNode(() => agent.id == 2);
        var move63 = new ActionNode(() =>
        {
            agent.Move(6, 1);
            return NodeStatus.Success;
        });

        // Композитные узлы
        var attackSequence = new SequenceNode(new List<BTNode> { isId1, move61 });
        var retreatSequence = new SequenceNode(new List<BTNode> { isId2, move63 });
         var mainSelector = new SelectorNode(new List<BTNode> { attackSequence, retreatSequence });

        return mainSelector;
    }
}

/**
 * Win the water fight by controlling the most territory, or out-soak your opponent!
 **/
class Player
{
    

    static void Main(string[] args)
    {
        var behaviourTreeBuilder = new BehaviorTreeBuilder();

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
            agentCount = int.Parse(Console.ReadLine());
            var agents = new List<Agent>();

            for (int i = 0; i < agentCount; i++)
            {
                var agent = new Agent();
                inputs = Console.ReadLine().Split(' ');
                agent.id = int.Parse(inputs[0]);
                agent.x = int.Parse(inputs[1]);
                agent.y = int.Parse(inputs[2]);
                agent.cooldown = int.Parse(inputs[3]); // Number of turns before this agent can shoot
                agent.splashbombs = int.Parse(inputs[4]);
                agent.wetness = int.Parse(inputs[5]); // Damage (0-100) this agent has taken
                agent.Tree = behaviourTreeBuilder.Build(agent);
                agents.Add(agent);
            }
            int myAgentCount = int.Parse(Console.ReadLine()); // Number of alive agents controlled by you
            for (int i = 0; i < myAgentCount; i++)
            {
                var curr = agents[i];
                curr.Tree.Execute();
                // Write an action using Console.WriteLine()
                // To debug: Console.Error.WriteLine("Debug messages...");


                // One line per agent: <agentId>;<action1;action2;...> actions are "MOVE x y | SHOOT id | THROW x y | HUNKER_DOWN | MESSAGE text"
            
            }
        }
    }
}