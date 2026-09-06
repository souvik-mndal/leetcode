class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        if( nums.size() == 0 ){
            return 0;
        }
        sort( nums.begin() , nums.end() );
        int lng = 1 , cnt = 1 , ele = nums[0];
        for( int i=1 ; i<nums.size() ; i++ ){
            if( ele == nums[i] ){
                continue;
            }
            else if( ele+1 == nums[i] ){
                cnt++;
                ele = nums[i];
                lng = max( lng , cnt );
            }
            else{
                ele = nums[i];
                cnt=1;
            }
        }
        return lng;
    }
};