#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <unordered_map>
#include <stack>
using namespace std;

#include "data.h"
#include "Bench.h"
#include "robot.h"
#include "mAlgorithm.h"

#define INCOME_COFF (double)0.
#define DISTANCE_COFF (double)1.
#define DISTANCE_2_FRAME (double)(8.33333 * 1.1) // 机器人全速时距离和帧数的转换关系

//#define DISTANCE_2_FRAME (double)8.5
struct Task {
	int _qBench; // 当前任务工作台编号
	int _qCmd; // 当前任务：0：买；1：卖
};
enum TaskLevel {
	LEVEL1 = 0, // 1：最低什么都没有
	LEVEL2_LOWEST = 1, // 123卖给9
	LEVEL2 = 3, // 2：14 24什么都没有
	LEVEL3 = 7, // 3：456缺一个原材料
	LEVEL3_HEIGTH = 8, // 3：456缺一个原材料
	LEVEL3_LOWEST = 9, // 456卖给9
	LEVEL4 = 11, // 7缺一个原材料将14、15、26优先级提高
	LEVEL5 = 15, // 5：7缺三个原材料（执行了这个任务还差两个原材料）
	LEVEL6_1 = 18, // 6：7缺两个原材料，有一个Going状态原材料
	LEVEL6 = 19, // 6：7缺两个原材料（执行了这个任务还差一个原材料）
	LEVEL7_2 = 21, // 7：7缺一个原材料，有两个Going状态原材料
	LEVEL7_1 = 22, // 7：7缺一个原材料，有一个Going状态原材料
	LEVEL7 = 23, // 7：7缺一个原材料
	LEVEL8 = 888, // 8：针对性优化使用
	LEVEL9 = 999, // 9：7生产好了，并且可以送456

	LEVEL_SECOND_1 = 1, // 买123卖9
	LEVEL_SECOND_2 = 2, // 买123卖456
	LEVEL_SECOND_3 = 3, // 买456卖79
	LEVEL_SECOND_4 = 4, // 买7卖8

	LEVEL_THIRD_0 = 0, // 任务太长>=2
	LEVEL_THIRD_1 = 1, // 买123卖
	LEVEL_THIRD_2 = 2, // 买456卖
	LEVEL_THIRD_3 = 3, // 买7卖
	//LEVEL1 = 0, // 1：最低什么都没有
	//LEVEL2_LOWEST = 1, // 123卖给9
	//LEVEL2 = 3, // 2：14 24什么都没有
	//LEVEL3 = 7, // 3：456缺一个原材料
	//LEVEL3_LOWEST = 8, // 456卖给9
	////LEVEL4 = 11, // 4：7生产好了456不可以送
	//LEVEL5 = 15, // 5：7缺三个原材料（执行了这个任务还差两个原材料）
	//LEVEL6 = 19, // 6：7缺两个原材料（执行了这个任务还差一个原材料）
	//LEVEL7 = 23, // 7：7缺一个原材料
	//LEVEL8 = 24, // 8：7缺一个原材料将14、15、26优先级提高
	//LEVEL9 = 27, // 9：7生产好了，并且可以送456
};

class TaskStack {
public:
	int _level = TaskLevel::LEVEL1; // 当前任务的层级7；456次之
	int _income = 0; // 当前任务收益
	double _distance = 0.; // 当前任务走过的距离
	vector<Task> _qTaskList; // 当前任务列表，按照从高到底排序
};

class TaskPool {
public:
	bool operator()(TaskStack tq1, TaskStack tq2);
};

extern priority_queue<TaskStack, vector<TaskStack>, TaskPool> qTaskPool;

int GetBenchRawMaterialNum(int benchNum, int state);
//void CreateMyTaskQueue(int benchNum, TaskStack& ts, bool action, int sellType);
void CreateMyTaskQueue(int benchNum, TaskStack ts, bool action, int sellType);

