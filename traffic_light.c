#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define NUM_ROUTES 13
#define MAX_PHASES 10 // 最大相位上限，通常不会超过 10

// 路线名称，严格对应你给出的 13 条
const char* route_names[NUM_ROUTES] = {
    "BA", "DC", "ED", // 0, 1, 2
    "AB", "AC", "AD", // 3, 4, 5
    "BC", "BD",       // 6, 7
    "DA", "DB",       // 8, 9
    "EB", "EC",       // 10, 11
    "EA"              // 12
};

// 冲突矩阵
bool conflict[NUM_ROUTES][NUM_ROUTES] = {false};

void set_conflict(int i, int j) {
    conflict[i][j] = true;
    conflict[j][i] = true;
}

// 核心：根据交通规则初始化冲突矩阵
void init_conflicts() {
    // 规则1：同一进口道的路线互斥（从同一个路口出发）
    // A 出发: AB, AC, AD
    set_conflict(3, 4); set_conflict(3, 5); set_conflict(4, 5);
    // B 出发: BA, BC, BD
    set_conflict(0, 6); set_conflict(0, 7); set_conflict(6, 7);
    // D 出发: DA, DB, DC
    set_conflict(8, 9); set_conflict(8, 1); set_conflict(9, 1);
    // E 出发: EB, EC, EA
    set_conflict(10, 11); set_conflict(10, 12); set_conflict(11, 12);

    // 规则2：驶向同一出口的路线互斥（去往同一个路口）
    // 去 A: BA, DA, EA
    set_conflict(0, 8); set_conflict(0, 12); set_conflict(8, 12);
    // 去 B: AB, DB, EB
    set_conflict(3, 9); set_conflict(3, 10); set_conflict(9, 10);
    // 去 C: AC, BC, DC, EC (注意：没有从C出发的路线)
    set_conflict(4, 6); set_conflict(4, 1); set_conflict(4, 11);
    set_conflict(6, 1); set_conflict(6, 11); set_conflict(1, 11);
    // 去 D: AD, BD, ED
    set_conflict(5, 7); set_conflict(5, 2); set_conflict(7, 2);

    // 规则3：交叉冲突（左转让直行等，题干明确指出 E->B 和 A->D 冲突）
    set_conflict(10, 5); // EB vs AD
    
    // 补充常见的交叉冲突 (为了确保安全，保守设置)
    set_conflict(0, 3); // BA vs AB (对向直行)
    set_conflict(0, 6); // BA vs BC (左转与直行) - 其实规则1已包含
    set_conflict(8, 3); // DA vs AB (交叉)
    set_conflict(8, 6); // DA vs BC
    set_conflict(12, 5); // EA vs AD
    set_conflict(12, 7); // EA vs BD
    set_conflict(10, 4); // EB vs AC
    set_conflict(10, 11); // EB vs EC (规则1已含)
    set_conflict(11, 5); // EC vs AD
    set_conflict(11, 7); // EC vs BD
    
    // 确保对角线为 false
    for(int i = 0; i < NUM_ROUTES; i++) {
        conflict[i][i] = false;
    }
}

// 当前分组方案
int best_phases[NUM_ROUTES];
int best_phase_count = MAX_PHASES;

// 检查路线能否放入当前相位
bool can_place(int route, int phase, int current_phases[NUM_ROUTES]) {
    for (int i = 0; i < NUM_ROUTES; i++) {
        if (current_phases[i] == phase && conflict[route][i]) {
            return false;
        }
    }
    return true;
}

// 递归回溯搜索
void solve(int route_idx, int current_phases[NUM_ROUTES], int current_max_phase) {
    // 剪枝：如果当前使用的相位数已经大于等于目前找到的最优解，则放弃
    if (current_max_phase + 1 >= best_phase_count) return;

    // 所有路线都分配完毕
    if (route_idx == NUM_ROUTES) {
        best_phase_count = current_max_phase + 1;
        memcpy(best_phases, current_phases, sizeof(int) * NUM_ROUTES);
        return;
    }

    // 尝试将当前路线放入已有的相位中
    for (int p = 0; p <= current_max_phase; p++) {
        if (can_place(route_idx, p, current_phases)) {
            current_phases[route_idx] = p;
            solve(route_idx + 1, current_phases, current_max_phase);
            current_phases[route_idx] = -1; // 回溯
        }
    }

    // 尝试开辟一个新相位
    current_phases[route_idx] = current_max_phase + 1;
    solve(route_idx + 1, current_phases, current_max_phase + 1);
    current_phases[route_idx] = -1; // 回溯
}

int main() {
    init_conflicts();

    int current_phases[NUM_ROUTES];
    memset(current_phases, -1, sizeof(current_phases));

    printf("正在计算最优信号灯相位方案...\n\n");
    
    // 从第0条路线开始搜索
    solve(0, current_phases, -1);

    // 输出最优结果
    printf("===== 最优信号灯相位方案 =====\n");
    printf("最少需要 %d 个相位（红绿灯组）：\n\n", best_phase_count);

    for (int p = 0; p < best_phase_count; p++) {
        printf("【相位 %d】可同时通行：", p + 1);
        for (int i = 0; i < NUM_ROUTES; i++) {
            if (best_phases[i] == p) {
                printf("%s  ", route_names[i]);
            }
        }
        printf("\n");
    }

    // 最终合法性校验
    printf("\n===== 冲突验证 =====\n");
    bool valid = true;
    for (int i = 0; i < NUM_ROUTES; i++) {
        for (int j = i + 1; j < NUM_ROUTES; j++) {
            if (conflict[i][j] && best_phases[i] == best_phases[j]) {
                printf("错误：冲突路线 %s 和 %s 被分到了同一组！\n", route_names[i], route_names[j]);
                valid = false;
            }
        }
    }
    if (valid) {
        printf("验证通过：所有冲突路线均已被安全隔离，方案无冲突。\n");
    }

    return 0;
}
