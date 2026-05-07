#pragma comment(lib, "MSIMG32.LIB") 
#include <stdlib.h> 
#include "menu.h"

int main() {
    system("chcp 65001");//控制台编码改为 UTF-8

    MenuController app(1280, 760); // 初始化窗口大小为 1280x760
    app.run();
    return 0;
}