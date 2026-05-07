#pragma once
#include <windows.h>
#include <algorithm>
#include "CampusMap.h"
#include "renderer.h"
#include "file_io.h"

// 交互控制器类
class MenuController {
private:
    AppState appState;
    Renderer renderer;
    int currentMapIndex;//初始化为-1
    bool isRunning;  
    bool showMapListOverlay; //判断有没有打开地图列表菜单

    double camX, camY, targetCamX, targetCamY, zoom, targetZoom;
    bool isDragging;  //右键拖动
    int dragLastX, dragLastY;
    int mouseX, mouseY;

    Building* hoveredBuilding;
    Seqlist<Button> buttons;

    int w2sX(double worldX) const { return (int)((worldX - camX) * zoom + renderer.WIN_W / 2.0); }
    int w2sY(double worldY) const { return (int)((worldY - camY) * zoom + renderer.WIN_H / 2.0); }
    double s2wX(int screenX) const { return (screenX - renderer.WIN_W / 2.0) / zoom + camX; }
    double s2wY(int screenY) const { return (screenY - renderer.WIN_H / 2.0) / zoom + camY; }

    void showMsg(const wstring& text) const { MessageBoxW(GetHWnd(), text.c_str(), L"系统提示", MB_OK); }

    bool inputStr(const wchar_t* prompt, string& result, const wchar_t* def = L"") const {
        wchar_t buf[256]; wcsncpy_s(buf, def, _TRUNCATE);
        if (InputBox(buf, 256, prompt, L"系统输入", def, 0, 0, false)) {
            result = ws2s(buf);
            return true;
        }
        return false;
    }

    bool inputInt(const wchar_t* prompt, int& value, int def = 0) const {
        string str;
        if (inputStr(prompt, str, to_wstring(def).c_str())) {
            try { value = stoi(str); return true; }
            catch (...) { showMsg(L"请输入有效的整数！"); }
        }
        return false;
    }

    void handleBtnClick(int id) {
        switch (id) {
        case 0: { // 1. 创建地图
            string name; int l, w;
            if (!inputStr(L"请输入校园名称：", name)) return;
            if (name.empty()) { showMsg(L"校园不能没有名字，就像西方不能失去耶路撒冷( ˘•ω•˘ )"); return; } // 防空
            if (!inputInt(L"地图实际长度 L：", l) || !inputInt(L"地图实际宽度 W：", w)) return;
            if (l <= 0 || w <= 0) {
                showMsg(L"请给校园一点生存空间吧~(￣▽￣)");
                return;
            }

            CampusMap map(name, l, w);
            map.setID(appState.maps.size() + 1);
            appState.maps.add(map);
            currentMapIndex = appState.maps.size() - 1;//创建完地图后，画面立刻切换到这个新地图上
            targetCamX = l / 2.0; targetCamY = w / 2.0;
            targetZoom = min((renderer.WIN_W - 200.0) / l, (renderer.WIN_H - 200.0) / w);//留点边距
            showMsg(L"创建成功！");
            break;
        }
        case 1: { // 切换地图
            if (appState.maps.size() == 0) showMsg(L"先创建一个地图再点我试试吧(◕ᴗ◕✿)");
            else showMapListOverlay = true;
            break;
        }
        case 2: { // 2. 增加建筑
            if (currentMapIndex < 0) { showMsg(L"先创建一个地图再点我试试吧(◕ᴗ◕✿)"); return; }
            CampusMap& map = appState.maps[currentMapIndex];
            Building b;
            if (!inputStr(L"建筑名称：", b.name)) return;
            if (b.name.empty()) { showMsg(L"建筑名称不能为空呦๐•ᴗ•๐"); return; } // 防空

            if (!inputInt(L"类型(1教学 2食堂 3图书 4体育 5湖泊 6宿舍 或其他数字)：", b.type)) return;
            int tx, ty, tl, tw;
            if (!inputInt(L"左上角坐标 X：", tx)) return; b.x = tx;
            if (!inputInt(L"左上角坐标 Y：", ty)) return; b.y = ty;
            if (!inputInt(L"长度 L：", tl)) return; b.length = tl;
            if (!inputInt(L"宽度 W：", tw)) return; b.width = tw;
            if (tl <= 0 || tw <= 0) {
                    showMsg(L"哎呀，这个建筑小到连细菌都住不进去啦...(｡•́︿•̀｡)");
                    return;
                }

            if (!inputStr(L"功能描述：", b.description)) return;
            if (b.description.empty()) b.description = "这个楼很懒，什么也没留下(￣o￣) . z Z";

            if (!map.AddBuilding(b)) showMsg(L"添加失败！(T▽T)  可能原因：\n1. 建筑名称已存在\n2. 建筑重叠冲突\n3. 超出地图边界");
            else showMsg(L"添加成功！(≧▽≦)");
            break;
        }
        case 3: { // 4. 删除建筑
            if (currentMapIndex < 0) { showMsg(L"先创建一个地图再点我试试吧(◕ᴗ◕✿)"); return; }
            int tid;
            if (inputInt(L"请输入要删除的【建筑编号(ID)】：", tid)) {
                if (appState.maps[currentMapIndex].removeBuildingById(tid)) showMsg(L"删除成功！");
                else showMsg(L"未找到该编号！");
            }
            break;
        }
        case 4: { // 5. 修改建筑
            if (currentMapIndex < 0) { showMsg(L"先创建一个地图再点我试试吧(◕ᴗ◕✿)"); return; }
            int tid;
            if (inputInt(L"请输入建筑编号(ID)以修改：", tid)) {
                CampusMap& map = appState.maps[currentMapIndex];
                Building* target = map.getBuildingById(tid);
                if (!target) { showMsg(L"未找到该编号！"); return; }

                Building b = *target;
                wstring defName = s2ws(b.name);                 // 原名称转宽字符
                inputStr(L"新建筑名称：", b.name, defName.c_str());
                if (b.name.empty()) { showMsg(L"建筑名称不能为空呦๐•ᴗ•๐"); return; } // 防空

                if (!inputInt(L"新类型(1教学 2食堂 3图书 4体育 5湖泊 6宿舍 或其他数字)：", b.type, b.type)) return;

                int tx, ty, tl, tw;
                if (inputInt(L"新坐标 X：", tx, b.x)) b.x = tx;
                if (inputInt(L"新坐标 Y：", ty, b.y)) b.y = ty;
                if (inputInt(L"新长度 L：", tl, b.length)) b.length = tl;
                if (inputInt(L"新宽度 W：", tw, b.width)) b.width = tw;
                
                if (b.length <= 0 || b.width <= 0) {
                    showMsg(L"怎么把这栋楼给降维了？长宽必须大于 0 orz");
                    return;
                }

                inputStr(L"新描述：", b.description, s2ws(b.description).c_str());
                if (b.description.empty()) b.description = "这个楼很懒，什么也没留下(￣o￣) . z Z";

                if (map.updateBuilding(tid, b)) showMsg(L"修改成功！ヾ(*´∀ ˋ*)ﾉ");
                else showMsg(L"修改失败！(╥﹏╥)  可能原因：\n1. 新名称已存在\n2. 建筑重叠冲突\n3. 超出地图边界");
            }
            break;
        }
        case 5: { // 保存
            string path;
            if (inputStr(L"请输入保存路径 (例如 D:\\campus_data.txt)：", path, L"D:\\campus_data.txt")) {
                if (FileIO::saveToFile(path, appState)) showMsg(L"所有数据保存成功！");
                else showMsg(L"保存失败！请检查路径或数据是否为空。");
            }
            break;
        }
        case 6: { // 导入
            string path;
            if (inputStr(L"请输入导入路径 (例如 D:\\campus_data.txt)：", path, L"D:\\campus_data.txt")) {
                if (FileIO::loadFromFile(path, appState)) {
                    currentMapIndex = appState.maps.size() > 0 ? 0 : -1; //如果有地图的话显示第一个
                    if (currentMapIndex >= 0) {
                        targetCamX = appState.maps[0].getLength() / 2.0; targetCamY = appState.maps[0].getWidth() / 2.0;
                        targetZoom = min((renderer.WIN_W - 200.0) / appState.maps[0].getLength(), (renderer.WIN_H - 200.0) / appState.maps[0].getWidth());
                    }
                    showMsg(L"数据导入成功！");
                }
                else showMsg(L"导入失败，请检查文件是否存在或格式是否正确！");
            }
            break;
        }
        case 7: isRunning = false; break;
        }
    }

    void updateLogic() {
        camX += (targetCamX - camX) * 0.15;
        camY += (targetCamY - camY) * 0.15;
        zoom += (targetZoom - zoom) * 0.15;

        hoveredBuilding = nullptr;
        bool uiHovered = false;

        for (int i = 0; i < buttons.get_size(); ++i) {
            Button& b = buttons[i];
            bool isHover = (!showMapListOverlay && mouseX >= b.x && mouseX <= b.x + b.w && mouseY >= b.y && mouseY <= b.y + b.h);
            b.hoverProgress += (isHover ? 0.15 : -0.15);
            b.hoverProgress = max(0.0, min(1.0, b.hoverProgress));
            //b.clickScale += (1.0 - b.clickScale) * 0.2;    //收敛于1  //由于动画显示有问题故放弃按钮缩放
            if (isHover) uiHovered = true;  //判断鼠标指着按钮
        }

        if (!uiHovered && !isDragging && !showMapListOverlay && currentMapIndex >= 0) {
            double wMouseX = s2wX(mouseX), wMouseY = s2wY(mouseY);
            hoveredBuilding = appState.maps[currentMapIndex].getBuildingAt(wMouseX, wMouseY);//建筑高亮
        }
    }

    void renderLoop() {
        BeginBatchDraw();
        cleardevice();
        setfillcolor(RGB(242, 246, 250));
        solidrectangle(0, 0, renderer.WIN_W, renderer.WIN_H);

        if (currentMapIndex >= 0 && currentMapIndex < appState.maps.size()) {
            CampusMap& map = appState.maps[currentMapIndex];
            int msX = w2sX(0), msY = w2sY(0), meX = w2sX(map.getLength()), meY = w2sY(map.getWidth());
            renderer.drawSmoothRoundedRect(msX, msY, meX - msX, meY - msY, 15, RGB(255, 255, 255), RGB(200, 200, 210), 3);

            settextcolor(RGB(150, 150, 160));
            settextstyle(20, 0, L"微软雅黑", 0, 0, 700, false, false, false);
            outtextxy(msX + 15, msY - 30, s2ws(map.getName()).c_str());

            for (int i = 0; i < map.getBuildingCount(); ++i) {
                const Building& p = map.getBuilding(i);
                int x1 = w2sX(p.x), y1 = w2sY(p.y);
                int bw = (int)(p.length * zoom), bh = (int)(p.width * zoom);
                bool isHovered = (&p == hoveredBuilding);

                COLORREF baseColor = renderer.getThemeColor(p.type);
                COLORREF drawColor = isHovered ? renderer.blendColor(WHITE, baseColor, 0.2) : baseColor;

                int expand = isHovered ? 4 : 0;
                renderer.drawSmoothRoundedRect(x1 - expand, y1 - expand, bw + expand * 2, bh + expand * 2, 10, drawColor, renderer.blendColor(BLACK, drawColor, 0.2), isHovered ? 3 : 2);

                if (bw > 25 && bh > 15) {  //够大才能有字
                    wstring displayName = L"[" + to_wstring(p.id) + L"] " + s2ws(p.name);
                    int fontSize = max(10, min(30, (int)(bh / 2.5)));
                    settextstyle(fontSize, 0, L"微软雅黑", 0, 0, isHovered ? 700 : 400, false, false, false);
                    settextcolor(RGB(40, 40, 40));
                    outtextxy(x1 + (bw - textwidth(displayName.c_str())) / 2, y1 + (bh - textheight(displayName.c_str())) / 2, displayName.c_str());
                }
            }
        }
        else {
            settextcolor(RGB(180, 180, 190));
            settextstyle(30, 0, L"微软雅黑", 0, 0, 700, false, false, false);
            outtextxy(renderer.WIN_W / 2 - 250, renderer.WIN_H / 2 - 50, L"点一下【创建地图】来开荒吧！(〃'▽'〃)");
        }

        if (hoveredBuilding && !showMapListOverlay) renderer.drawHoverCard(hoveredBuilding, mouseX, mouseY);  //显示详细信息

        for (int i = 0; i < buttons.get_size(); ++i) {  //绘制按钮
            Button& b = buttons[i];

            //int currW = (int)(b.w * b.clickScale), currH = (int)(b.h * b.clickScale);
            //int currX = b.x + (b.w - currW) / 2, currY = b.y + (b.h - currH) / 2;
            int currW = b.w, currH = b.h;
            int currX = b.x, currY = b.y;

            COLORREF bgColor = renderer.blendColor(RGB(235, 240, 255), RGB(255, 255, 255), b.hoverProgress); //前面updateLogic算出的hoverProgress
            COLORREF txtColor = (i == 7) ? RGB(220, 50, 50) : renderer.blendColor(RGB(60, 100, 250), RGB(80, 80, 90), b.hoverProgress);

            renderer.drawSmoothRoundedRect(currX, currY, currW, currH, 15, bgColor, RGB(210, 215, 225), 2);
            settextstyle((int)16, 0, L"微软雅黑", 0, 0, 700, false, false, false);
            settextcolor(txtColor);
            outtextxy(currX + (currW - textwidth(b.text.c_str())) / 2, currY + (currH - textheight(b.text.c_str())) / 2, b.text.c_str());
        }

        if (showMapListOverlay) renderer.drawMapListOverlay(appState, mouseX, mouseY);
        EndBatchDraw(); //一次性展示在屏幕上
    }

public:
    MenuController(int w, int h) : renderer(w, h), currentMapIndex(-1), isRunning(true), showMapListOverlay(false),
        camX(0), camY(0), targetCamX(0), targetCamY(0), zoom(1.0), targetZoom(1.0),
        isDragging(false), mouseX(0), mouseY(0), hoveredBuilding(nullptr) {

        const wchar_t* labels[] = { L"创建地图", L"切换地图", L"添加", L"删除", L"修改", L"保存", L"导入", L"退出" };
        int btnCount = 8, btnW = 110, btnH = 46, spacing = 12;

        int totalW = btnCount * btnW + (btnCount - 1) * spacing;
        int startX = (w - totalW) / 2;
        //使按钮们居中
        for (int i = 0; i < btnCount; ++i) {
            Button b = { i, startX + i * (btnW + spacing), h - 70, btnW, btnH, labels[i], 0.0, 1.0 };
            buttons.push_back(b);
        }
    }

    void run() {
        initgraph(renderer.WIN_W, renderer.WIN_H);
        setbkmode(TRANSPARENT);  //文字背景透明

        ExMessage msg;
        while (isRunning) {
            DWORD startTime = GetTickCount();

            while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
                mouseX = msg.x; mouseY = msg.y;

                if (showMapListOverlay && msg.message == WM_LBUTTONDOWN) {  //左键按下
                    int boxW = 500, boxH = 460;
                    int boxX = (renderer.WIN_W - boxW) / 2, boxY = (renderer.WIN_H - boxH) / 2 - 30;
                    if (msg.x > boxX + boxW - 45 && msg.x < boxX + boxW - 15 && msg.y > boxY + 15 && msg.y < boxY + 45) {
                        showMapListOverlay = false; continue;
                    }//地图列表的叉号
                    for (int i = 0; i < appState.maps.size(); ++i) {
                        int itemY = boxY + 70 + i * 55;
                        if (msg.x > boxX + 20 && msg.x < boxX + boxW - 20 && msg.y > itemY && msg.y < itemY + 45) {
                            currentMapIndex = i;
                            targetCamX = appState.maps[i].getLength() / 2.0; targetCamY = appState.maps[i].getWidth() / 2.0;
                            targetZoom = min((renderer.WIN_W - 200.0) / appState.maps[i].getLength(), (renderer.WIN_H - 200.0) / appState.maps[i].getWidth());
                            showMapListOverlay = false; break;
                        }
                    }
                    continue;
                }

                if (!showMapListOverlay) {
                    if (msg.message == WM_MOUSEWHEEL) {
                        double wX = s2wX(msg.x), wY = s2wY(msg.y);
                        if (msg.wheel > 0) targetZoom *= 1.25; else targetZoom /= 1.25;
                        targetZoom = max(0.1, min(10.0, targetZoom));
                        targetCamX = wX - (msg.x - renderer.WIN_W / 2.0) / targetZoom;
                        targetCamY = wY - (msg.y - renderer.WIN_H / 2.0) / targetZoom;
                    }
                    else if (msg.message == WM_RBUTTONDOWN || msg.message == WM_MBUTTONDOWN) {
                        isDragging = true; dragLastX = msg.x; dragLastY = msg.y;
                    }
                    else if (msg.message == WM_RBUTTONUP || msg.message == WM_MBUTTONUP) {
                        isDragging = false;
                    }
                    else if (msg.message == WM_MOUSEMOVE && isDragging) {
                        double dx = (msg.x - dragLastX) / zoom, dy = (msg.y - dragLastY) / zoom;
                        targetCamX -= dx; camX -= dx; targetCamY -= dy; camY -= dy;
                        dragLastX = msg.x; dragLastY = msg.y;
                    }
                    else if (msg.message == WM_LBUTTONDOWN) {
                        for (int i = 0; i < buttons.get_size(); ++i) {
                            Button& b = buttons[i];
                            if (msg.x >= b.x && msg.x <= b.x + b.w && msg.y >= b.y && msg.y <= b.y + b.h) {
                                b.clickScale = 0.85; handleBtnClick(b.id);
                            }
                        }
                    }
                }
            }

            updateLogic();
            renderLoop();

            DWORD elapsed = GetTickCount() - startTime;
            if (elapsed < 16) Sleep(16 - elapsed);
        }
        closegraph();
    }
};