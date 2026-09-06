class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        int size = digits.size()-1;
        vector<int>ans_vec;
        int carry = 0;
        int sum;
        while( size >= 0 ){
            if( carry == 0 ){
                sum = digits[size] + 1 + carry;
            }
            else{
                sum = digits[size] + carry ; 
            }
            ans_vec.push_back( sum % 10 );
            carry = sum / 10 ;
            size--;
            if( carry == 0 ){
                break;
            }
        }
        if( carry ){
            ans_vec.push_back( carry );
        }
        while( size >= 0 ){
            ans_vec.push_back( digits[size--]);
        }
        reverse( ans_vec.begin() , ans_vec.end());
        return ans_vec;
    }
};