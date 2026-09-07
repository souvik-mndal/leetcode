class Solution {
public:
    int maximumLength(vector<int>& nums) {
        unordered_map<long long , int >mp;
        for( auto &it:nums ){
            mp[it]++;
        }
        int maxi = 0;
        for( auto &it:mp ){
            int count = 0;
            long long curr = it.first;
            while( mp.find(curr) != mp.end() ){
                if( curr == 1 ){
                    count = it.second;
                    maxi = max(count , maxi);
                    break;
                }
                if( mp[curr] >= 2 ){
                    count+=2;
                    curr = curr * curr;
                }
                else{
                    count++;
                    maxi = max(count , maxi);
                    break;
                }
                maxi = max(count , maxi);
            }
        }
        if( !(maxi & 1) ){
            return maxi-1;
        }
        return maxi;
    }
};