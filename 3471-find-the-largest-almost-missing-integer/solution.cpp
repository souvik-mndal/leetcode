class Solution {
public:
    bool counting( vector<int>nums , int st , int end , int target ){
        
        for( int i=st ; i<=end ; i++ ){
            if( nums[i] == target ){
                return 1;
            }
        }
        return 0;
    }
    int largestInteger(vector<int>& nums, int k) {
        vector<int>store(nums.size());
        for( int i=0 ; i<nums.size() ; i++ ){
            int st=0 , end=k;
            int total = counting(nums , st , end-1 , nums[i]);
            st++;
            while(end<nums.size() ){
                total+=counting(nums , st , end , nums[i]);
                end++;
                st++;
            }
            store[i]=total;
        }
        for( auto it:store ){
            cout<<it<<" ";
        }
        int ans = -1;
        for( int i=0 ; i<store.size() ; i++ ){
            if( store[i] == 1 && ans < nums[i]){
                ans = nums[i];
            }
        }
        return ans;
    }
};