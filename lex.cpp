#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <streambuf>
#include <typeinfo>
#include <iomanip>

using namespace std;
/*保留字*/
string reserves[32] = {"auto", "break", "ase", "char", "const", "continue", "default",
                       "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int",
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
int row = 1, column = 0;
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
                str.push_back(in);
                return true;
        }
        return false;
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
                if (!str.compare(reserves[i]))
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
        str.clear();
}
void outtable(tables t)
{
        vector<string> *table = nullptr;
        string outstr;
        switch (t)
        {
        case ID:
                table = &symbol_talbe;
                outstr = "ID TABLE:";
                break;
        case NUM:
                table = &num_talbe;
                outstr = "NUM TABLE: ";
                break;
        case STR:
                table = &string_table;
                outstr = "STR TABLE ";
                break;
        default:
                outstr = "outtable error";
                exit(0);
        }
        o << outstr << endl;
        vector<string>::iterator walker = table->begin();
        for (; walker < table->end(); walker++)
        {       
                o << walker - table->begin()<<'\t'<< *walker  << endl;
        }
}

void outerror(const char *error)
{
        o << row << "," << column - str.length()+1 << ":" << error << ":" << str<<endl;
        while (!(in == '\r' || in == '\n') && goforward());
        goback();
        str.clear();
}

void outopt()
{
        o << str << ",-" << endl;
        str.clear();
}
/*-------------------------------------------*/

int main()
{
        input.push_back('\n');
        int state = 0; //默认状态为0
        while (goforward())
        {

                switch (state)
                {
                case 0: //默认状态
                        if ((in >= 'A' && in <= 'Z') || (in >= 'a' && in <= 'z') || (in == '_'))
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
                        { //其他特殊符号
                                switch (in)
                                {
                                case '.':
                                        state = 16;
                                        break;
                                case '+':
                                case '&':
                                case '|':
                                        if (goforward() && !(in == str[0] || in == '='))
                                        {
                                                goback();
                                        }
                                        outopt();
                                        break;
                                case '-':
                                        if (goforward() && !(in == '-' || in == '=' || in == '>'))
                                        {
                                                goback();
                                        }
                                        outopt();
                                        break;
                                case '*':
                                case '%':
                                case '=':
                                case '!':
                                case '^':
                                        if (goforward() && !(in == '='))
                                        {
                                                goback();
                                        }
                                        outopt();
                                        break;
                                case '/':
                                        if (goforward())
                                        {
                                                if (in == '=')
                                                        outopt();
                                                else if (in == '/')
                                                {
                                                        str.clear();
                                                        state = 17;
                                                }

                                                else if (in == '*')
                                                {
                                                        str.clear();
                                                        /*???*/ state = 18;
                                                }
                                        }
                                        break;
                                case '~':
                                case '?':
                                case ',':
                                case ';':
                                case '(':
                                case ')':
                                case '[':
                                case ']':
                                case '{':
                                case '}':
                                        outopt();
                                        break;
                                case ' ':
                                case '\t':
                                        str.clear();
                                        break;
                                case '\r':
                                        if (goforward() && in != '\n')
                                        {
                                                goback(); //win系统\r\n换行 ，mac\r换行
                                        }
                                        row++;
                                        column = -1;
                                        str.clear();
                                        break;
                                case '\n': // linux系统 \n换行
                                        row++;
                                        column = -1;
                                        str.clear();
                                        break;
                                case ':':
                                        if (goforward() && !(in == ':'))
                                        {
                                                goback();
                                        }
                                        outopt();
                                        break;
                                case '>':
                                case '<':
                                        state = 20;
                                        break;
                                case '\'':
                                        state = 22;
                                        str.pop_back();//删掉‘
                                        break;
                                case '\"':
                                        state = 27;
                                        str.pop_back();//删掉“
                                        break;
                                default:
                                        outerror("stray in program");
                                        state =0;
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
                                goback();
                                int flag = isreserved();
                                if (flag != -1){ //保留字>=0
                                        o << str << ',' << flag << endl;
                                        str.clear();
                                }
                                else
                                {
                                        
                                        intable(ID);
                                }
                                state = 0;
                        }
                        break;
                case 2: //0  八进制 or 十六进制
                        if (in == 'x' || in == 'X')
                        {
                                state = 3; //十六进制 十六进制浮点数必须带指数p (g++)
                        }
                        else if (in >= '0' && in <= '7')
                        {
                                state = 10; //八进制 八进制无浮点数，带浮点视为十进制 (g++)
                        }
                        else if (in =='.')
                        {
                                state = 12;
                        }
                        /*此外，只要加'.' 无论后面是否跟数字or指数都视为浮点数。如0xA. 报错：十六进制浮点数必须带指数*/
                        else
                        {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 3: //0x
                        if ((in >= '0' && in <= '9') || (in >= 'A' && in <= 'F') || (in >= 'a' && in <= 'f'))
                        {
                                state = 4; //0xA
                        }
                        else if (in = '.')
                        {
                                state = 5; //0x.
                        }
                        else
                        {
                                outerror("invalid num");
                                state = 0;
                        }
                        break;

                case 4: //0xA
                        if ((in >= '0' && in <= '9') || (in >= 'A' && in <= 'F') || (in >= 'a' && in <= 'f'))
                        {
                                state = 4; //0xABC123
                        }
                        else if (in == '.')
                        {
                                state = 6; //0xABC123.
                        }
                        else if(in == 'p'|| in =='P'){
                                state = 7; //0xAbcp
                        }
                        else if((in >='g'&&in<='z')||(in>='G'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 5: //0x.
                        if ((in >= '0' && in <= '9') || (in >= 'A' && in <= 'F') || (in >= 'a' && in <= 'f'))
                        {
                                state = 6; //0x.B
                        }
                        else if((in >='g'&&in<='z'&&in!='p')||(in>='G'&&in<='Z'&&in!='P')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                outerror("no digits in hexadecimal floating constant");
                                state = 0;
                        }
                        break;
                case 6: //0xA. or 0x.B
                        if ((in >= '0' && in <= '9') || (in >= 'A' && in <= 'F') || (in >= 'a' && in <= 'f'))
                        {
                                state = 6; //0xA2.F23
                        }
                        else if (in == 'p' || in == 'P')
                        {
                                state = 7; //0xA2.F23p
                        }
                        else if((in >='g'&&in<='z')||(in>='G'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        
                        else
                        {
                                outerror("hexadecimal floating constants require an exponent");
                                state = 0;
                        }
                        break;
                case 7: //0xA2.23p or 0x.bc12p or 0xABCp
                        if (in >= '0' && in <= '9')
                        {                  //p后只支持十进制
                                state = 8; //0xA2.23p1
                        }
                        else if (in == '-' || in == '+')
                        {
                                state = 9; //0xA2.23p-
                        }
                        else if((in >='a'&&in<='z')||(in>='A'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                outerror("exponent has no digits");
                                state = 0;
                        }
                        break;
                case 8: //0xA2.23p1 or //0xA2.23p-1
                        if (in >= '0' && in <= '9')
                        {
                                state = 8; //0xA2.23p123 or //0xA2.23p-123
                        }
                        else if((in >='a'&&in<='z')||(in>='A'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 9: //0xA2.23p-
                        if (in >= '0' && in <= '9')
                        {
                                state = 8; //0xA2.23p-1
                        }
                        else if((in >='a'&&in<='z')||(in>='A'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        
                        else
                        {
                                outerror("exponent has no digits");
                                state = 0;
                        }
                        break;
                case 10: //八进制
                        if (in >= '0' && in <= '7')
                        {
                                state = 10;
                        }
                        else if (in == '.')
                        {
                                state = 12; //八进制 八进制无浮点数，带浮点视为十进制 (g++)
                        }
                        else if (in >='8' && in <='9')
                        {       
                                outerror("invalid digit in octal constant");
                                state = 0;
                        }
                        else if((in >='a'&&in<='z')||(in>='A'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 11: //1
                        if (in >= '0' && in <= '9')
                                state = 11; //1234
                        else if (in == '.')
                                state = 12; //1234.
                        else if (in == 'e'|| in == 'E')
                                state = 13;////123e
                        else if((in >='a'&&in<='z')||(in>='A'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 12: //123.   0.
                        if (in >= '0' && in <= '9')
                                state = 12; //123.456
                        else if (in == 'e' || in == 'E')
                                state = 13; //123.456e
                        else if((in >='a'&&in<='z')||(in>='A'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 13: //123.456e or 123e
                        if (in >= '0' && in <= '9')
                                state = 14; //123.456e7
                        else if (in == '-' || in == '+')
                                state = 15; //123.456e-
                        else if((in >='a'&&in<='z')||(in>='A'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                outerror("exponent has no digits");
                                state = 0;
                        }
                        break;
                case 14: //123.456e7 or 123.456e-7
                        if (in >= '0' && in <= '9')
                                state = 14; //123.456e789 or 123.456e-789
                        else if((in >='a'&&in<='z')||(in>='A'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                goback();
                                intable(NUM);
                                state = 0;
                        }
                        break;
                case 15: //123.456e-
                        if (in >= '0' && in <= '9')
                                state = 14; //123.456e-7
                        else if((in >='a'&&in<='z')||(in>='A'&&in<='Z')||in =='-'){
                                outerror("expected unqualified-id before numeric constant");
                                state = 0;
                        }
                        else
                        {
                                outerror("exponent has no digits");
                                state = 0;
                        }
                        break;
                case 16: //.
                        if (in >= '0' && in <= '9')
                                state = 12; //小数模式
                        else
                        {
                                goback();
                                outopt();
                                state = 0;
                        }
                        break;
                case 17: // //
                        if (in == '\r' || in == '\n')
                        {
                                goback();
                                o << "Comment:" << str<<endl;;
                                str.clear();
                                state = 0;
                        }
                        break;
                case 18: // /*
                        if (in == '*')
                                state = 19;
                        break;
                case 19: // /*balabala*
                        if (in == '/')
                        {
                                str.pop_back();
                                str.pop_back(); // 弹出*/ 同前面弹出/*
                                o << "Comment:" << str<<endl;
                                str.clear();
                                state = 0;
                        }
                        else
                                state = 18;
                        break;
                case 20: // > <
                        if (in == '=')
                        {
                                outopt();
                                state = 0;
                        }
                        else if (in == str[0])
                        {
                                state = 21;
                        }
                        else
                        {
                                goback();
                                outopt();
                                state = 0;
                        }
                        break;
                case 21: // <<
                        if (in != '=')
                                goback(); //<<3
                        outopt();
                        state = 0;
                        break;
                case 22: // char 模式 '（已经删掉） str="{in}"
                        if (in != '\\')
                                /* \ */
                                state = 23;
                        else {
                                state = 24; // a
                        } 
                        break;
                case 23:// 'mode: \ //
                        char tra;
                        switch(in){// 'mode \a //
                                case 'a' : tra='\a';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case 'b' : tra='\b';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case 'f' : tra='\f';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case 'n' : tra='\n';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case 'r' : tra='\r';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case 't' : tra='\t';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case 'v' : tra='\v';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case '\\': tra='\\';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case '\'' : tra='\'';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case '\"' : tra='\"';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                case '\?' : tra='\?';str.pop_back();str.pop_back();str.push_back(tra);state =24;break;
                                                
                                case 'x':// \x
                                        state = 25;
                                default:
                                        outerror("illegal char!");
                                        state =0;
                        }
                        break;
                case 24:// 'over mode : a{in}
                        if (in == '\'')// a' \'回单引号
                        {
                                str.pop_back();// a
                                intable(STR);
                                
                        }
                        else {//ab
                                outerror("illegal char!");
                        }
                        state = 0;
                case 25:// \x{in}
                        if((in>='0'&&in<='9')||(in>='a'&&in<='f')||(in>='A'&&in<='F'))
                                state =26;
                        else {
                                outerror("illegal char!");
                                state =0;
                        }
                        break;
                case 26: // \xa{in}
                        if((in>='0'&&in<='9')||(in>='a'&&in<='f')||(in>='A'&&in<='F')){
                        // \xaf
                                state = 24;   
                        }
                        else if(in =='\''){
                        // \xa'  
                                str.pop_back(); // \xa
                                intable(STR);
                        }
                        else {
                                outerror("illegal char!");
                                state =0;
                        }
                        break;
                case 27: // string mode "(被删掉) str="{in}"
                        if (in == '\\')//str = "asdasdasd\"
                                state = 28;
                        else if (in == '\"')//str = "ababababa""
                                {
                                        str.pop_back();
                                        intable(STR);
                                        state = 0;
                                }
                        else if (in == '\r'|| in == '\n'){
                                goback();
                                outerror("missing terminating \" character");
                                state = 0;
                        }
                        else state =27;
                        break;
                case 28:// asfasdfasdf\{in}
                        char traa;
                        switch(in){
                                case 'a' : traa='\a';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case 'b' : traa='\b';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case 'f' : traa='\f';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case 'n' : traa='\n';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case 'r' : traa='\r';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case 't' : traa='\t';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case 'v' : traa='\v';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case '\\': traa='\\';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case '\'' : traa='\'';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case '\"' : traa='\"';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                case '\?' : traa='\?';str.pop_back();str.pop_back();str.push_back(traa);state =27;break;
                                default:
                                        outerror("illegal char!");
                                        state =0;
                        }
                        break;

                }
        }
        if (!str.empty())
        {
                o << row << ":" << "illegal end!" << endl;
        }
        row--;//平衡开始增添的\n
        outtable(ID);
        outtable(NUM);
        outtable(STR);
        cout << "Num of rows:  " << row << endl;
        
}
