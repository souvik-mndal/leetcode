class Solution {
public:
    int romanToInt(string s) {
        string roman[] = {"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
        int value[] = {1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1};
        int i=0,j=1;
        int num=0;
        while(i<s.length()){
            int value1=0,value2=0;
            string check1=s.substr(i,1);
            for( int x=0 ; x<13 ; x++){
                
                if(roman[x]==check1){
                    value1=value[x];
                    break;
                }
            }
            if(j<s.length()){string check=s.substr(j,1);
                for( int x=0 ; x<13 ; x++){
                    
                    if(roman[x]==check){
                        value2=value[x];
                        break;
                    }
                }
            }
            if(value1>value2){
                num=num+value1;
                i++;j++;
            }
            if(value2>value1){
                num=num+(value2-value1);
                i+=2;j+=2;
            }
            if(value1==value2){
                num=num+value1+value2;
                i+=2;j+=2;
            }
        }
        return num;
    }
};