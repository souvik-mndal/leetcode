class Solution {
public:
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>>ans;
        int total = 1 << nums.size();
        for( int i=0 ; i<total ; i++ ){
            int num = i;
            vector<int>temp;
            for( int j=0 ; j<nums.size() ; j++ ){
                if( num & ( 1 << j )){
                    temp.push_back(nums[j]);
                }
            }
            ans.push_back(temp);
        }
        return ans;
    }
};