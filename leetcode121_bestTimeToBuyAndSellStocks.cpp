#include "includes.cpp"

class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int min, minIndex = -1, 
            profit = 0;
        int localProfit;

        for(int i = 0; i < prices.size(); i++){
            if(minIndex == -1 || min > prices[i]) {
                minIndex = i;
                min = prices[i];
            }
            if(prices[i] - min > profit) {
                profit = prices[i] - min;
            } 
        }

        return profit;
    }
};