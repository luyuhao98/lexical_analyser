#include<string>
#include<iostream>
using namespace std;
int main()
{
        double 0_p balabala; 
        /*  非法符号0_p,
        报错且跳过balabala进入下一行  */
        
        double a = 0x.Fap-10; //符号+正确的十六进制浮点数

        //保留字 + 正确的十六进制浮点数 
        auto aa = 0x3.Fap+10; 
        //非法数字
        double b = 0x; 
        //缺位的十六进制浮点数
        double c = 0x.; 
        //十六进制浮点数缺少指数
        double d = 0x1b3.; 
        //十六进制浮点数指数缺位
        double e = 0x.Fap; 
        //正确的八进制数
        int f = 0071; 
        //非法的八进制数
        int g = 0091; 
        //合法的十进制浮点数
        double h= 012.34; 
        //合法的十进制整数
        int i = 0; 
        //合法的十进制浮点数
        int j = .123; 
        //合法的十进制带指数浮点数
        double k = 9.123E-999; 
        //十进制浮点数指数缺位
        double l = 9.123e; 

        int m= 7 , n = 15;
        m +=  a >> 11;
        n &= (1^1) ? f : i ;
        string o = "i\'m \t a \"lex\" \n"  
        char p = '\x72';
        if (p == 'r'){
                for (int i = 0 ; i<3;i++)
                cout<<6;
        }
        string q = "this is \n a correct string" ;
        string r = "this is a 
                        error string ";
        
}/* this is an il leg al en d
