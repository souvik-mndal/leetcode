class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        int n = nums.size();
        int cnt = 0;
        sort( nums.begin() , nums.end());
        for( int i=0 ; i<n ; i++ ){
            int rem = nums[i] + k;
            if( i>0 && nums[i] == nums[i-1] ){
                continue;
            }
            if( binary_search(nums.begin()+i+1 , nums.end() , rem )){
                cnt++;
            }
        }
        return cnt;
    }
};