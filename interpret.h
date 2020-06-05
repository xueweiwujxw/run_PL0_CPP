//
// Created by xww on 2020/5/25.
//

#ifndef RUN_PL0_INTERPRET_H
#define RUN_PL0_INTERPRET_H
#include "block.h"
#include <stack>
//codeEle currentCode;    //指令寄存器
int pcReg;              //下一条目标指令地址
int spReg;              //栈顶寄存器
int bpReg;              //基地址寄存器
vector<int> runningStack;//运行栈

//名字表中var的value没有设置，在解释执行的时候赋值

void Interpret()
{
    cout << "开始执行" << endl;
    bool code_end = false;
    int cIndex = 0;     //代码序列索引
    //初始化运行栈，并添加静态链、动态链、返回地址
    runningStack.clear();
    runningStack.push_back(0);   //静态链入栈
    runningStack.push_back(0);   //动态链入栈
    runningStack.push_back(0);   //返回地址入栈
    spReg = -1;
    pcReg = 0;
    bpReg = 0;
    while (cIndex < codes.size() && !code_end) {
        pcReg++;
        if (codes[cIndex].func == "lit") {
            spReg++;
            runningStack.push_back(codes[cIndex].offset);
        }
        else if (codes[cIndex].func == "lod") {
            spReg++;
            int tmpBpReg = bpReg;
            for (int i = 0; i < codes[cIndex].level_d; ++i)
                tmpBpReg = runningStack[tmpBpReg];        //根据当前过程的静态链查找层差大于0的基地址
            runningStack.push_back(runningStack[tmpBpReg + codes[cIndex].offset]);
        }
        else if (codes[cIndex].func == "sto") {
            int tmpBpReg = bpReg;
            for (int i = 0; i < codes[cIndex].level_d; ++i)
                tmpBpReg = runningStack[tmpBpReg];        //根据当前过程的静态链查找层差大于0的基地址
            runningStack[tmpBpReg + codes[cIndex].offset] = runningStack.back();
            runningStack.pop_back();
            spReg--;
        }
        else if (codes[cIndex].func == "cal") {
            int tmpBpReg = bpReg;
            for (int i = 0; i < codes[cIndex].level_d; ++i)
                tmpBpReg = runningStack[tmpBpReg];        //根据当前过程的静态链查找到真正的静态外层
            runningStack.push_back(tmpBpReg);      //静态链
            runningStack.push_back(bpReg);  //动态链
            runningStack.push_back(pcReg);  //返回地址
            bpReg = spReg + 1;              //修改基地址寄存器
            pcReg = codes[cIndex].offset;   //修改pc值
        }
        else if (codes[cIndex].func == "int") {
            int tmpSpace = codes[cIndex].offset - 3;
            runningStack.resize(runningStack.size()+tmpSpace);
            spReg += codes[cIndex].offset;
        }
        else if (codes[cIndex].func == "jmp") {
            pcReg = codes[cIndex].offset;
        }
        else if (codes[cIndex].func == "jpc") {
            if (runningStack[spReg] == 0) {
                pcReg = codes[cIndex].offset;
            }
            spReg--;
            runningStack.pop_back();
        }
        else if (codes[cIndex].func == "opr") {
            switch (codes[cIndex].offset) {
                case ReturnOpr: {
                    if (pcReg == codes.size()) {
                        code_end = true;
                        cout << "程序结束" << endl;
                    }
                    int count = (int)runningStack.size() - bpReg;
                    spReg = bpReg - 1;
                    bpReg = runningStack[spReg+2];      //修改基地址寄存器
                    pcReg = runningStack[spReg+3];      //修改pc寄存器
                    //出栈
                    runningStack.resize(runningStack.size()-count);
                    break;
                }
                case MinusOpr: {
                    runningStack[spReg] = -runningStack[spReg];
                    break;
                }
                case AddOpr: {
                    spReg--;
                    runningStack[spReg] += runningStack[spReg+1];
                    runningStack.pop_back();
                    break;
                }
                case SubOpr: {
                    spReg--;
                    runningStack[spReg] -= runningStack[spReg+1];
                    runningStack.pop_back();
                    break;
                }
                case MulOpr: {
                    spReg--;
                    runningStack[spReg] *= runningStack[spReg+1];
                    runningStack.pop_back();
                    break;
                }
                case DivOpr: {
                    spReg--;
                    runningStack[spReg] /= runningStack[spReg+1];
                    runningStack.pop_back();
                    break;
                }
                case OddOpr: {
                    runningStack[spReg] %= 2;
                    break;
                }
                case ModOpr: {
                    spReg--;
                    runningStack[spReg] %= runningStack[spReg+1];
                    runningStack.pop_back();
                    break;
                }
                case EquOpr: {
                    spReg--;
                    runningStack[spReg] = (runningStack[spReg] == runningStack[spReg+1]);
                    runningStack.pop_back();
                    break;
                }
                case NeqOpr: {
                    spReg--;
                    runningStack[spReg] = (runningStack[spReg] != runningStack[spReg+1]);
                    runningStack.pop_back();
                    break;
                }
                case LessOpr: {
                    spReg--;
                    runningStack[spReg] = (runningStack[spReg] < runningStack[spReg+1]);
                    runningStack.pop_back();
                    break;
                }
                case EgreOpr: {
                    spReg--;
                    runningStack[spReg] = (runningStack[spReg] >= runningStack[spReg+1]);
                    runningStack.pop_back();
                    break;
                }
                case GreOpr: {
                    spReg--;
                    runningStack[spReg] = (runningStack[spReg] > runningStack[spReg+1]);
                    runningStack.pop_back();
                    break;
                }
                case ElessOpr: {
                    spReg--;
                    runningStack[spReg] = (runningStack[spReg] <= runningStack[spReg+1]);
                    runningStack.pop_back();
                    break;
                }
                case WriteOpr: {
                    cout << "向屏幕输出：" << runningStack.back();
                    spReg--;
                    runningStack.pop_back();
                    break;
                }
                case ChaLineOpr: {
                    cout << endl;
                    break;
                }
                case ReadOpr: {
                    spReg++;
                    int cinTmp;
                    cout << "从键盘读入：";
                    cin >> cinTmp;
                    runningStack.push_back(cinTmp);
                    //cout << "输入完成" << endl;
                    break;
                }
                default: {
                    break;
                }
            }
        }
        cIndex = pcReg;
    }
}

#endif //RUN_PL0_INTERPRET_H
