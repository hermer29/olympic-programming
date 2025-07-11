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
        map<int, int> sorted_window;

        // Записываем в map всё окно и считаем вхождения
        for(int j = 0; j < k; j++) {
            if(sorted_window.find(nums[j]) != sorted_window.end()) {
                sorted_window[nums[j]]++;
            } else {
                sorted_window[nums[j]] = 1;
            }
        }
        results.push_back((int) (*sorted_window.rbegin()).first);

        for(int i = 1; i < n - k + 1; i++) {
            int deleting = nums[i - 1];
            int newElem = nums[i + k - 1];
            if(sorted_window[deleting] == 1) {
                sorted_window.erase(deleting);
            } else {
                sorted_window[deleting]--;
            }
            if(sorted_window.find(newElem) != sorted_window.end()) {
                sorted_window[newElem]++;
            } else {
                sorted_window[newElem] = 1;
            }
            results.push_back((int) (*sorted_window.rbegin()).first);
        }
        return results;
    }
};