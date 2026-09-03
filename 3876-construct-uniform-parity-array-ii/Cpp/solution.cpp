class Solution {
public:
    bool uniformArray(vector<int>& nums1) {
        sort( nums1.begin() , nums1.end());
        int val = nums1[0]%2;
        bool allSame = true;
        for( int i=1 ; i<nums1.size() ; i++ ){
            if( val != nums1[i]%2 ){
                allSame = false;
                break;
            }
        }
        if( allSame ){
            return true;
        }
        int minOdd = 0, minEven = 0;
        for( int i=0 ; i<nums1.size() ; i++ ){
            if( nums1[i]%2 == 0 && minEven == 0 ){
                minEven = nums1[i];
            }
            if( nums1[i]%2 == 1 && minOdd == 0 ){
                minOdd = nums1[i];
            }
            if( minOdd != 0 && minEven != 0 ){
                break;
            }
        }
        vector<int>arr(nums1.size());
        arr[0]=nums1[0];
        for( int i=1 ; i<nums1.size() ; i++ ){
            if( nums1[i]%2 == val ){
                arr[i]=nums1[i];
            }
        }
        for( int i=1 ; i<arr.size() ; i++ ){
            if( arr[i] == 0 ){
                if( nums1[i]%2 == 0 && val == 1){
                    arr[i] = nums1[i]-minOdd;
                }
                if( nums1[i]%2 == 1 && val == 0){
                    arr[i] = nums1[i]-minEven;
                }
            }
        }
        allSame = true;
        for( int i=1 ; i<arr.size() ; i++ ){
            if( val != arr[i]%2 ){
                allSame = false;
                break;
            }
        }
        if( allSame ){
            return true;
        }
        return false;
    }
};