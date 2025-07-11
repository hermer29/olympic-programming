#include "./includes.cpp"

struct CacheEntry {

};

class LRUCache {
public:
    LRUCache(int capacity) {
        
    }
    
    int get(int key) {
        access_count[key]++;
    }
    
    void put(int key, int value) {
        access_count.
    }

private:
    priority_queue<int, int> access_count;
};