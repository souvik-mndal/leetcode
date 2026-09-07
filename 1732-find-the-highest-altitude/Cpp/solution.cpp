class Solution {
public:
    int largestAltitude(vector<int>& gain) {
        int maxi = 0;
        int current = 0;
        for( auto &i:gain ){
            current += i;
            maxi = max( maxi , current );
        }
        return maxi;
    }
};