class Solution {
public:
    int maximumElementAfterDecrementingAndRearranging(vector<int>& arr) {
        sort( arr.begin() , arr.end() );
        if( arr[0] != 1 ){
            arr[0]=1;
        }
        for( int j=1 ; j<arr.size() ; j++ ){
            if( abs(arr[j]-arr[j-1]) > 1 ){
                arr[j] = arr[j-1]+1;
            }
        }
        return *max_element( arr.begin() , arr.end());
    }
};