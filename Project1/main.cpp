#pragma comment(lib, "MSIMG32.LIB") 
#include "menu.h"

int main() {
    MenuController app(1280, 760); // 实例化主菜单 ，初始化窗口大小为 1280x760
    app.run();
    return 0;
}