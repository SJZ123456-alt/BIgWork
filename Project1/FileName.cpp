//#include <iostream>
//#include <cassert>  
//#include "AppState.h"
//
//// 单元测试函数
//void runUnitTests() {
//    std::cout << "===== 开始 Member A 单元测试 =====" << std::endl;
//
//    // --- 测试 1: Seqlist 基础功能 ---
//    Seqlist<int> seq;
//    seq.push_back(10);
//    seq.push_back(20);
//    assert(seq.get_size() == 2);
//    assert(seq[0] == 10);
//    assert(seq[1] == 20);
//
//    seq.remove(0); // 删除第0个
//    assert(seq.get_size() == 1);
//    assert(seq[0] == 20);
//    std::cout << "[Pass] Seqlist 基础测试通过" << std::endl;
//
//
//    // --- 测试 2: AABB 冲突检测算法 ---
//    CampusMap testMap("测试校区", 1000, 1000);
//
//    // 建筑 A: (0,0) 10x10
//    Building bA(0, 0, 10, 10, 1, "建筑A", "教学楼");
//    // 建筑 B: (5,5) 10x10 -> 与 A 重叠
//    Building bB(5, 5, 10, 10, 2, "建筑B", "食堂");
//    // 建筑 C: (20,20) 10x10 -> 与 A 不重叠
//    Building bC(20, 20, 10, 10, 3, "建筑C", "宿舍");
//
//    // 验证逻辑：!is_not_conflict 应该为 true 代表冲突
//    assert(testMap.is_not_conflict(bA, bB) == false); // 预期：重叠，返回 false
//    assert(testMap.is_not_conflict(bA, bC) == true);  // 预期：不重叠，返回 true
//    std::cout << "[Pass] AABB 冲突检测算法准确性测试通过" << std::endl;
//
//
//    // --- 测试 3: CampusMap 边界判定与自动 ID ---
//    // 尝试添加一个超出边界的建筑 (x=995, length=10 -> 总长1005 > 地图1000)
//    Building bOut(995, 0, 10, 10, 99, "越界建筑", "无效");
//    bool canAddOut = testMap.AddBuilding(bOut);
//    assert(canAddOut == false); // 预期：应该添加失败
//
//    // 添加第一个合法建筑
//    bool add1 = testMap.AddBuilding(bA);
//    assert(add1 == true);
//    // 验证 ID 是否由地图自动分配（如果你实现了自动分配）
//    // assert(testMap.getBuilding(0).id == 1); 
//
//    // 尝试在同一位置添加重叠建筑
//    bool addOverlap = testMap.AddBuilding(bB);
//    assert(addOverlap == false); // 预期：冲突，添加失败
//    std::cout << "[Pass] 地图边界与冲突拦截测试通过" << std::endl;
//
//
//    // --- 测试 4: AppState 与 LinkedList 联动 ---
//    AppState state;
//    state.addMap("主校区", 500, 500);
//    state.addMap("分校区", 300, 300);
//    assert(state.maps.size() == 2);
//
//    state.selectMap(0); // 选中“主校区”
//    assert(state.currentMap != nullptr);
//    assert(state.currentMap->operator==(state.maps[0]));
//    std::cout << "[Pass] AppState 多地图管理测试通过" << std::endl;
//
//    std::cout << "===== 所有单元测试 100% 通过！地基稳固 =====" << std::endl << std::endl;
//}
//
//int main() {
//    // 运行测试
//    try {
//        runUnitTests();
//    }
//    catch (const std::exception& e) {
//        std::cerr << "单元测试发现严重 Bug: " << e.what() << std::endl;
//        return -1;
//    }
//
//    // 接下来才是 Member B 的主菜单代码...
//    std::cout << "进入系统主菜单..." << std::endl;
//    // ...
//    return 0;
//}
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
// 
// 
// 
// 
// 
//测试已全部通过