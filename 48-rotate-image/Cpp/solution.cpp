class Solution {
public:
    void rev( vector<vector<int>>& matrix,int s,int e,int i){
        while(s<e){
            swap(matrix[i][s++],matrix[i][e--]);
        }
    }
    void rotate(vector<vector<int>>& matrix) {
        for( int i=0 ; i<matrix.size() ; i++){
            for( int j=i+1 ; j<matrix[i].size() ; j++){
                swap( matrix[i][j],matrix[j][i]);
            }
        }
        for( int i=0 ; i<matrix.size(); i++){
            rev(matrix,0,matrix[i].size()-1,i);
        }
    }
};