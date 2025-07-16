//https://habr.com/ru/companies/selectel/articles/794658/

#include <iostream>
#include <vector>
#include <cmath>
#include <random>

#ifdef _WIN32
#include <Windows.h>
#endif

#define C 1.414f
#define ITER_COUNT 15

enum Actor {
    PLAYER,
    BOT
};

struct Node {
    int stones_remaining;
    Actor player;
    
    Node* parent = nullptr;
    int visits = 0;
    int wins = 0;
    std::vector<Node*> children = std::vector<Node*>();

    bool is_terminal() {
        return stones_remaining == 0;
    }
};

/*
Получается тут несколько кейсов
- Детей максимум только 2 - который отнимает один и который отнимает два камня
*/
Node* expansion(Node* selected) {
    if(selected->is_terminal())
        return selected;

    int n = selected->children.size();
    if(n == 2) {
        return selected;
    }
    auto stones_remaining = selected->stones_remaining;
    if(stones_remaining == 1 && n == 1) {
        return selected;
    }
    Node* new_node = new Node();
    new_node->player = selected->player == PLAYER ? BOT : PLAYER;
    new_node->parent = selected;
    // Будем добавлять детей сначала отнимающие 1, а затем 2. Таким образом это условие сработает
    if(stones_remaining >= 2 && n == 1) { // кейс если уже добавлен путь, при котором мы забираем один камень
        new_node->stones_remaining = stones_remaining - 2;
    } else if (n == 0) { // кейс если ещё не добавлен путь, при котором мы забираем один камень
        new_node->stones_remaining = stones_remaining - 1;
    }
    if(new_node->player == BOT && new_node->stones_remaining == 0) {
        new_node->wins = 100;
    }
    selected->children.push_back(new_node);
    return new_node;
}

Actor simulation(Node* new_node) {
    auto stones_remaining = new_node->stones_remaining;
    if(stones_remaining == 0)
        return new_node->player;
    Actor curr = new_node->player;
    static std::random_device rd;  // Источник энтропии (может быть аппаратным)
    static std::mt19937 gen(rd());  // Генератор (Mersenne Twister)
    static std::uniform_int_distribution<int> dist(1, 2);

    while(true) {
        int randint = dist(gen);
        curr = curr == PLAYER ? BOT : PLAYER;
        if(stones_remaining - 1 == 0 || stones_remaining - 2 == 0) {
            return curr;
        }
        stones_remaining -= randint;
    }    
    return PLAYER; // мы сюда по идее никогда не придем
}

void backpropogation(Node* new_node, Actor winner) {
    Node* curr = new_node;
    while(curr != nullptr) {
        curr->visits++;
        if(winner == BOT) {
            curr->wins++;
        }
        curr = curr->parent;
    }
}

float calc_uct(const Node& node) {
    return ((float)node.wins / node.visits) + C * (sqrt(log(node.parent->visits) / node.visits));
}

Node* selection(Node* current) {
    int n = current->children.size();
    if(n == 0 || current->is_terminal()) {
        return current;
    }
    if(n == 1) {
        return current->children[0];
    }
    float max_uct = -1.0;
    int max_uct_child; // не должен быть пустым
    for(int i = 0; i < n; i++) {
        auto curr = current->children[i];
        if(curr->visits == 0)
            return curr;
        float cur_uct = calc_uct(*curr);
        if(cur_uct > max_uct) {
            max_uct_child = i;
            max_uct = cur_uct;
        }
    }
    return current->children[max_uct_child];
}

Node* mcst(Node* current) {
    if(current->is_terminal())
        return nullptr;
    auto selected = selection(current);
    if(selected->is_terminal())
        return selected;
    auto new_node = expansion(selected);
    if(!new_node->is_terminal()) {
        backpropogation(new_node, simulation(new_node));
    }
    return selected;
}

int main() {
    #ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    #endif

    int whos_first;
    std::cout << "Кто ходит первый? 1 - ты, 2 - бот" << std::endl;
    std::cin >> whos_first;

    Node initial;
    initial.stones_remaining = 4;
    initial.player = whos_first == 1 ? PLAYER : BOT;

    while(true) { // games loop
        Node* current = &initial;
        Actor player = current->player;
        for(int mcst_i = 0; mcst_i < ITER_COUNT; ++mcst_i) {
            mcst(current);
        }
        while(true) { // one match loop
            std::cout << "Осталось " << current->stones_remaining << " камней." << std::endl;
            if(current->is_terminal()) {
                std::cout << "Игра окончена! Победил " << (current->player == BOT ? "Бот!" : "Игрок!");
                break;
            }
            if(player == BOT) {
                Node* best;
                for(int mcst_i = 0; mcst_i < ITER_COUNT; ++mcst_i) {
                    best = mcst(current);
                }
                std::cout << "Бот забрал " << (current->stones_remaining - best->stones_remaining) << " камней."; 
                current = best;
            } else {
                int how_much = 0;
                do {
                    std::cout << "Сколько заберём камней?" << std::endl;
                    std::cin >> how_much;
                    std::cout << std::endl;
                } while(how_much > current->stones_remaining && how_much <= 0);
                Node* chosen_child = nullptr;
                for(auto child : current->children) {
                    if(child->stones_remaining == current->stones_remaining - how_much) {
                        chosen_child = child;
                        break;
                    }
                }
                if (!chosen_child) {
                    chosen_child = new Node();
                    chosen_child->stones_remaining = current->stones_remaining - how_much;
                    chosen_child->player = (current->player == PLAYER) ? BOT : PLAYER;
                    chosen_child->parent = current;
                    current->children.push_back(chosen_child);
                }
                current = chosen_child;
            }
            player = player == PLAYER ? BOT : PLAYER;
        }
    }
}

