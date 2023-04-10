#ifndef _DATA__H
#define _DATA__H

#include <vector>
#include <cmath>
#include <unordered_map>
#include <set>
#include <stack>
#include "Bench.h"
#include "robot.h"

#define RPI (double)3.14159

#define MAPWIDTH (int)100 // 地图宽pixel
#define MAPHEIGHT (int)100 // 地图长pixel
#define MAX_BENCH_SIZE (int)50 // 最大工作台数量
#define ROBOT_NUM (int)4 // 机器人数量
#define ROBOT_MAX_FORWARDSPEED (double)6 // 最大前进6m/s
#define ROBOT_MAX_RETREATSPEED (double)2. // 最大后退2m/s
#define SYS_MAX_FRAMENUM (int)9000 // 最大帧数
#define SYS_SECOND_FRAMENUM (int)8500 // 最大帧数
#define SYS_THIRD_FRAMENUM (int)8000 // 最大帧数
#define ROBOT_MAX_ROTATESPEED RPI // 最大角速度
#define ROBOT_RADUIS_WITHOUT (double)0.45 // 机器人空载半径
#define ROBOT_RADUIS_WITH (double)0.53 // 机器人载物半径

RPoint Pos2Actual(RPoint); // 坐标到实际尺寸的转换
#define Pos2Actual(p) RPoint({0.5 * (p.x + 0.5), 0.5 * (99.5 - p.y)}) 

double DistRPoint2RPoint(RPoint, RPoint); // 计算点到点距离
#define DistRPoint2RPoint(p1, p2) (double)sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)) 

double Point2Rad2(RPoint, RPoint); // 两点相对0rad为旋转角
#define Point2Rad2(p1, p2) atan2(p2.y - p1.y, p2.x - p1.x)

extern unordered_map<int, vector<int>> mBench2SubBench; // 当前工作台到下一级工作台的映射关系
extern unordered_map<int, int> mMaxProfit; // 货物对应最大利润
extern unordered_map<int, vector<int>> mRawMaterial; // 工作台类型对应原材料
extern unordered_map<int, vector<int>> mBenchSort; // 每种工作台的所有编号

extern int currentFrameId; // 当前绝对帧数（相对于起始0而言）
extern int actualBenchNum; // 有效工作台数量
extern vector<Bench> vBenchInfo; // 所有工作台
extern vector<Robot> vRobotInfo; // 所有机器人信息

// task pool
extern unordered_map<int, int> mBuyBenchGoods; // 货物对应最大利润
extern unordered_map<int, int> mSellBenchGoods; // 货物对应最大利润
//// 7缺失456生产所需原材料；0：不缺456都在加工，1
extern set<int> sSevenLackRaw; // 7类型工作台缺的原材料类型
extern unordered_map<int, set<int>> mSevenRawState; // 7类型工作台有的原材料类型
extern vector<int> needResetNum; // 到达目标后需要最后复位的机器人

// 调试文件路径
extern double param1;
extern double param2;
extern int profitMoney;
extern string savePath;
extern string mapNum;

void InitMapInfo(const vector<string>& vs);
void UpDataInfo(const vector<string>& vs);
void SysInit(); // 系统初始化，读取地图信息
void PrintRobotInfo();
void PrintBenchInfo();
void PrintTaskPoolInfo();

#define SYS_DEBUG 1

#endif

