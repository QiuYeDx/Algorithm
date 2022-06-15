#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
using namespace std;
struct Web{
    int id, level;
};
string str[1000000];
string str2[1000000];
string input_word[10000];
int input_word_id[10000];
//int levels[10000];
Web webs[10000];
int num_input, num_legal_input, num_page;
int length;
int ls[1000000] = { 0 };
void quickSort(int s[], int l, int r);

class Message {
public:
    Message() = default;
public:
    int id{};//网址编号
    string website;//网址
    string headline;//网址标题
};

class Word {
public:
    Word() = default;
public:
    int id_word{};//单词编号
    int id_website{};//网址编号
};

class Word_str {
public:
    Word_str()= default;
public:
    int bian{};//单词编号
    int we_bian{};//单词的网页编号
    string str_word;//单词内容
};
vector<Word_str> jack;//存储单词编号文件的内容
vector<Message> mary;//存储页面文件里的内容：网页编号、网址、文章标题
int num_word = 0;//单词个数
int web_id = 0;//网址标号


void read() {
    jack.reserve(100000);
    mary.reserve(100000);
    Message mes;
    Word wd;
    ifstream myFile;
    ofstream data, words;
    myFile.open("news.csv", ios_base::in);
    data.open("data.txt", ios_base::out);
    words.open("words.txt", ios_base::out);
    //ios::out：如果没有文件，那么生成空文件；如果有文件，清空文件
    if (!myFile || !data) {
        cout << "news.csv或者data.txt文件打开失败！请重试！" << endl;
    }
    else {
        string s, s1, s2;
        while (getline(myFile, s)) {
            if (s[0] == 'h' && s[1] == 't' && s[2] == 't' && s[3] == 'p' && s[4] == ':') {
                web_id++;//网页编号
                int t = s.find(',');
                s1 = s.substr(0, t);
                //cout << s1 << endl;
                int g = s.size() - t - 3;
                s2 = s.substr(t+1, g);
                //cout << s2 << endl;
                mes.id = web_id;
                mes.website = s1;
                mes.headline = s2;
                mary.push_back(mes);//每次获得的信息都存储到vector数组里
                s1.clear();
                s2.clear();
                //data.write((char*)&mes, sizeof(Message));
                data << mes.id << " " << mes.website << " " << mes.headline << endl;
                continue;
            }


            int k = 0;
            string s3("");
            while ( k < (s.size()-1) ) {
                if ((s[k] >= 'a' && s[k] <= 'z') || (s[k] >= 'A' && s[k] <= 'Z') || (s[k] == '-') ) {
                    s3 += s[k];
                    ++k;
                    while ((s[k] >= 'a' && s[k] <= 'z') || (s[k] >= 'A' && s[k] <= 'Z') || (s[k] == '-')) {
                        s3 += s[k];
                        ++k;
                    }
                    //此时K指向各种空格、标点符号
                    int d = 0;//标志
                    for (vector<Word_str>::iterator it = jack.begin(); it < jack.end(); it++) {
                        if ( ((*it).str_word == s3)  && ((*it).we_bian != web_id) ) {   //新网页
                            wd.id_word = (*it).bian;//找到相同的单词
                            wd.id_website = web_id;
                            (*it).we_bian = web_id;//更新这个单词的网页编号
                            s3.clear();//用完之后清零
                            words << wd.id_word << " " << wd.id_website << endl;
                            d = 1;
                            break;
                        }
                        else if ( ((*it).str_word == s3) && ((*it).we_bian == web_id) ) {  //网页没改变,则不用输出到临时索引文件
                            s3.clear();//用完之后清零
                            d = 1;
                            break;
                        }
                    }

                    if (d == 0) {   //未找到相同的单词
                        num_word++;
                        wd.id_word = num_word;
                        Word_str str;
                        str.bian = wd.id_word;
                        str.str_word = s3;
                        str.we_bian = web_id;//单词第一次出现时的网页编号
                        jack.push_back(str);
                        s3.clear();//用完之后清零
                        wd.id_website = web_id;
                        words << wd.id_word << " " << wd.id_website << endl;
                    }
                    while (s[k] == ',' || s[k] == ' ' || s[k] == '"' || s[k] == '\n' || s[k] == '.' || s[k] == '(' || s[k] == ')' || s[k] == '_' || (s[k] >= '0' && s[k] <= '9')) {
                        ++k;
                    }
                }
                else {
                    ++k;
                }
            }
        }
        myFile.close();
        words.close();
        data.close();
        cout << "页面文件导出成功！" << endl;
        cout << "临时索引文件导出成功！" << endl;
    }
}

void OUT_wordfile() {
    ofstream w;
    w.open("bian_word.txt",ios::out);
    if (!w) {
        cout << "bian_word.txt文件打开失败！请重试！" << endl;
    }
    for (vector<Word_str>::iterator it = jack.begin(); it < jack.end(); it++) {
        w << (*it).bian << " " << (*it).str_word << endl;
    }
    w.close();
    cout << "单词编号文件导出成功！" << endl;
}


void read1()
{
    ifstream ifs;
    ifs.open("words.txt", ios::in);
    if (!ifs.is_open())
    {
        cout << "文件打开失败" << endl;
        return;
    }
    int i = 0;

    while (getline(ifs, str[i]))
    {
        i++;
    }
    length = i;
    ifs.close();
}

void change()
{
    string temp;
    int i = 0, j = 0;
    for (i = 0; i < length; i++)
    {
        for (j = 0; str[i][j] != ' '; j++)
        {
            ls[i] = ls[i] * 10 + (str[i][j] - '0');
        }
    }

    quickSort(ls, 0, length - 1);

}

void quickSort(int s[], int l, int r)
{
    if (l < r)
    {
        int i = l, j = r, x = s[l];
        string temp;
        temp = str[l];
        while (i < j)
        {
            while (i < j && s[j] >= x) // 从右向左找第一个小于x的数
                j--;
            if (i < j)
            {
                s[i] = s[j];
                str[i] = str[j];
                i++;
            }
            while (i < j && s[i] < x) // 从左向右找第一个大于等于x的数
                i++;
            if (i < j)
            {
                s[j] = s[i];
                str[j] = str[i];
                j--;
            }
        }
        s[i] = x;
        str[i] = temp;
        quickSort(s, l, i - 1); // 递归调用
        quickSort(s, i + 1, r);
    }
}

void change_p()
{
    int i = 0, j = 0, q = ls[0];
    str2[0] = str[i];
    int p1 = 0, p2 = 0, p3 = 0;
    for (i = 1; i < length; i++)
    {
        if (ls[i] == q)
        {
            str2[j].append(1, ',');
            p1 = str[i].find(' ');
            p2 = str[i].size();
            p3 = p2 - p1;
            str2[j].append(str[i], p1 + 1, p3 - 1);
        }
        else
        {
            q = ls[i];
            j++;
            str2[j] = str[i];
        }

    }

}

void write()
{
    ofstream ofs;
    ofs.open("wordsplus.txt", ios::out);
    if (!ofs.is_open())
    {
        cout << "文件打开失败" << endl;
        return;
    }
    int i = 0;

    for (i = 0; i < length; i++)
    {
        if(str2[i]!="\0"){
            ofs << str2[i] << endl;
        }
    }
    cout<<"倒排序文件导出成功！"<<endl;
    ofs.close();
}

void Go() {
    read1();
    change();
    change_p();
    write();
}

// getAns() subFunc:
// string input_word[1000000];
// int num_input;
void getInput(){
    num_input = 0;
    string input;
    getline(cin, input);
    int length = input.size();
    int left, right;
    for(int i = 0; i < length;){
        while(input[i] == ' ')
            i++;
        left = i;
        while(input[i] != ' ')
            i++;
        right = i;
        input_word[num_input++] = input.substr(left, right - left);
    }
//    *** debug ***
//    cout << "num_input: " << num_input << endl;
//    for(int i = 0; i < num_input; i++){
//        cout << input_word[i] << endl;
//    }
    int n = 0;
    for(int i = 0; i < num_input; i++){
        for (vector<Word_str>::iterator it = jack.begin(); it < jack.end(); it++) {
            if(it->str_word == input_word[i]){
                input_word_id[n++] = it->bian;
                break;
            }
        }
    }
    num_legal_input = n;

//    *** debug ***
//    cout << "num_legal_input: " << num_legal_input << endl;
}

void addLevel(int id_word){
    string tmp = str2[id_word - 1];
    string tmp_web_id_str;
    int tmp_web_id = 0;
    int left = 0;
    int i = 0;
    while(tmp[i] != ' ')
        i++;
    left = ++i;
    while(i < tmp.size()){
        while(tmp[i] != ',' && i < tmp.size())
            i++;
        tmp_web_id_str = tmp.substr(left, i - left);
//        cout << "tmp_web_id_str: " << tmp_web_id_str << endl;
        tmp_web_id = atoi(tmp_web_id_str.c_str());
//        cout << "tmp_web_id: " << tmp_web_id << endl;
        webs[tmp_web_id].level++;
        left = ++i;
    }
}

void getLevel(){
    num_page = (int)mary.size();
//    cout << "num_page:" << num_page << endl;
    for(int i = 0; i < num_legal_input; i++){
        addLevel(input_word_id[i]);
    }
//    *** debug ***
//    for(int i = 0; i < num_page; i++){
//        cout << webs[tmp_web_id].level << endl;
//    }
}

void rangeLevel(){
    Web tmp;
    for(int i = 1; i <= num_page; i++){
        webs[i].id = i;
    }
    for(int i = 1; i <= num_page; i++){
        for(int j = i + 1; j <= num_page; j++){
            if(webs[i].level < webs[j].level){
                tmp = webs[i];
                webs[i] = webs[j];
                webs[j] = tmp;
            }else if(webs[i].level == webs[j].level && webs[i].id > webs[j].id){
                tmp = webs[i];
                webs[i] = webs[j];
                webs[j] = tmp;
            }
        }
    }
}

void outputAns(){
    cout << "查询结果如下:" << endl;
    for(int i = 1; i <= num_page; i++){
//        *** debug ***
//        cout << "webs[i].id: " << webs[i].id << endl;
        if(webs[i].level == 0){
//            *** debug ***
//            cout << "break;" << endl;
            break;
        }
        cout << "标题:" << mary[webs[i].id - 1].headline << endl << "网址:" << mary[webs[i].id - 1].website << endl << endl;
    }
//    cout << webs[4717].id << webs[4717].level << endl;
}

void getAns(){
    getInput();
    if(num_legal_input == 0)
        cout << "Warning: 未找到相关内容，请更换关键词。" << endl;
    else{
        getLevel();
        rangeLevel();
        outputAns();
    }
}

void re0(){
    for(int i = 0; i < 10000; i++){
        input_word[i] = "";
        input_word_id[i] = 0;
        webs[i].level = 0;
        webs[i].id = i;
    }
    num_input = 0;
    num_legal_input = 0;
}
int main()
{
    read();
    OUT_wordfile();
    Go();
    while(1){
        cout << "请输入查询关键字:";
        getAns();
        re0();
    }
    return 0;
}