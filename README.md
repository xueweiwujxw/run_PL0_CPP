# PL0编译器设计——C++实现

## `getsym.h`: 词法分析
    
状态转化图
    
![image][link] 
> 词法分析就是将代码文件中的字符通过状态转化图所示的语法规则读出单词即可。
>
> 为了表示单词的种类，定义如下枚举类型
```c++
enum SYM_class {
    //关键字
    CONST_SYM,              //const
    VAR_SYM,                //var
    PROCEDURE_SYM,          //procedure
    BEGIN_SYM,              //begin
    END_SYM,                //end
    ODD_SYM,                //odd
    IF_SYM,                 //if
    THEN_SYM,               //then
    CALL_SYM,               //call
    WHILE_SYM,              //while
    DO_SYM,                 //do
    READ_SYM,               //read
    WRITE_SYM,              //write
    //算符与界符
    ADD_SYM,                // +
    SUB_SYM,                // -
    MULT_SYM,               // *
    DIV_SYM,                // /
    EQUAL_SYM,              // =
    NEQUAL_SYM,             // #
    LESS_SYM,               // <
    ELESS_SYM,              // <=
    GRE_SYM,                // >
    EGRE_SYM,               // >=
    LEFTP_SYM,              // (
    RIGHTP_SYM,             // )
    DOT_SYM,                // .
    COMMA_SYM,              // ,
    ASSI_SYM,               // :=
    SEMICOLON_SYM,          // ;
    NUM_SYM,                //数字
    IDE_SYM                 //标识符
};
```
> 因为编译器本身不需要额外的输入，并且为了使用cin的get()函数，所以在读取之前，将cin重定向至文件
```c++
    streambuf *stdin_backup = cin.rdbuf();         //备份标准输入流
    const char *fileinname = "../demo.txt";
    fcin.open(fileinname);
    if (!fcin.is_open()) {
        cout << "failed to open " << fileinname << endl;
        return 0;
    }
    cin.rdbuf(fcin.rdbuf());                //将cin重定向到文件
```
> 恢复输入流的操作
```c++
    cin.rdbuf(stdin_backup);
    fcin.close();
```
> 完成了准备工作之后，通过cin.get()读取单个字符，读取的过程中需要过滤空格、tab、换行符，并根据换行符的数量统计出当前的行数。
> 读取到文件结束符结束。
```c++
    char ch;                //存放最新读进的源程序字符
    ch = cin.get();
    while(ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
        if (ch == '\n')
            lineSum++;
        ch = cin.get();
    }                       //过滤空格、tab、换行符，并对换行符进行统计，以便于定位出错位置。
    if (ch == EOF) {
        return "finish";
    }
```
> 读取的字符根据状态转化图进行判断，合成单词并记录，下面是标识符、关键字的例子。
```c++
    if (isalpha(ch)) {      //ch是字母，则单词可能是标识符或者关键字
        do {
            strToken += ch;
            ch = cin.get();
        }while (isalpha(ch) || isdigit(ch));
        id = strToken;
        if (id.size() > 10) {         //关键词与标识符的大小限制为10
            error(TOOLANG);
            return "error";
        }
        bool isKey = false;
        for (int i = 0; i < 13; ++i)       //查找是否属于关键字
            if (id == keywords[i]) {
                isKey = true;
                SYM_type = i;
                break;
            }
        if (isKey) {                         //关键字的输出
            ;//print_result(SYM_type);
        }
        else {                              //标识符的输出
            SYM_type = IDE_SYM;
            insertIdentifier(id);
            //print_result<string>(SYM_type, id, insertIdentifier(id));
//#if DEBUG
            debugId = id;
//#endif
            id = "identifier";
        }
        fcin.seekg(-1,ios::cur);    //退回读取的多余的字符
        return id;
    }
```
> 剩余单词的处理基本一致
>
> 在词法分析阶段，可能会存在标识符过长、赋值号错误、不合法的单词以及错误的数字等错误单词，需要针对其进行报错处理
```c++
enum ERROR_CODE {
    TOOLANG,
    ASSI_ERROR,
    ANOTHER_WORD,
    WRONG_NUMBER
};
```

## `block.h`: 语法分析+语法树生成+目标代码生成
 
## `interpret.h`：解释执行
 
## `main.cpp`：主函数

参考资料：[北航 编译实践 PL/0文法 - 姜楠 - 博客园](https://www.cnblogs.com/ZJUT-jiangnan/p/3494501.html)

[link]:https://github.com/xueweiwujxw/run_PL0_CPP/blob/master/%E7%8A%B6%E6%80%81%E8%BD%AC%E5%8C%96%E5%9B%BE.png