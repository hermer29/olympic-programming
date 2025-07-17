using System;
using System.Linq;
using System.IO;
using System.Text;
using System.Collections;
using System.Collections.Generic;

using static Player;
using static Team;

public enum Team
{
    Player,
    Enemy
}

public class Agent
{
    public int id;
    public int localId;
    public int x;
    public int y;
    public int shootCooldown;
    public int soakingPower;
    public int cooldown;
    public int optimalRange;
    public int splashBombs;
    public int wetness;
    public int? plannedWetness;
    public int Team;

    public BTNode Tree;

    private string collectedCommand;

    public void WritePrefix()
    {
        collectedCommand = "{id}";
    }

    public void EndCommand() {
        Console.WriteLine(collectedCommand);
    }

    public void Move(int x, int y)
    {
        collectedCommand += $";MOVE {x} {y}";
    }

    public void Attack(Agent agent)
    {
        collectedCommand += $";SHOOT {agent.id}";
    }

    public void Idle()
    {
        collectedCommand += ";HUNKER_DOWN";
    }

    public bool IsFriendly => Team == MyId;
    public bool IsAlive => wetness >= 100;
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

public static class Blackboard
{
    private static Dictionary<string, object> _data = new Dictionary<string, object>();

    public static void Set<T>(string key, T value) => _data[key] = value;

    public static T Get<T>(string key) => _data.TryGetValue(key, out var value) ? (T)value : default(T);

    public static bool Has(string key) => _data.ContainsKey(key);

    public static void LocalSet<T>(Agent agent, string key, T value)
    {
        _data[$"{agent.id}_{key}"] = value;
    }

    public static T LocalGet<T>(Agent agent, string key) => _data.TryGetValue($"{agent.id}_{key}", out var value) ? (T)value : default(T);
    public static bool LocalHas(Agent agent, string key) => _data.ContainsKey($"{agent.id}_{key}");
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
        
        var searchForBestTarget = new ActionNode(() =>
        {
            var maxH = 0;
            Agent bestTarget = null;
            foreach (var enemy in Hostile.Values)
            {
                var h = CalculateAttackHeuristic(agent, enemy);
                if (h > maxH)
                {
                    maxH = h;
                    bestTarget = enemy;
                }
            }
            Blackboard.LocalSet(agent, "target", bestTarget);
            return NodeStatus.Success;
        });

        var isAttack = new ConditionNode(() =>
        {
            var target = Blackboard.LocalGet<Agent>(agent, "target");
            if (target == null)
            {
                return NodeStatus.Failure;
            }
            return NodeStatus.Success;
        });

        var attack = new ActionNode(() =>
        {
            var target = Blackboard.LocalGet(agent, "target", bestTarget);
            agent.Attack(target);
            return NodeStatus.Success;
        });

        var ignore = new ActionNode(() =>
        {
            agent.Idle();
            return NodeStatus.Success;
        });


        // Композитные узлы
        var attackSequence = new SequenceNode(new List<BTNode> { searchForBestTarget, isAttack, attack });
        var mainSelector = new SelectorNode(new List<BTNode> { attackSequence, ignore });

        return mainSelector;
    }
}


/**
 * Win the water fight by controlling the most territory, or out-soak your opponent!
 **/
class Player
{
    public static int CalculateAttackHeuristic(Agent a, Agent b)
    {
        var damage = CalculateDamage(a, b);
        b.plannedWetness ??= b.wetness;
        if (b.plannedWetness.Value >= 100)
        {
            return 0;
        }
        if (b.plannedWetness.Value + damage >= 100)
        {
            return 9999;
        }
        return b.plannedWetness.Value + damage;
    }

    public static int ManhattanDistance(int x1, int y1, int x2, int y2)
    {
        return Math.Abs(x1 - x2) + Math.Abs(y1 - y2);
    }

    public static int CalculateDamage(Agent agentA, Agent agentB)
    {
        var initDamage = agentA.soakingPower;
        var distance = ManhattanDistance(agentA.x, agentA.y, agentB.x, agentB.y);
        if (distance > agentA.optimalRange * 2)
        {
            initDamage = 0;
        }
        if (distance > agentA.optimalRange)
        {
            initDamage /= 2;
        }
        return initDamage;
    }

    public static int MyId;
    public static Dictionary<int, Agent> Friendlies = new Dictionary<int, Agent>();
    public static Dictionary<int, Agent> Hostile = new Dictionary<int, Agent>();
    public static Dictionary<int, Agent> AllAgents = new Dictionary<int, Agent>();

    static void Main(string[] args)
    {
        var behaviourTreeBuilder = new BehaviorTreeBuilder();

        string[] inputs;
        MyId = int.Parse(Console.ReadLine()); // Your player id (0 or 1)
        int agentCount = int.Parse(Console.ReadLine()); // Total number of agents in the game
        for (int i = 0; i < agentCount; i++)
        {
            var agent = new Agent();
            inputs = Console.ReadLine().Split(' ');
            agent.id = int.Parse(inputs[0]); // Unique identifier for this agent
            agent.Team = int.Parse(inputs[1]); // Player id of this agent
            agent.shootCooldown = int.Parse(inputs[2]); // Number of turns between each of this agent's shots
            agent.cooldown = agent.shootCooldown;
            agent.optimalRange = int.Parse(inputs[3]); // Maximum manhattan distance for greatest damage output
            agent.soakingPower = int.Parse(inputs[4]); // Damage output within optimal conditions
            agent.splashBombs = int.Parse(inputs[5]); // Number of splash bombs this can throw this game
            AllAgents[agent.id] = agent;
            if (agent.IsFriendly)
            {
                Friendlies[agent.id] = agent;
                agent.Tree = behaviourTreeBuilder.Build(agent);
            }
            else
            {
                Hostile[agent.id] = agent;
            }

        }
        inputs = Console.ReadLine().Split(' ');
        int width = int.Parse(inputs[0]); // Width of the game map
        int height = int.Parse(inputs[1]); // Height of the game map
        for (int i = 0; i < height; i++)
        {
            inputs = Console.ReadLine().Split(' ');
            for (int j = 0; j < width; j++)
            {
                int x = int.Parse(inputs[3 * j]);// X coordinate, 0 is left edge
                int y = int.Parse(inputs[3 * j + 1]);// Y coordinate, 0 is top edge
                int tileType = int.Parse(inputs[3 * j + 2]);
            }
        }

        // game loop
        while (true)
        {
            agentCount = int.Parse(Console.ReadLine());

            for (int i = 0; i < agentCount; i++)
            {
                inputs = Console.ReadLine().Split(' ');
                var id = int.Parse(inputs[0]);
                var curr = AllAgents[id];
                curr.x = int.Parse(inputs[1]);
                curr.y = int.Parse(inputs[2]);
                curr.cooldown = int.Parse(inputs[3]); // Number of turns before this agent can shoot
                curr.splashBombs = int.Parse(inputs[4]);
                curr.wetness = int.Parse(inputs[5]); // Damage (0-100) this agent has taken
            }
            int myAgentCount = int.Parse(Console.ReadLine()); // Number of alive agents controlled by you
            foreach (var agent in Friendlies.Values.OrderBy(x => x.id))
            {
                agent.WritePrefix();
                agent.Tree.Execute();
                agent.EndCommand();
            }
        }
    }
}