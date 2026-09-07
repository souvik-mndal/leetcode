class Solution {
public:
    int countMajoritySubarrays(vector<int>& nums, int target) {
        int total = 0;
        for( int i=0 ; i<nums.size() ; i++ ){
            int count = 0;
            for( int j=i ; j<nums.size() ; j++ ){
                if( nums[j] == target ){
                    count++;
                }
                if( count > (j-i+1)/2 ){
                    total++;
                }
            }
        }
        return total;
    }
};