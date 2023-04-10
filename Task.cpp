#include "header.h"
priority_queue<TaskStack, vector<TaskStack>, TaskPool> qTaskPool;

/*
	获取当前工作台原材料数量
	state：0虚拟；1真实；2真实+虚拟
	-1：表示没有原材料的工作台
	0~3
*/
int GetBenchRawMaterialNum(int benchNum, int state) {
	int rawState = 0;
	if (state == 0) {
		rawState = vBenchInfo[benchNum].getRawState();
	}
	else if (state == 1) {
		rawState = vBenchInfo[benchNum]._rawStateGoing;
	}
	else {
		rawState = vBenchInfo[benchNum].getRawState() | vBenchInfo[benchNum]._rawStateGoing;
	}
	int cnt = 0;
	if (vBenchInfo[benchNum].getBenchType() > 2 && vBenchInfo[benchNum].getBenchType() < 7) {
		vector<int> vRawMaterial = mRawMaterial[vBenchInfo[benchNum].getBenchType()];
		for (int i = 0; i < vRawMaterial.size(); i++) {
			if ((rawState >> (vRawMaterial[i] + 1)) & 0x01) {
				cnt++;
			}
		}
	}
	else return -1;
	return cnt;
}

void FirstLevel(TaskStack& ts) {
	// 层级
	vector<Task> totalBench = ts._qTaskList;
	Task maxNumTask = totalBench[totalBench.size() - 1];
	Bench currBench = vBenchInfo[maxNumTask._qBench];
	if (currBench.getBenchType() > 2 && currBench.getBenchType() < 7) {
		int rawNum = GetBenchRawMaterialNum(maxNumTask._qBench, 2);
		// 4567优先级才有
		if (sSevenLackRaw.size() != 0) {
			// 7缺了一个原材料，且还有时间生产7时
			int cnt = count(sSevenLackRaw.begin(), sSevenLackRaw.end(), currBench.getBenchType());
			if (cnt) {
				ts._level = TaskLevel::LEVEL8;
				qTaskPool.push(ts);
				//fprintf(stderr, "seven lack %d a raw material!\n", currBench.getBenchType());
				return;
			}
		}
		if (currBench.getBenchType() == 6) {
			// 7类型工作台
			if (rawNum == 0) ts._level = TaskLevel::LEVEL5;
			else if (rawNum == 1) ts._level = TaskLevel::LEVEL6;
			else if (rawNum == 2) ts._level = TaskLevel::LEVEL7;
			else ts._level = TaskLevel::LEVEL5;
			//else ts._level = TaskLevel::LEVEL7;
		}
		else {
			// 456类型工作台
			if (rawNum == 0) ts._level = TaskLevel::LEVEL2;
			else if (rawNum == 1) ts._level = TaskLevel::LEVEL3;
			else ts._level = TaskLevel::LEVEL2;
			//else ts._level = TaskLevel::LEVEL3;
		}
	}
	else if (currBench.getBenchType() == 7) {
		ts._level = TaskLevel::LEVEL9;
		//// 对于78 4778 144778这种情况，78优先级最低，4778、144778最高
		//if (ts._qTaskList.size() > 2) ts._level = TaskLevel::LEVEL9;
		//else ts._level = TaskLevel::LEVEL4;
	}
	else if (currBench.getBenchType() == 8) {
		int lastNum = ts._qTaskList[totalBench.size() - 2]._qBench;
		if (vBenchInfo[lastNum].getBenchType() >= 0 && vBenchInfo[lastNum].getBenchType() < 3) {
			ts._level = TaskLevel::LEVEL2_LOWEST;
		}
		else if (vBenchInfo[lastNum].getBenchType() >= 3 && vBenchInfo[lastNum].getBenchType() < 6) {
			ts._level = TaskLevel::LEVEL3_LOWEST;
		}
		else {
			ts._level = TaskLevel::LEVEL9;
		}
	}
}

void SecondLevel(TaskStack& ts) {
	// 层级
	Bench buyBench = vBenchInfo[ts._qTaskList[ts._qTaskList.size() - 2]._qBench];
	Bench sellBench = vBenchInfo[ts._qTaskList[ts._qTaskList.size() - 1]._qBench];
	if (buyBench.getBenchType() < 3 && sellBench.getBenchType() > 2 && sellBench.getBenchType() < 6) {
		ts._level = TaskLevel::LEVEL_SECOND_2;
	}
	else if (buyBench.getBenchType() > 2 && buyBench.getBenchType() < 6) {
		ts._level = TaskLevel::LEVEL_SECOND_3;
	}
	else if (buyBench.getBenchType() == 6) {
		ts._level = TaskLevel::LEVEL_SECOND_4;
	}
	else {
		ts._level = TaskLevel::LEVEL_SECOND_1;
	}
}

void ThirdLevel(TaskStack& ts) {
	if (ts._qTaskList.size() > 2) ts._level = TaskLevel::LEVEL_THIRD_0;
	else {
		// 层级
		Bench buyBench = vBenchInfo[ts._qTaskList[ts._qTaskList.size() - 2]._qBench];
		if (buyBench.getBenchType() < 3) {
			ts._level = TaskLevel::LEVEL_THIRD_1;
		}
		else if (buyBench.getBenchType() < 6) {
			ts._level = TaskLevel::LEVEL_THIRD_2;
		}
		else {
			ts._level = TaskLevel::LEVEL_THIRD_3;
		}
	}
}

bool SaveTask(TaskStack& ts) {
	// 大于两个工作台、且任务为偶数的任务才保存
	//if (ts._qTaskList.size() != 2 && ts._qTaskList.size() != 3) return false;
	//if (ts._qTaskList.size() < 2 || ts._qTaskList.size() % 2 == 1) return false;
	if (ts._qTaskList.size() < 2) return false;
	if (ts._qTaskList.size() % 2 == 1) {
		// 144弹出最后一个
		ts._qTaskList.pop_back();
	}
	// 清除之前任务链的信息
	ts._income = 0;
	ts._distance = 0.;
	vector<Task> totalBench = ts._qTaskList;
	int maxBenchNum = totalBench[totalBench.size() - 1]._qBench;
	Bench lastBench = vBenchInfo[totalBench[0]._qBench];
	// 收入、距离
	for (int i = 0; i < totalBench.size(); i++) {
		Bench currBench = vBenchInfo[totalBench[i]._qBench];
		ts._distance += DistRPoint2RPoint(currBench.getPos(), lastBench.getPos());
		lastBench = currBench;
		if (!totalBench[i]._qCmd) {
			// 当前买一个货物对应一定能卖出去，计算一次收益
			//fprintf(stderr, "%d	", mBuyBenchGoods[vBenchInfo[totalBench[i]._qBench].getBenchType()]);
			ts._income += mMaxProfit[vBenchInfo[totalBench[i]._qBench].getBenchType()];
		}
	}

	if (currentFrameId < SYS_THIRD_FRAMENUM) {
		FirstLevel(ts);
	}
	else if (currentFrameId < SYS_SECOND_FRAMENUM) {
		SecondLevel(ts);
	}
	else {
		ThirdLevel(ts);
	}

	qTaskPool.push(ts);

	return true;
}

/*
	计算当前所有任务所需帧数之和
*/
int CalcTotalNeedFrame(TaskStack& ts) {
	if (ts._qTaskList.size() <= 1) return INT32_MAX;
	double dist = 0.;
	int lastBenchNum = ts._qTaskList[0]._qBench;
	for (int i = 1; i < ts._qTaskList.size(); i++) {
		int curBenchNum = ts._qTaskList[i]._qBench;
		dist += DistRPoint2RPoint(vBenchInfo[lastBenchNum].getPos(), vBenchInfo[curBenchNum].getPos());
	}
	return (int)ceil(dist * DISTANCE_2_FRAME);
}

//void CreateMyTaskQueue(int benchNum, TaskStack& ts, bool action, int sellType) {
void CreateMyTaskQueue(int benchNum, TaskStack ts, bool action, int sellType) {
	// 当前任务直接压入栈中
	Task currentT;
	currentT._qBench = benchNum;
	currentT._qCmd = action;
	ts._qTaskList.push_back(currentT);
	//fprintf(stderr, "_________________curr task stack size:%d	task:%d\n", ts._qTaskList.size(), action);
	// 买操作是否合法：对应工作台456没有生产好或有机器人正在执行买任务（不合法）?????或走过去还没生产好
	//if (!action && vBenchInfo[benchNum].getBenchType() > 2
	//	&& !vBenchInfo[benchNum].getProduceState()) {
	//if (!action && vBenchInfo[benchNum].getBenchType() > 2
	//	&& (!vBenchInfo[benchNum].getProduceState() || vBenchInfo[benchNum]._buyGoing)) {
	//int benchProductState = (CalcTotalNeedFrame(ts) > vBenchInfo[benchNum].getProductTime()) | vBenchInfo[benchNum].getProduceState();
	int benchProductState = vBenchInfo[benchNum].getProduceState();
	if (!action && vBenchInfo[benchNum].getBenchType() > 2
		&& (!benchProductState || vBenchInfo[benchNum]._buyGoing)) {
		//fprintf(stderr, "-----------------------buy---------------------------\n");
		//fprintf(stderr, "filter success.\n");
		//fprintf(stderr, "-----------------------buy save---------------------------\n");
		ts._qTaskList.pop_back();
		SaveTask(ts);
		return;

	}
	// 卖操作是否合法：对应工作台4567有原材料（不合法）---工作台虚拟状态和工作台状态同时不被占用
	int rawState = vBenchInfo[benchNum].getRawState() >> (sellType + 1);
	int rawStateGoing = vBenchInfo[benchNum]._rawStateGoing >> (sellType + 1);
	rawStateGoing |= rawState;
	if (action && vBenchInfo[benchNum].getBenchType() > 2 && vBenchInfo[benchNum].getBenchType() < 7
		&& (rawStateGoing & 0x01)) {
		//fprintf(stderr, "-----------------------sell---------------------------\n");
		//fprintf(stderr, "filter success.\n");
		//fprintf(stderr, "-----------------------sell save---------------------------\n");
		ts._qTaskList.pop_back();
		SaveTask(ts);
		return;
	}
	// 如果是卖操作，下一步就该买（在当前工作台看能不能买）
	if (action) {
		//fprintf(stderr, "-----------------------curr bench buy---------------------------\n");		
		CreateMyTaskQueue(benchNum, ts, 0, -1);
		return;
	}
	// 如果是买操作，下一步就该卖（到下一级去找工作台卖）
	// 获取当前bench所有子工作台类型map.find(key) == map.end()
	vector<int> subType = mBench2SubBench[vBenchInfo[benchNum].getBenchType()];
	//fprintf(stderr, "bench info:\n");
	//for (int i = 0; i < subType.size(); i++) {
	//	fprintf(stderr, "%d	", subType[i]);
	//}
	//fprintf(stderr, "\n");
	if (subType.empty()) return; // 子工作台编号不存在返回
	for (int i = 0; i < subType.size(); i++) {
		//fprintf(stderr, "bench size:%d\n", mBenchSort[subType[i]].size());
		if (mBenchSort[subType[i]].empty()) continue;
		// 遍历当前类型所有子工作台编号
		vector<int> nowTypeBenchNum = mBenchSort[subType[i]];
		//fprintf(stderr, "next bench list info:\n");
		//for (int num = 0; num < nowTypeBenchNum.size(); num++) {
		//	fprintf(stderr, "%d	", nowTypeBenchNum[num]);
		//}
		//fprintf(stderr, "\n");
		for (int num = 0; num < nowTypeBenchNum.size(); num++) {
			//fprintf(stderr, "curr task stack size:%d\n", ts._qTaskList.size());
			// 到下一级去卖
			//fprintf(stderr, "-----------------------next bench sell---------------------------\n");
			CreateMyTaskQueue(nowTypeBenchNum[num], ts, 1, vBenchInfo[benchNum].getBenchType());
		}
	}
	return;
}

bool TaskPool::operator()(TaskStack tq1, TaskStack tq2) {
	// 层级优先
	if (tq1._level != tq2._level) {
		// tq1小就交换
		return tq1._level < tq2._level;
	}

	//// 计算tq1和tq2任务总体收益：
	double totalIncome1 = tq1._income / tq1._distance;
	double totalIncome2 = tq2._income / tq2._distance;
	return totalIncome1 < totalIncome2;
}

