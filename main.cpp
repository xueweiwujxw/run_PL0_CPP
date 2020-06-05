#include "interpret.h"

int main() {
    integerArray.clear();
    identifierArray.clear();
    streambuf *stdin_backup = cin.rdbuf();         //备份标准输入流
    const char *fileinname = "../demo.txt";
    fcin.open(fileinname);
    if (!fcin.is_open()) {
        cout << "failed to open " << fileinname << endl;
        return 0;
    }
    cin.rdbuf(fcin.rdbuf());                //将cin重定向到文件
    auto *BLOCKT = new treeNode("程序");
    while (fcin.tellg() != ios::end) {
        BLOCK(BLOCKT);
        if (sym == "finish" || sym == "error")
            break;
    }
    if (!error_grammar) {
        printTreeNode(BLOCKT, 0);
        printTables();
        printCodes();
    }
    cin.rdbuf(stdin_backup);
    fcin.close();
    Interpret();
    return 0;
}
