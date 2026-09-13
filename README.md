# 多岔路口交通管理作业

## 1. 作业简介
这是《多岔路口交通管理》课程的课后作业。通过算法计算，为包含13条通行路线的五岔路口设计合理的红绿灯放行方案。

## 2. 设计思路
* 将 13 条路线抽象为图的 13 个顶点。
* 将互相冲突的路线（例如 E→B 和 A→D）之间连边。
* 使用回溯法（Backtracking）求图的最小着色数（Chromatic Number），即最少的信号灯相位数。
* 输出最终的交通灯控制方案。
* 
## 3. 运行方法
本代码基于 C 语言编写。需要具备 C 语言编译环境（如 gcc、Dev-C++、Visual Studio 等）。

## 4. 编译命令
traffic_light.c
## 5. 运行结果
<img width="413" height="281" alt="image" src="https://github.com/user-attachments/assets/18f64b2a-2513-4406-94c4-c032b918393e" />



