class Solution {
public:
    int minimumPushes(string word) {
        int count = 0;
        for( auto it:word ){
            if( count < 8 ) count++;
            else if( count < 24) {
                count += 2;
            }
            else if( count < 48) {
                count += 3;
            }
            else{
                count +=4;
            }
        }
        return count;
    }
};