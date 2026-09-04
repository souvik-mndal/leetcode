class Solution {
public:
    void minimum( vector<int>&arr , int &mini , int j){
        for( int i=j ; i<arr.size() ; i++ ){
            mini = min( mini , arr[i]);
        }
    }
    int firstStableIndex(vector<int>& nums, int k) {
        int maxi = nums[0];
        int mini = nums[0];
        int ans = INT_MAX;
        int ind = -1;
        for( int i=0 ; i<nums.size() ; i++ ){
            maxi = max(maxi , nums[i]);
            mini = nums[i];
            minimum(nums , mini , i);
            int val = maxi - mini;
            if( val <= k && val<ans ){
                ans = val;
                return i;
            }
        }
        return -1;
    }
};