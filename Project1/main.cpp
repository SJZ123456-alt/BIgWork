#pragma comment(lib, "MSIMG32.LIB") // 处理透明度必须链接的库
#include "menu.h"

int main() {
    // 实例化主菜单/系统控制器，初始化窗口大小为 1280x760
    MenuController app(1280, 760);

    // 启动系统主循环
    app.run();

    return 0;
}