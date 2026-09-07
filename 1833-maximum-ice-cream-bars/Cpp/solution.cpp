class Solution {
public:
    int maxIceCream(vector<int>& costs, int coins) {
        int cnt = 0;
        vector<int>arr(*(max_element(costs.begin(),costs.end()))+1);
        for( int i=0 ; i<costs.size() ; i++ ){
            arr[costs[i]]++;
        }
        for( int i=0 ; i<arr.size() ; i++ ){
            if( arr[i] == 0 ) continue;
            if( i > coins ){
                break;
            }
            int d = min(coins / i , arr[i]);
                coins = coins - ( d * i );
                cnt+=d;
        }
        return cnt;
        
    }
};