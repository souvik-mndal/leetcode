class Solution {
public:
    vector<int> resultArray(vector<int>& nums) {
        vector<int>ans;
        vector<int>a2;
        for( auto i:nums ){
            if( ans.size()==0 ){
                ans.push_back(i);
                continue;
            }
            if( a2.size()==0 ){
                a2.push_back(i);
                continue;
            }
            if( ans.back()>a2.back() ){
                ans.push_back(i);
            }
            else{
                a2.push_back(i);
            }
        }
        for( auto it:ans ){
            cout<<it<<" ";
        }
        cout<<endl;
        for( auto it:a2 ){
            cout<<it<<" ";
        }
        for( auto it:a2 ){
            ans.push_back(it);
        }
        return ans;
    }
};