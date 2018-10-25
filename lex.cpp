#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <streambuf>
#include <typeinfo>

using namespace std;
/*保留字*/
string reserves[32] = {"auto", "break", "ase", "char", "const", "continue", "default",
                       "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int,",
                       "long", "register", "return", "short", "signed", "sizeof", "static", "struct",
                       "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
/*输入区*/
ifstream t("input.txt");
string input((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
/*标志位*/
int i_input = -1;
/*存放临时字符串*/
string str = "";
/*读头字符in*/
char in = input[i_input];
/*行号*/
int row = 0, column = -1;
/*行数*/
int num_row = 0;

/*输出区*/
ofstream o("output.txt");

/*符号表*/
vector<string> symbol_talbe;
/*常数表*/
vector<string> num_talbe;
/*字符串表*/
vector<string> string_table;

/*----------------函数区-------------*/
bool goforward()
{

        if (++i_input < input.length())
        {
                in = input[i_input];
                column++;
                return 1;
                str.push_back(in);
        }
        return 0;
}

void goback()
{
        in = input[--i_input];
        str.pop_back();
        column--;
}

int isreserved()
{
        for (int i = 0; i < 32; i++)
        {
                if (str == reserves[i])
                        return i;
        }
        return -1;
}
enum tables
{
        ID,
        NUM,
        STR
};
/*返回表内id，若无此变量，则压入*/
void intable(tables t)
{       
        vector<string> *table = nullptr;
        string outstr;
        switch (t)
        {
        case ID:
                table = &symbol_talbe;
                outstr = "ID, ";
                break;
        case NUM:
                table = &num_talbe;
                outstr = "NUM, ";
                break;
        case STR:
                table = &string_table;
                outstr = "STR, ";
                break;
        default:
                cout << "intable error";
                exit(0);
        }
        int id;
        vector<string>::iterator walker = find(table->begin(), table->end(), str);
        id = walker - table->begin();
        if (walker == table->end())
        {
                table->push_back(str);
        }
        o << outstr << id << endl;
}

void outerror(const char *error)
{
        o << row << "," << column - str.length() << ":" << error << ":" << str;
        exit(0);
}
/*-------------------------------------------*/

int main()
{
        int state = 0; //默认状态为0
        while (goforward())
        {
               
                switch (state)
                {
                case 0:              //默认状态
                        str.clear(); //每次归 state 0 时清空
                        if (in == ' ' || in == '\t')
                                state = 0; //其他字符 如空格tab
                        else if ((in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z') || (in == '_'))
                        {                  //大小写字母下划线开头
                                state = 1; //标示符
                              
                        }
                        else if (in == '0')
                        { //八进制2 十六进制3
                                state = 2;
                                
                        }
                        else if (in >= '1' && in <= '9')
                        { //十进制4
                                state = 11;
                        }
                        else
                        {//其他特殊符号
                                switch (in)
                                {
                                case '.':
                                        state = 16;
                                        break;
                                
                                }
                        }
                        break;

                case 1: //符号状态
                        if ((in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z') || (in >= '0' && in <= '9') || in == '_')
                        {
                                state = 1;
                        }
                        else
                        {
                               
                                int flag = isreserved();
                                if (flag != -1) //保留字>=0
                                        o << str << ',' << flag << endl;
                                else
                                {
                                        intable(ID);
                                }
                                state = 0;
                        }
                        break;
                case 2: //八进制 or 十六进制
                        if (in == 'x'|| in =='X')
                        {
                                state = 3; //十六进制 十六进制浮点数必须带指数p (g++) 

                        }
                        else if (in >= '0' && in <= '7')
                        {
                                state = 10; //八进制 八进制无浮点数，带浮点视为十进制 (g++)
                        
                        }
                        /*此外，只要加'.' 无论后面是否跟数字or指数都视为浮点数。如0xA. 报错：十六进制浮点数必须带指数*/
                        else
                        {       goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 3://0x
                        if ((in>='0'&&in<='9')||(in>='A'&&in<='F')||(in>='a'&&in<='f')){
                                state = 4;//0xA
                              
                        }
                        else if (in = '.'){
                                state = 5;//0x.
                        }
                        else {
                                outerror("invalid num");
                        }
                        break;
                
                case 4://0xA
                        if ((in>='0'&&in<='9')||(in>='A'&&in<='F')||(in>='a'&&in<='f')){
                                state = 4;//0xABC123
                        }
                        else if (in = '.'){
                                state =6;//0xABC123.
                        }
                        else {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 5: //0x.
                        if ((in>='0'&&in<='9')||(in>='A'&&in<='F')||(in>='a'&&in<='f')){
                                state = 6;//0x.B
                        }
                        else {
                                outerror("no digits in hexadecimal floating constant");
                        }
                        break;
                case 6://0xA. or 0x.B
                        if ((in>='0'&&in<='9')||(in>='A'&&in<='F')||(in>='a'&&in<='f')){
                                state = 6;//0xA2.F23
                        }
                        else if (in == 'p'||in == 'P')
                        {
                                state = 7;//0xA2.F23p
                        }
                        else {
                                outerror("hexadecimal floating constants require an exponent");
                        }
                        break;
                case 7://0xA2.23p
                        if (in>='0'&&in<='9'){//p后只支持十进制
                                state = 8;//0xA2.23p1
                        }
                        else if (in == '-'|| in =='+'){
                                state = 9;//0xA2.23p-
                        }
                        else {
                                outerror("exponent has no digits");
                        }
                        break;
                case 8://0xA2.23p1 or //0xA2.23p-1
                        if(in>='0'&&in<='9'){
                                state =8;//0xA2.23p123 or //0xA2.23p-123
                        }
                        else {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 9://0xA2.23p-
                        if(in >='0'&&in<='9'){
                                state = 8;//0xA2.23p-1
                        }
                        else{
                                outerror("exponent has no digits");
                        }
                        break;
                case 10://八进制
                        if(in>='0'&&in<='7'){
                                state = 10;
                        }
                        else if(in =='.')
                        {
                                state = 12;//八进制 八进制无浮点数，带浮点视为十进制 (g++)
                        }
                        else{
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 11://1
                        if(in>='0'&&in<='9')
                                state = 11;//1234
                        else if (in == '.')
                                state = 12;//1234.
                        else {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 12://123.
                        if(in >='0'&& in<='9')
                                state = 12;//123.456
                        if (in == 'e'|| in == 'E')
                                state = 13;//123.456e
                        else {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 13://123.456e
                        if(in >= '0'&& in <= '9')
                                state = 14;//123.456e7
                        if(in == '-'&& in == '+')
                                state = 15;//123.456e-
                        else {
                                outerror("exponent has no digits");
                        }
                        break;
                case 14://123.456e7 or 123.456e-7
                        if(in >='0'&&in<='9')
                                state = 14;//123.456e789 or 123.456e-789
                        else {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 15://123.456e-
                        if (in>='0'&& in<='9')
                                state = 14;//123.456e-7
                        else {
                                outerror("exponent has no digits");
                        }
                        break;
                case 16://.
                        if(in >='0'&& in <='9')
                                state = 12;//小数模式
                        else {
                                goback();
                                o<<str<<",-"<<endl;//运算符
                        }
                        break;
                }
        }

        cout << typeid(input[i_input]).name() << endl;
}
