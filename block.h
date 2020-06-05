//
// Created by xww on 2020/5/25.
//

#ifndef RUN_PL0_BLOCK_H
#define RUN_PL0_BLOCK_H
#include "getsym.h"

enum OPR_TYPE {
    ReturnOpr,      //return
    /*
    (sp <- bp - 1;
    bp <- stack[sp + 2];
    pc <- stack[sp + 3];)
    */
    MinusOpr,       //取负
    /*
    (- stack[sp] )
    */
    AddOpr,         //加
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] + stack[sp + 1])
    */
    SubOpr,         //减
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] - stack[sp + 1])
    */
    MulOpr,         //乘
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] * stack[sp + 1])
    */
    DivOpr,         //除
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] / stack[sp + 1])
    */
    OddOpr,         //odd
    /*
    (stack[sp] <- stack % 2)
    */
    ModOpr,         //取模
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] % stack[sp + 1])
    */
    EquOpr,         //等于
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] == stack[sp + 1])
    */
    NeqOpr,         //不等于
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] != stack[sp + 1])
    */
    LessOpr,        //小于
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] < stack[sp + 1])
    */
    EgreOpr,        //大于等于
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] >= stack[sp + 1])
    */
    GreOpr,         //大于
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] > stack[sp + 1])
    */
    ElessOpr,       //小于等于
    /*
    (sp <- sp – 1 ;
    stack[sp] <- stack[sp] <= stack[sp + 1])
    */
    WriteOpr,       //write(输出栈顶)
    /*
    print (stack[sp]);
    sp <- sp – 1;
    */
    ChaLineOpr,     //输出换行
    /*
    print ('\n');
    */
    ReadOpr         //读入栈顶
    /*
    scan(stack[sp]);
    sp <- sp + 1;
    */
};

enum ERROR_GRAMMAR {
    const_state_end_wrong,
    number_wrong,
    const_assign_wrong,
    identifier_wrong,
    var_state_end_wrong,
    procedure_state_end_wrong,
    procedure_head_wrong,
    var_assign_wrong,
    expression_in_factor_wrong,
    if_then_wrong,
    condition_wrong,
    while_do_wrong,
    read_end_wrong,
    read_wrong,
    write_end_wrong,
    write_wrong,
    complex_end_wrong,
    wrong_program_end,
    var_undefined,
    var_redefined
};

int procedureLevel = 0;     //记录层数
int tx = 0;                 //名字表索引
int dx = 3;                 //每层的局部相对地址
int cx = 0;                 //代码集合的索引

struct treeNode {
    string element;
    vector<treeNode* > child;
    explicit treeNode(string e) {
        element = e;            //NOLINT
        for (auto & it : child)
            it = nullptr;
    }
};

struct tableEle {
    string name;
    string kind;
    int value;
    int level;
    int addr;
    tableEle(){
        name = "";
        kind = "";
        value = -1;
        level = -1;
        addr = -1;
    }
}; vector<tableEle> tables;         //符号表

int findAddrInTable(string &s)
{
    for (int i =(int)tables.size()-1; i >= 0; --i)
        if (s == tables[i].name && tables[i].level <= procedureLevel)
            return i;
    return -1;
}

void printTables()
{
    cout << "名字表" << endl;
    for (int i = 0; i < tables.size(); ++i) {
        cout << i << ": " << tables[i].name << " " << tables[i].kind << " ";
        if (tables[i].kind == "const")
            cout << tables[i].value << endl;
        else
            cout << tables[i].level << " " << tables[i].addr << endl;
    }

}

struct codeEle {
    string func;
    int level_d;
    int offset;
}; vector<codeEle> codes;           //中间代码表

void addCode(string str, int lev, int off)
{
    codeEle coe;
    coe.func = str;
    coe.level_d = lev;
    coe.offset = off;
    codes.push_back(coe);
    cx++;
}

void printCodes()
{
    cout << "目标代码序列" << endl;
    for (int i = 0; i < codes.size(); ++i)
        cout << i << ": " << codes[i].func << " " << codes[i].level_d << " " << codes[i].offset << endl;
}

void printTreeNode(treeNode *root, int level)
{
    if (root != nullptr) {
        for (int i = 0; i < level; ++i)
            cout << "|  ";
        cout << ">  " << root->element << endl;
        for (auto & it : root->child)
            printTreeNode(it, level+1);
    }
}

string sym;

bool error_grammar = false;

void error_exc(int error_grammar_type);               //错误处理
void BLOCK(treeNode* tn);                   //语法分析入口程序
void subBlock(treeNode* tn);                //分程序
void const_BLOCK(treeNode* tn);             //常量说明
void const_def(treeNode* tn);               //常量定义
int var_BLOCK(treeNode* tn);               //变量说明
void procedure_BLOCK(treeNode* tn);         //过程说明
void procedure_head(treeNode* tn);          //过程首部
void sen_BLOCK(treeNode* tn);               //语句
void assi_sen(treeNode* tn);                //赋值语句
void condition_sen(treeNode* tn);           //条件语句
void do_while_sen(treeNode* tn);            //当型循环语句
void proc_call_sen(treeNode* tn);           //过程调用语句
void read_sen(treeNode* tn);                //读语句
void write_sen(treeNode* tn);               //写语句
void complex_sen(treeNode* tn);             //复合语句
void expression(treeNode* tn);              //表达式
void condition(treeNode* tn);               //条件
void item(treeNode* tn);                    //项
void factor(treeNode* tn);                  //因子

void error_exc(int error_grammar_type)
{
    error_grammar = true;
    switch (error_grammar_type) {
        case const_state_end_wrong:
            cout << "常量说明结束错误 in line " << lineSum << endl;
            break;
        case number_wrong:
            cout << "无符号整数错误 in line " << lineSum << endl;
            break;
        case const_assign_wrong:
            cout << "常数赋值错误 in line " << lineSum << endl;
            break;
        case identifier_wrong:
            cout << "标识符错误 in line " << lineSum << endl;
            break;
        case var_state_end_wrong:
            cout << "变量说明结束错误 in line " << lineSum << endl;
            break;
        case procedure_state_end_wrong:
            cout << "过程说明结束错误 in line " << lineSum << endl;
            break;
        case procedure_head_wrong:
            cout << "过程首部错误 in line " << lineSum << endl;
            break;
        case var_assign_wrong:
            cout << "变量赋值错误 in line " << lineSum << endl;
            break;
        case expression_in_factor_wrong:
            cout << "因子中的表达式错误 in line " << lineSum << endl;
            break;
        case if_then_wrong:
            cout << "if_then语句错误 in line " << lineSum << endl;
            break;
        case condition_wrong:
            cout << "条件错误 in line " << lineSum << endl;
            break;
        case while_do_wrong:
            cout << "while_do语句错误 in line " << lineSum << endl;
            break;
        case read_end_wrong:
            cout << "read语句结束错误 in line " << lineSum << endl;
            break;
        case read_wrong:
            cout << "read语句错误 in line " << lineSum << endl;
            break;
        case write_end_wrong:
            cout << "write语句结束错误 in line " << lineSum << endl;
            break;
        case write_wrong:
            cout << "write语句错误 in line " << lineSum << endl;
            break;
        case complex_end_wrong:
            cout << "复合语句错误 in line " << lineSum << endl;
            break;
        case wrong_program_end:
            cout << "wrong program end" << endl;
            break;
        case var_undefined:
            cout << "有未定义的变量" << lineSum << endl;
            break;
        case var_redefined:
            cout << "重复定义的变量" << lineSum << endl;
            break;
        default:
            break;

    }
    exit(0);
}
void BLOCK(treeNode* tn)
{
    sym = GETSYM();
    auto *subBlockt = new treeNode("分程序");
    tn->child.push_back(subBlockt);
    subBlock(subBlockt);
    if (sym == ".") {
        tn->child.push_back(new treeNode("."));
        sym = GETSYM();
        //添加代码
        addCode("opr",0,ReturnOpr);
        //
        cout << "Grammar analysis complete" << endl;
        //cout << "the chart of integer has " << integerArray.size() << " items." << endl;
        //cout << "the chart of identifier has " << identifierArray.size() << " items" << endl;
    }
    else error_exc(wrong_program_end);
}

void subBlock(treeNode* tn)
{
    addCode("jmp",0,-1);        //跳转到语句部分
    int tmpCodeAddr = cx - 1;
    int varcount = 0;
    if (sym == "const") {
        auto *const_BLOCKt = new treeNode("常量说明部分");
        tn->child.push_back(const_BLOCKt);
        const_BLOCK(const_BLOCKt);              //常量说明
    }
    if (sym == "var") {
        auto *var_BLOCKt = new treeNode("变量说明部分");
        tn->child.push_back(var_BLOCKt);
        varcount = var_BLOCK(var_BLOCKt);                //变量说明
    }
    if (sym == "procedure") {
        auto *procedure_BLOCKt = new treeNode("过程说明部分");
        tn->child.push_back(procedure_BLOCKt);
        procedure_BLOCK(procedure_BLOCKt);          //过程说明
    }
    auto *sen_BLOCKt = new treeNode("语句");
    tn->child.push_back(sen_BLOCKt);
    //添加代码
    addCode("int",0,varcount+3);
    codes[tmpCodeAddr].offset = cx - 1;
    //
    sen_BLOCK(sen_BLOCKt);                //语句
}
void const_BLOCK(treeNode* tn)
{
    if (sym == "const") {
        tn->child.push_back(new treeNode("const"));
        sym = GETSYM();
        auto *const_deft = new treeNode("常量定义");
        tn->child.push_back(const_deft);
        const_def(const_deft);
        while (sym == ",") {
            tn->child.push_back(new treeNode(","));
            sym = GETSYM();
            auto *const_defts = new treeNode("常量定义");
            tn->child.push_back(const_defts);
            const_def(const_defts);
        }
        if (sym == ";") {
            tn->child.push_back(new treeNode(";"));
            sym = GETSYM();
        }
        else error_exc(const_state_end_wrong);           //常量说明的错误结束
    } else return;
}
void const_def(treeNode* tn)
{
    if (sym == "identifier") {
        auto *idt = new treeNode(debugId);
        tn->child.push_back(new treeNode("标识符"));
        tn->child.back()->child.push_back(idt);
        //添加名字表
        tableEle tabe;
        tabe.name = debugId;
        tabe.kind = "const";
        //
        sym = GETSYM();
        if (sym == "=") {
            tn->child.push_back(new treeNode("="));
            sym = GETSYM();
            if (sym == "number") {
                auto *numt = new treeNode(debugNum);
                tn->child.push_back(new treeNode("无符号整数"));
                tn->child.back()->child.push_back(numt);
                //添加名字表
                tabe.value = debugNumber;
                tables.push_back(tabe);
                tx++;
                //
                sym = GETSYM();
            }
            else error_exc(number_wrong);       //不是数字
        } else error_exc(const_assign_wrong);         //不是常量赋值
    } else error_exc(identifier_wrong);             //不是标识符
}
int var_BLOCK(treeNode* tn)
{
    if (sym == "var") {
        int varNumber = 0;
        tn->child.push_back(new treeNode("var"));
        sym = GETSYM();
        tableEle tabe;
        if (sym == "identifier") {
            auto *idt = new treeNode(debugId);
            tn->child.push_back(new treeNode("标识符"));
            tn->child.back()->child.push_back(idt);
            //添加名字表
            int labelAddr = findAddrInTable(debugId);
            if (labelAddr != -1)
                error_exc(var_redefined);
            tabe.name = debugId;
            tabe.kind = "var";
            tabe.addr = dx++;
            tabe.level = procedureLevel;
            tables.push_back(tabe);
            tx++;
            varNumber++;
            //
            sym = GETSYM();
        }
        else error_exc(identifier_wrong);           //不是标识符
        while (sym == ",") {
            tn->child.push_back(new treeNode("，"));
            sym = GETSYM();
            if (sym == "identifier") {
                auto *idt = new treeNode(debugId);
                tn->child.push_back(new treeNode("标识符"));
                tn->child.back()->child.push_back(idt);
                //添加名字表
                int labelAddr = findAddrInTable(debugId);
                if (labelAddr != -1)
                    error_exc(var_redefined);
                tabe.name = debugId;
                tabe.kind = "var";
                tabe.addr = dx++;
                tabe.level = procedureLevel;
                tables.push_back(tabe);
                tx++;
                varNumber++;
                //
                sym = GETSYM();
            }
            else error_exc(identifier_wrong);       //不是标识符
        }
        if (sym == ";") {
            tn->child.push_back(new treeNode(";"));
            sym = GETSYM();
        }
        else error_exc(var_state_end_wrong);           //变量说明的结束错误
        return varNumber;
    } else return -1;
}
void procedure_BLOCK(treeNode* tn)
{
    if (sym == "procedure") {
        //进入一个过程，增加层数，局部相对地址恢复为3
        procedureLevel++;
        dx = 3;
        auto *procedure_headt = new treeNode("过程首部");
        tn->child.push_back(procedure_headt);
        procedure_head(procedure_headt);
        auto *subBlockt = new treeNode("分程序");
        tn->child.push_back(subBlockt);
        subBlock(subBlockt);
        if (sym == ";") {
            tn->child.push_back(new treeNode(";"));
            //添加代码
            addCode("opr",0,ReturnOpr);
            procedureLevel--;   //过程结束，降低层数
            //
            sym = GETSYM();
            while (sym == "procedure") {
                auto *procedure_BLOCKt = new treeNode("过程说明部分");
                tn->child.push_back(procedure_BLOCKt);
                procedure_BLOCK(procedure_BLOCKt);
            }
        }
        else error_exc(procedure_state_end_wrong);           //过程说明的结束错误
    } else return;
}
void procedure_head(treeNode* tn)
{
    if (sym == "procedure") {
        tn->child.push_back(new treeNode("procedure"));
        sym = GETSYM();
        if (sym == "identifier") {
            auto *idt = new treeNode(debugId);
            tn->child.push_back(new treeNode("标识符"));
            tn->child.back()->child.push_back(idt);
            //添加名字表
            int labelAddr = findAddrInTable(debugId);
            if (labelAddr != -1)
                error_exc(var_redefined);
            tableEle tabe;
            tabe.name = debugId;
            tabe.kind = "procedure";
            tabe.addr = cx;
            tabe.level = procedureLevel-1;    //过程的层数应该隶属于过程的上一层
            tables.push_back(tabe);
            tx++;
            //
            sym = GETSYM();
            if (sym == ";") {
                tn->child.push_back(new treeNode(";"));
                sym = GETSYM();
            }
            else error_exc(procedure_head_wrong);       //过程首部结束错的
        } else error_exc(identifier_wrong);         //不是标识符
    } else return;
}
void sen_BLOCK(treeNode* tn)
{
    if (sym == "identifier") {
        auto *assi_sent = new treeNode("赋值语句");
        tn->child.push_back(assi_sent);
        assi_sen(assi_sent);
    }
    else if (sym == "if") {
        auto *condition_sent = new treeNode("条件语句");
        tn->child.push_back(condition_sent);
        condition_sen(condition_sent);
    }
    else if (sym == "while") {
        auto *do_while_sent = new treeNode("当型循环语句");
        tn->child.push_back(do_while_sent);
        do_while_sen(do_while_sent);
    }
    else if (sym == "call") {
        auto *proc_call_sent = new treeNode("过程调用语句");
        tn->child.push_back(proc_call_sent);
        proc_call_sen(proc_call_sent);
    }
    else if (sym == "read") {
        auto *read_sent = new treeNode("读语句");
        tn->child.push_back(read_sent);
        read_sen(read_sent);
    }
    else if (sym == "write") {
        auto *write_sent = new treeNode("写语句");
        tn->child.push_back(write_sent);
        write_sen(write_sent);
    }
    else if (sym == "begin") {
        auto *complex_sent = new treeNode("复合语句");
        tn->child.push_back(complex_sent);
        complex_sen(complex_sent);
    }
}
void assi_sen(treeNode* tn)
{
    if (sym == "identifier") {
        auto *idt = new treeNode(debugId);
        tn->child.push_back(new treeNode("标识符"));
        tn->child.back()->child.push_back(idt);
        string tmp = debugId;       //保存待赋值的变量
        sym = GETSYM();
        if (sym == ":=") {
            tn->child.push_back(new treeNode(":="));
            sym = GETSYM();
            auto *expressiont = new treeNode("表达式");
            tn->child.push_back(expressiont);
            expression(expressiont);
            //添加代码
            int labelAddr = findAddrInTable(tmp);
            if (labelAddr == -1)
                error_exc(var_undefined);
            int level_diff = abs(tables[labelAddr].level - procedureLevel);
            addCode("sto",level_diff,tables[labelAddr].addr);
            //
        } else error_exc(var_assign_wrong);         //不是变量赋值
    } else return;
}
void expression(treeNode* tn)
{
    if (sym == "+" || sym == "-" ) {
        tn->child.push_back(new treeNode(sym));
        string opr = sym;
        sym = GETSYM();
        //添加代码
        if (opr == "-")
            addCode("opr",0,MinusOpr);     //取负运算
        //
    }
    auto *itemt = new treeNode("项");
    tn->child.push_back(itemt);
    item(itemt);
    while (sym == "+" || sym == "-") {
        auto *idt = new treeNode(sym);
        tn->child.push_back(new treeNode("加减运算符"));
        tn->child.back()->child.push_back(idt);
        string opr = sym;       //暂存运算符
        sym = GETSYM();
        auto *itemts = new treeNode("项");
        tn->child.push_back(itemts);
        item(itemts);
        //添加代码
        if (opr == "+")
            addCode("opr",0,AddOpr);     //+运算
        else if (opr == "-")
            addCode("opr",0,SubOpr);     //-运算
        //
    }
}
void item(treeNode* tn)
{
    auto factort = new treeNode("因子");
    tn->child.push_back(factort);
    factor(factort);
    while (sym == "*" || sym == "/") {
        string opr = sym;
        auto *idt = new treeNode(sym);
        tn->child.push_back(new treeNode("乘除运算符"));
        tn->child.back()->child.push_back(idt);
        sym = GETSYM();
        auto factorts = new treeNode("因子");
        tn->child.push_back(factorts);
        factor(factorts);
        //添加代码
        if (opr == "*")
            addCode("opr",0,MulOpr);    //*运算
        else if (opr == "/")
            addCode("opr",0,DivOpr);    // /运算
        //
    }
}
void factor(treeNode* tn)
{
    if (sym == "identifier" || sym == "number") {
        string tmp;
        treeNode *idt;
        if (sym == "identifier") {
            tmp = "标识符";
            idt = new treeNode(debugId);
            //添加代码
            int labelAddr = findAddrInTable(debugId);
            if (labelAddr == -1)
                error_exc(var_undefined);
            if (tables[labelAddr].kind == "const")
                addCode("lit",0,tables[labelAddr].value);
            else if (tables[labelAddr].kind == "var")
                addCode("lod",abs(tables[labelAddr].level-procedureLevel), tables[labelAddr].addr);
            //
        }
        else {
            tmp = "无符号整数";
            idt = new treeNode(debugNum);
            //添加代码
            addCode("lit",0,debugNumber);
            //
        }
        tn->child.push_back(new treeNode(tmp));
        tn->child.back()->child.push_back(idt);
        sym = GETSYM();
    }
    else if (sym == "(") {
        tn->child.push_back(new treeNode("("));
        sym = GETSYM();
        auto *expressiont = new treeNode("表达式");
        tn->child.push_back(expressiont);
        expression(expressiont);
        if (sym == ")") {
            tn->child.push_back(new treeNode(")"));
            sym = GETSYM();
        }
        else error_exc(expression_in_factor_wrong);           //因子中包含的表达式的错误结束
    }
    else return;
}
void condition_sen(treeNode* tn)
{
    if (sym == "if") {
        tn->child.push_back(new treeNode("if"));
        sym = GETSYM();
        auto conditiont = new treeNode("条件");
        tn->child.push_back(conditiont);
        condition(conditiont);
        if (sym == "then") {
            tn->child.push_back(new treeNode("then"));
            sym = GETSYM();
            auto *sen_BLOCKt = new treeNode("语句");
            tn->child.push_back(sen_BLOCKt);
            //添加代码
            addCode("jpc",0,-1);    //地址等待回填
            int tmp = cx - 1;
            //
            sen_BLOCK(sen_BLOCKt);
            //语句结束之后回填
            codes[tmp].offset = cx;      //这里跳转的地址其实是if条件为假跳转的地址
            //
        } else error_exc(if_then_wrong);         //不是正确的if语句
    } else return;
}
void condition(treeNode* tn)
{
    if (sym != "odd") {
        auto *expressiont0 = new treeNode("表达式");
        tn->child.push_back(expressiont0);
        expression(expressiont0);
        if (sym == "=" || sym == "#" || sym == "<" || sym == "<=" || sym == ">" || sym == ">=") {
            string opr = sym;
            auto *idt = new treeNode(sym);
            tn->child.push_back(new treeNode("条件运算符"));
            tn->child.back()->child.push_back(idt);
            sym = GETSYM();
            auto *expressiont1 = new treeNode("表达式");
            tn->child.push_back(expressiont1);
            expression(expressiont1);
            //添加代码
            if (opr == "=")
                addCode("opr",0,EquOpr);     //=运算
            else if (opr == "#")
                addCode("opr",0,NeqOpr);     //#运算
            else if (opr == "<")
                addCode("opr",0,LessOpr);     //<运算
            else if (opr == "<=")
                addCode("opr",0,ElessOpr);     //<=运算
            else if (opr == ">")
                addCode("opr",0,GreOpr);     //>运算
            else if (opr == ">=")
                addCode("opr",0,EgreOpr);    //>=运算

            //
        } else error_exc(condition_wrong);         //不是正确的条件符
    }
    else {
        string opr = sym;
        tn->child.push_back(new treeNode("odd"));
        sym = GETSYM();
        auto *expressiont2 = new treeNode("表达式");
        tn->child.push_back(expressiont2);
        expression(expressiont2);
        //添加代码
        addCode("opr",0,OddOpr);     //odd运算
        //
    }
}
void proc_call_sen(treeNode* tn)
{
    if (sym == "call") {
        tn->child.push_back(new treeNode("call"));
        sym = GETSYM();
        if (sym == "identifier") {
            auto *idt = new treeNode(debugId);
            tn->child.push_back(new treeNode("标识符"));
            tn->child.back()->child.push_back(idt);
            //添加代码
            int labelAddr = findAddrInTable(debugId);
            if (labelAddr == -1)
                error_exc(var_undefined);
            int entryCode = codes[tables[labelAddr].addr].offset;
            addCode("cal", abs(tables[labelAddr].level-procedureLevel),entryCode);
            //
            sym = GETSYM();
        }
        else error_exc(identifier_wrong);           //不是标识符
    } else return;
}
void do_while_sen(treeNode* tn)
{
    if (sym == "while") {
        tn->child.push_back(new treeNode("while"));
        sym = GETSYM();
        int tmpCodeAddr = cx;
        auto *conditiont = new treeNode("条件");
        tn->child.push_back(conditiont);
        condition(conditiont);
        if (sym == "do") {
            tn->child.push_back(new treeNode("do"));
            sym = GETSYM();
            auto *sen_BLOCKt = new treeNode("语句");
            tn->child.push_back(sen_BLOCKt);
            //添加代码
            addCode("jpc",0,-1);    //while条件非真跳转
            int tmpCodeId = cx - 1;
            //
            sen_BLOCK(sen_BLOCKt);
            //添加代码
            addCode("jmp",0,tmpCodeAddr);   //跳转回while
            //回填非真跳转
            codes[tmpCodeId].offset = cx;
            //
        } else error_exc(while_do_wrong);         //不是正确的while_do语句
    } else return;
}
void read_sen(treeNode* tn)
{
    if (sym == "read") {
        tn->child.push_back(new treeNode("read"));
        //添加代码
        addCode("opr",0,ReadOpr);    //read运算
        //
        sym = GETSYM();
        if (sym == "(") {
            tn->child.push_back(new treeNode("("));
            sym = GETSYM();
            if (sym == "identifier") {
                auto *idt = new treeNode(debugId);
                tn->child.push_back(new treeNode("标识符"));
                tn->child.back()->child.push_back(idt);
                //添加代码
                int labelAddr = findAddrInTable(debugId);
                if (labelAddr == -1)
                    error_exc(var_undefined);
                addCode("sto", abs(tables[labelAddr].level - procedureLevel), tables[labelAddr].addr);
                //
                sym = GETSYM();
            }
            else error_exc(identifier_wrong);       //不是标识符
            while (sym == ",") {
                tn->child.push_back(new treeNode(","));
                //添加代码
                addCode("opr",0,ReadOpr);    //read运算
                //
                sym = GETSYM();
                if (sym == "identifier") {
                    auto *idt = new treeNode(debugId);
                    tn->child.push_back(new treeNode("标识符"));
                    tn->child.back()->child.push_back(idt);
                    //添加代码
                    int labelAddr = findAddrInTable(debugId);
                    if (labelAddr == -1)
                        error_exc(var_undefined);
                    addCode("sto", abs(tables[labelAddr].level - procedureLevel), tables[labelAddr].addr);
                    //
                    sym = GETSYM();
                }
            }
            if (sym == ")") {
                tn->child.push_back(new treeNode(")"));
                sym = GETSYM();
            }
            else error_exc(read_end_wrong);       //不是正确的read语句结束
        } else error_exc(read_wrong);         //不是正确的read语句
    } else return;
}
void write_sen(treeNode* tn)
{
    if (sym == "write") {
        tn->child.push_back(new treeNode("write"));
        sym = GETSYM();
        if (sym == "(") {
            tn->child.push_back(new treeNode("("));
            sym = GETSYM();
            auto *expressiont = new treeNode("表达式");
            tn->child.push_back(expressiont);
            expression(expressiont);
            //添加代码
            addCode("opr",0,WriteOpr);    //write运算
            addCode("opr",0,ChaLineOpr);
            //
            while (sym == ",") {
                tn->child.push_back(new treeNode(","));
                sym = GETSYM();
                auto *expressionts = new treeNode("表达式");
                tn->child.push_back(expressionts);
                expression(expressionts);
                //添加代码
                addCode("opr",0,WriteOpr);    //write运算
                addCode("opr",0,ChaLineOpr);
                //
            }
            if (sym == ")") {
                tn->child.push_back(new treeNode(")"));
                sym = GETSYM();
            }
            else error_exc(write_end_wrong);       //不是正确的write语句结束
        } else error_exc(write_wrong);         //不是正确的write语句
    } else return;
}
void complex_sen(treeNode* tn)
{
    if (sym == "begin") {
        tn->child.push_back(new treeNode("begin"));
        sym = GETSYM();
        auto *sen_BLOCKt = new treeNode("语句");
        tn->child.push_back(sen_BLOCKt);
        sen_BLOCK(sen_BLOCKt);
        while (sym == ";") {
            tn->child.push_back(new treeNode(";"));
            sym = GETSYM();
            auto *sen_BLOCKts = new treeNode("语句");
            tn->child.push_back(sen_BLOCKts);
            sen_BLOCK(sen_BLOCKts);
        }
        if (sym == "end") {
            tn->child.push_back(new treeNode("end"));
            sym = GETSYM();
        }
        else error_exc(complex_end_wrong);           //不是正确的复合语句结束
    } else return;
}
#endif //RUN_PL0_BLOCK_H
