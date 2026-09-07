class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        int row=matrix.size();
        int col = matrix[0].size();
        int sr=0,sc=0,er=row-1,ec=col-1;
        int tot=row*col;
        int count=0;
        vector<int>ans;
        while( count<tot){
            for( int i=sc;i<=ec && count<tot ; i++){
                ans.push_back(matrix[sr][i]);
                count++;
            }
            sr++;
            for( int i=sr;i<=er && count<tot ; i++){
                ans.push_back(matrix[i][ec]);
                count++;
            }
            ec--;
            for( int i=ec;i>=sc && count<tot ; i--){
                ans.push_back(matrix[er][i]);
                count++;
            }
            er--;
            for( int i=er;i>=sr && count<tot ; i--){
                ans.push_back(matrix[i][sc]);
                count++;
            }
            sc++;
            
        }
        return ans;
    }
};