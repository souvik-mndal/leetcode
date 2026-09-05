class Solution {
public:
    int firstStableIndex(vector<int>& nums, int k) {
        int maxi = nums[0];
        vector<int>store(nums.size());
        int str = nums[nums.size()-1];
        for(int j=nums.size()-1;j>=0;j--){
            str = min(str,nums[j]);
            store[j] = str;
        }
        int ans = INT_MAX;
        int ind = -1;
        for( int i=0 ; i<nums.size() ; i++ ){
            maxi = max(maxi , nums[i]);
            int val = maxi - store[i];
            if( val <= k && val<ans ){
                ans = val;
                return i;
            }
        }
        return -1;
    }
};