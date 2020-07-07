# PL0编译器设计——C++实现

## `getsym.h`: 词法分析
    
状态转化图
    
![image][link]
    
词法分析就是将代码文件中的字符通过状态转化图所示的语法规则读出单词即可。

为了表示单词的种类，定义如下枚举类型

```cpp
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

接下来的工作就是读取字符，因为编译器本身不需要额外的输入，并且为了使用cin的get()函数，所以在读取之前，将cin重定向至文件

```cpp
    streambuf *stdin_backup = cin.rdbuf();         //备份标准输入流
    const char *fileinname = "../demo.txt";
    fcin.open(fileinname);
    if (!fcin.is_open()) {
        cout << "failed to open " << fileinname << endl;
        return 0;
    }
    cin.rdbuf(fcin.rdbuf());                //将cin重定向到文件
```

## `block.h`: 语法分析+语法树生成+目标代码生成
 
## `interpret.h`：解释执行
 
## `main.cpp`：主函数

参考资料：[北航 编译实践 PL/0文法 - 姜楠 - 博客园](https://www.cnblogs.com/ZJUT-jiangnan/p/3494501.html)

[link]:https://github.com/xueweiwujxw/run_PL0_CPP/blob/master/%E7%8A%B6%E6%80%81%E8%BD%AC%E5%8C%96%E5%9B%BE.png