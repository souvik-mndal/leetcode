class Solution {
public:
    vector<int> findMissingElements(vector<int>& nums) {
        vector<int>ans;
        sort( nums.begin(),nums.end());
        int toFind = nums[0];
        int counter = 0;
        while( counter < nums.size()){
            if( nums[counter] == toFind ){
                toFind++;
                counter++;
            }
            else{
                ans.push_back(toFind);
                toFind++;
            }
        }
        return ans;
    }
};