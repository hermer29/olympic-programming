#include "./includes.cpp"

/*
    Кейсы
        Новый попавший в окно элемент больше ранее найденного максимального: он новый максимум
        Мы покинули максимальный элемент, максимальным становится второй по очереди. 
            Если новый элемент больше - он максимум.

*/

class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        int n = nums.size();
        vector<int> results;
        deque<int> dq;

        for(int i = 0; i < n; i++) {
            dq.push_back(i);

            if(!dq.empty() && dq.front() <= i - k) {
                dq.pop_front();
            }

            if()
        }
        return results;
    }
};