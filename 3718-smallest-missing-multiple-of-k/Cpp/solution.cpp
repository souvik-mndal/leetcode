class Solution {
public:
    int missingMultiple(vector<int>& nums, int k) {
        sort( nums.begin() , nums.end());
        int val = k;
        for( auto it:nums ){
            if( it == val ){
                val+=k;
            }
            else if( it > val ){
                return val;
            }
        }
        return val;
    }
};