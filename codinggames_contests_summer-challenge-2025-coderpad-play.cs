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
    public int plannedWetness;
    public int Team;

    public bool isDead => wetness >= 100;
    public BTNode Tree;

    private string collectedCommand;

    public void WritePrefix()
    {
        collectedCommand = $"{id}";
    }

    public void EndCommand()
    {
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

    public Agent DeepCopy()
    {
        return new Agent()
        {
            id = this.id,
            collectedCommand = this.collectedCommand,
            x = this.x,
            y = this.y,
            shootCooldown = this.shootCooldown,
            soakingPower = this.soakingPower,
            cooldown = this.cooldown,
            optimalRange = this.optimalRange,
            splashBombs = this.splashBombs,
            wetness = this.wetness,
            plannedWetness = this.plannedWetness,
            Team = this.Team,
            localId = this.localId
        };
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
            var maxRatio = 0f;
            var bestTargetAll = (Agent)null;
            var bestY = 0;
            for (int y = 0; y <= 4; y++)
            {
                var ratio = SimulateOutInDamageRatio(agent, agent.x, y, out Agent bestTarget);

                if (ratio > maxRatio)
                {
                    maxRatio = ratio;
                    bestY = y;
                    bestTargetAll = bestTarget;
                }
            }
            Console.Error.WriteLine($"Best target for agent {agent.id} at position ({agent.x}, {bestY}) with ratio {maxRatio}, target {bestTargetAll?.id}");
            agent.Move(agent.x, bestY);
            Blackboard.LocalSet(agent, "target", bestTargetAll);
            return NodeStatus.Success;
        });

        var isAttack = new ConditionNode(() =>
        {
            var target = Blackboard.LocalGet<Agent>(agent, "target");
            return target != null;
        });

        var attack = new ActionNode(() =>
        {
            var target = Blackboard.LocalGet<Agent>(agent, "target");
            agent.Attack(target);
            target.plannedWetness += CalculateDamage(agent, target);
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
        if (b.plannedWetness >= 100)
        {
            return 0;
        }
        if (b.plannedWetness + damage >= 100)
        {
            return 9999;
        }
        return b.plannedWetness + damage;
    }

    public static float SimulateOutInDamageRatio(Agent a, int x, int y, out Agent bestTarget)
    {
        bestTarget = null;
        var copy = a.DeepCopy();
        copy.x = x;
        copy.y = y;
        var allDmgToMe = 0f;
        var allDmgToEnemy = 0f;
        var maxDmg = 0f;
        foreach (Agent hostile in Hostile.Values)
        {
            if (hostile.isDead)
            {
                continue;
            }
            var dmg = CalculateDamageWithCovers(copy, hostile);
            if(dmg > maxDmg)
            {
                maxDmg = dmg;
                bestTarget = hostile;
            }
            allDmgToMe += CalculateDamageWithCovers(hostile, copy);
            allDmgToEnemy += dmg;
        }
        allDmgToEnemy /= Hostile.Count;
        allDmgToMe /= Hostile.Count;
        var ratio = allDmgToMe / (allDmgToEnemy + 0.0001f); // Добавляем малое значение, чтобы избежать деления на ноль
        Console.Error.WriteLine($"Simulating attack at position ({x}, {y}), my damage: {allDmgToMe}, enemy damage: {allDmgToEnemy}, ratio: {ratio}");
        return ratio;
    }

    public static int ManhattanDistance(int x1, int y1, int x2, int y2)
    {
        return Math.Abs(x1 - x2) + Math.Abs(y1 - y2);
    }

    public static float CalculateDamageWithCovers(Agent agentA, Agent agentB)
    {
        var initDamage = CalculateDamage(agentA, agentB);
        var damageModification = DamageCalculator.IsTargetCovered(agentA, agentB);
        if(agentA.x == 12 && agentA.y == 3)
            Console.Error.WriteLine($"Agent {agentA.id} attacking {agentB.id} at ({agentB.x}, {agentB.y}) with initial damage {initDamage} and modification {damageModification}");
        return initDamage * damageModification;
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

    public static List<(int x, int y)> GetShortestPath(int startX, int startY, int endX, int endY)
    {
        List<(int x, int y)> path = new List<(int x, int y)>();

        int dx = Math.Abs(endX - startX);
        int dy = -Math.Abs(endY - startY);

        int sx = startX < endX ? 1 : -1;
        int sy = startY < endY ? 1 : -1;

        int err = dx + dy;
        int x = startX;
        int y = startY;

        while (true)
        {
            path.Add((x, y)); // Добавляем текущую точку в путь

            if (x == endX && y == endY) // Достигли цели
                break;

            int e2 = 2 * err;

            if (e2 >= dy) // Горизонтальное движение
            {
                if (x == endX)
                    break;
                err += dy;
                x += sx;
            }

            if (e2 <= dx) // Вертикальное движение
            {
                if (y == endY)
                    break;
                err += dx;
                y += sy;
            }
        }

        return path;
    }

    public class DamageCalculator
{
    // Проверяет, находится ли цель за укрытием
    public static float IsTargetCovered(
        Agent attackerPos, 
        Agent targetPos)
    {
        foreach (var cover in CoverPositions)
        {
            // 1. Проверка, что атакующий не соседствует с укрытием
            if (IsAdjacent(attackerPos, cover.x, cover.y))
                continue;

            // 2. Проверка, что атакующий не на одной оси с укрытием
            if (IsOnSameAxis(attackerPos, targetPos, cover))
                continue;

            // 3. Проверка, что укрытие находится между атакующим и целью
            if (IsCoverBetween(attackerPos, targetPos, cover))
                return cover.damageModification; // Возвращаем модификатор урона укрытия
        }
        return 1f; // Если укрытие не найдено, возвращаем 1 (без модификации урона)
    }

        private static void March(int sx, int sy, int ex, int ey)
        {
            for(int x = sx; int )
        }

    // Проверка соседства (включая диагонали)
        private static bool IsAdjacent(Agent pos1, int x, int y)
        {
            return Math.Abs(pos1.x - x) <= 1 && Math.Abs(pos1.y - y) <= 1;
        }

    // Проверка нахождения на одной оси (горизонталь/вертикаль)
    private static bool IsOnSameAxis(Agent attacker, Agent target, (int x, int y, float damageModification) cover)
    {
        // Горизонтальная ось
        if (attacker.y == target.y && attacker.y == cover.y)
            return true;
        
        // Вертикальная ось
        if (attacker.x == target.x && attacker.x == cover.x)
            return true;
        
        return false;
    }

    // Проверка, что укрытие находится между атакующим и целью
    private static bool IsCoverBetween(Agent attacker, Agent target, (int x, int y, float damageModification) cover)
    {
        // Проверка по X
        if ((attacker.x < cover.x && cover.x < target.x) || (attacker.x > cover.x && cover.x > target.x))
            return true;
        
        // Проверка по Y
        if ((attacker.y < cover.y && cover.y < target.y) || (attacker.y > cover.y && cover.y > target.y))
            return true;
        
        return false;
    }

}

    public static int MyId;
    public static Dictionary<int, Agent> Friendlies = new Dictionary<int, Agent>();
    public static Dictionary<int, Agent> Hostile = new Dictionary<int, Agent>();
    public static Dictionary<int, Agent> AllAgents = new Dictionary<int, Agent>();
    public static List<(int x, int y, float damageModification)> CoverPositions = new List<(int x, int y, float damageModification)>();
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
                if (tileType == 1) // Cover tile
                {
                    CoverPositions.Add((x, y, 0.5f));
                }
                else if (tileType == 2) // Water tile
                {
                    CoverPositions.Add((x, y, 0.75f));
                }
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
                curr.plannedWetness = curr.wetness;
            }
            int myAgentCount = int.Parse(Console.ReadLine()); // Number of alive agents controlled by you
            foreach (var agent in Friendlies.Values.OrderBy(x => x.id))
            {
                agent.WritePrefix();
                agent.Tree.Execute();
                agent.EndCommand();
            }
            foreach (var enemy in Hostile.Values.ToArray())
            {
                if (enemy.isDead)
                {
                    Hostile.Remove(enemy.id);
                }
            }
        }
    }
}