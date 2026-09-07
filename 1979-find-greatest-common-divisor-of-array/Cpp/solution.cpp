class Solution {
public:
    int gcd( int a , int b ){
        while( a != b ){
            if( a>b ){
                a=a-b;
            }
            else{
                b=b-a;
            }
        }
        return a;
    }
    int findGCD(vector<int>& nums) {
        int maxi=-1,mini=INT_MAX;
        for( int i=0 ; i<nums.size() ; i++ ){
            maxi=max(maxi,nums[i]);
            mini=min(mini,nums[i]);
        }
        return gcd(maxi,mini);
    }
};