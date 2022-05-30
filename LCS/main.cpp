#include <iostream>
using namespace std;
void getLCS_Length();
string getLCS();
const int subStringLengthA = 12;
const int subStringLengthB = 12; // 支持的最长字符串的有效字符数, 自行设置
const char LEFTUP = '`'; // means"左上方"
const char UP = '^'; // means"上方"
const char LEFT = '<'; // means"左方"
int lens[subStringLengthA + 1][subStringLengthB + 1] = {0}; // 记录子问题x[i]和y[i]的LCSLength
char marks[subStringLengthA + 1][subStringLengthB + 1] = {0}; // 方向标记
string A, B;
int main() {
    int tmp_len;
    cin >> A;
    cin >> B;
    A = string(1, ' ').append(A.substr(0, A.size() + 1)); // A[0]=' ', A[1...]有效
    B = string(1, ' ').append(B.substr(0, B.size() + 1)); // B[0]=' ', B[1...]有效
    tmp_len = (int)A.size();
    if(tmp_len < subStringLengthA + 1)
        for(int i = 0; i < subStringLengthA - tmp_len; i++)
            A = A.append(string(1, 'w')); // 长度不足则以该缺省值补充
    tmp_len = (int)B.size();
    if(tmp_len < subStringLengthB + 1)
        for(int i = 0; i < subStringLengthB - tmp_len; i++)
            B = B.append(string(1, 'w'));
    for(int i = 0; i <= subStringLengthA; i++)
        for(int j = 0; j <= subStringLengthB; j++)
            marks[i][j] = '*';
    getLCS_Length();
    cout << "lens[i][j]:" << endl;
    for(int i = 0; i <= subStringLengthA; i++){
        for(int j = 0; j <= subStringLengthB; j++)
            cout << lens[i][j] << ' ';
        cout << endl;
    }
    cout << "marks[i][j]:" << endl;
    for(int i = 0; i <= subStringLengthA; i++){
        for(int j = 0; j <= subStringLengthB; j++)
            cout << marks[i][j] << ' ';
        cout << endl;
    }
    cout << "LCS:" << endl << getLCS() << endl;
    return 0;
}

void getLCS_Length(){
    int m = subStringLengthA;
    int n = subStringLengthB;
    for(int i = 1; i <= m; i++)
        for(int j = 1; j <= n; j++)
            if(A[i] == B[j]){
                lens[i][j] = lens[i - 1][j - 1] + 1;
                marks[i][j] = LEFTUP;
            }else if(lens[i - 1][j] >= lens[i][j - 1]){
                lens[i][j] = lens[i - 1][j];
                marks[i][j] = UP;
            }else{
                lens[i][j] = lens[i][j - 1];
                marks[i][j] = LEFT;
            }
}

string getLCS(){
    string ans;
    int m = subStringLengthA;
    int n = subStringLengthB;
    while(m > 0 && n > 0) {
        switch (marks[m][n]) {
            case LEFTUP:
                ans.append(string(1, A[m]));
                m--;
                n--;
                break;
            case UP:
                m--;
                break;
            case LEFT:
                n--;
                break;
            default:
                break;
        }
    }
    reverse(ans.begin(), ans.end());
    return ans;
}