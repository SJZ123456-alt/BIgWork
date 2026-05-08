#pragma once
#include <graphics.h>
#include <string>
#include "CampusMap.h"

using namespace std;

// --- 字符串转换工具 --- //
inline wstring s2ws(const string& str) {
    if (str.empty()) return wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

inline string ws2s(const wstring& wstr) {
    if (wstr.empty()) return string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

struct Button {
    int id;
    int x, y, w, h;
    wstring text;
    double hoverProgress;
    double clickScale;
};

// 渲染器类（仅负责绘制）
class Renderer {
public:
    int WIN_W, WIN_H;

    Renderer(int w, int h) : WIN_W(w), WIN_H(h) {}

    COLORREF blendColor(COLORREF c1, COLORREF c2, double ratio) const {
        int r = (int)(GetRValue(c1) * ratio + GetRValue(c2) * (1 - ratio));
        int g = (int)(GetGValue(c1) * ratio + GetGValue(c2) * (1 - ratio));
        int b = (int)(GetBValue(c1) * ratio + GetBValue(c2) * (1 - ratio));
        return RGB(r, g, b);
    }

    COLORREF getThemeColor(int type) const {
        switch (type) {
        case 1: return RGB(105, 161, 255);
        case 2: return RGB(255, 145, 99);
        case 3: return RGB(255, 203, 71);
        case 4: return RGB(83, 212, 142);
        case 5: return RGB(92, 219, 237);
        case 6: return RGB(156, 136, 255);
        default: return RGB(180, 180, 190);
        }
    }

    void drawSmoothRoundedRect(int x, int y, int w, int h, int r, COLORREF fill, COLORREF border, int thickness = 2) const {
        setfillcolor(fill);
        setlinecolor(border);
        setlinestyle(PS_SOLID | PS_JOIN_ROUND, thickness);
        solidroundrect(x, y, x + w, y + h, r, r);//实心
        roundrect(x, y, x + w, y + h, r, r);//框架
    }

    // 绘制地图列表弹窗
    void drawMapListOverlay(const AppState& appState, int mouseX, int mouseY) const {
        int boxW = 500, boxH = 460;
        int boxX = (WIN_W - boxW) / 2, boxY = (WIN_H - boxH) / 2 - 30;

        drawSmoothRoundedRect(boxX, boxY, boxW, boxH, 15, RGB(255, 255, 255), RGB(150, 150, 160), 3);

        //画叉号
        int closeX = boxX + boxW - 30;
        int closeY = boxY + 30;
        int size = 8; // 大小
        setlinecolor(RGB(200, 200, 210));
        setlinestyle(PS_SOLID, 2);
        line(closeX - size, closeY - size, closeX + size, closeY + size);
        line(closeX + size, closeY - size, closeX - size, closeY + size);

        settextcolor(RGB(50, 50, 60));
        settextstyle(24, 0, L"微软雅黑", 0, 0, 700, false, false, false);
        outtextxy(boxX + 25, boxY + 20, L"请选择要切换的地图：");

        for (int i = 0; i < appState.maps.size(); ++i) {
            int itemY = boxY + 70 + i * 55;
            if (itemY > boxY + boxH - 60) break;

            bool hover = (mouseX > boxX + 20 && mouseX < boxX + boxW - 20 && mouseY > itemY && mouseY < itemY + 45);
            drawSmoothRoundedRect(boxX + 20, itemY, boxW - 40, 45, 8, hover ? RGB(230, 242, 255) : RGB(245, 248, 250), hover ? RGB(100, 150, 255) : RGB(220, 225, 230), 1);

            wstring text = L"地图 ID: " + to_wstring(appState.maps[i].getID()) + L"   |   " + s2ws(appState.maps[i].getName()) + L" (" + to_wstring((int)appState.maps[i].getLength()) + L"x" + to_wstring((int)appState.maps[i].getWidth()) + L")";
            //wstring text = L"地图 ID: " + to_wstring(appState.maps[i].getID()) + L"   |   " + s2ws(appState.maps[i].getName());
            settextstyle(18, 0, L"微软雅黑", 0, 0, hover ? 700 : 400, false, false, false);
            settextcolor(hover ? RGB(30, 80, 200) : RGB(30, 30, 30));
            outtextxy(boxX + 40, itemY + 12, text.c_str());
        }
    }

    // 绘制悬浮信息框
    void drawHoverCard(const Building* b, int mouseX, int mouseY) const {
        if (!b) return;
        int cardW = 220, cardH = 140;
        int cx = mouseX + 15, cy = mouseY + 15;
        if (cx + cardW > WIN_W) cx = mouseX - cardW - 15;
        if (cy + cardH > WIN_H) cy = mouseY - cardH - 15;

        drawSmoothRoundedRect(cx, cy, cardW, cardH, 15, RGB(255, 255, 255), RGB(200, 200, 200), 2);
        settextstyle(18, 0, L"微软雅黑", 0, 0, 700, false, false, false);
        settextcolor(getThemeColor(b->type));
        wstring title = L"[" + to_wstring(b->id) + L"] " + s2ws(b->name);
        outtextxy(cx + 15, cy + 15, title.c_str());

        settextstyle(14, 0, L"微软雅黑");
        settextcolor(RGB(100, 100, 100));
        wchar_t buf[256];
        swprintf(buf, 256, L"坐标: (%.1f, %.1f)", b->x, b->y);
        outtextxy(cx + 15, cy + 45, buf);
        swprintf(buf, 256, L"占地: %.1f × %.1f", b->length, b->width);
        outtextxy(cx + 15, cy + 65, buf);

        /*wstring desc = s2ws(b->description);
        if (desc.length() > 12) desc = desc.substr(0, 12) + L"...";
        wstring desc_1 = L"描述: " + desc;
        outtextxy(cx + 15, cy + 85, desc_1.c_str());*///这里直接显示前12个字，会截断

        outtextxy(cx + 15, cy + 85, L"描述: ");
        // 定义一个文字可以显示的矩形区域
        RECT r = { cx + 55, cy + 85, cx + cardW - 15, cy + cardH - 10 };
        wstring desc = s2ws(b->description);
        // DT_WORDBREAK 表示遇到边界自动换行
        drawtext(desc.c_str(), &r, DT_LEFT | DT_WORDBREAK);
    }
};