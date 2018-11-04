# 个人信息

姓名：卢昱昊　　　　　　　　　　　\
学号：2016210537　　　　　　　　\
班级：2016211304　　　　　　　　

# 文件目录：
lex.cpp　　　　　　　　　　　　　 词法分析程序\
lex　　　　　　　　　　　　　　　  Linux下可执行文件，由g++7.3.0编译\
lex.exe　　　　　　　　　　　　　 Win10下可执行文件，由vs2017编译\
input.txt　　　　　　　　　　　　 词法分析程序的输入，即源程序\
output.txt　　　　　　　　　　　 词法分析程序的输出\
text.cpp　　　　　　　　　　　　　用于编译测试效果,由　input.txt　+　相应头文件构成\
out　　　　　　　　　　　　　　　　\
｜——**document.html**　　　　　　　 **文档**，html格式,请用浏览器打开(推荐Chrome，不推荐IE)\
｜——[other　files]　　　　　　　文档的格式素材文件\
document　　　　　　　　　　　　　\
｜——document.mdk　　　　　　　　生成文档的madoko文件\
｜——g++.png　　　　　　　　　　　文档素材，g++7.3.0编译text.cpp效果图\
｜——state.dot　　　　　　　　　　生成状态转移图的graphviz文件\
｜——state.svg　　　　　　　　　　文档素材，状态转移图，由state.dot　生成\

# 使用方法

1. 编辑input.txt
2. 运行可执行文件　lex/lex.exe
3. 观察output.txt

# 修改文档　(前提是拥有madoko环境)
1. 编辑`document/document.mdk `
2. 回到主目录运行 `madoko -v document/document.mdk`
3. 观察`out/document.html`

# 修改状态图　(前提是配置好graphviz环境)
1. 编辑`document/states.dot`
2. 直接在`document`目录下运行 `dot -Tsvg states.dot -o state.svg`
3. 观察`state.svg`

# [Github](https://github.com/luyuhao98/lexical_analyser)
`ssh:　git@github.com:luyuhao98/lexical_analyser.git`
