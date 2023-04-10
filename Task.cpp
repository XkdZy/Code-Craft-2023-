#include "header.h"
priority_queue<TaskStack, vector<TaskStack>, TaskPool> qTaskPool;

/*
	��ȡ��ǰ����̨ԭ��������
	state��0���⣻1��ʵ��2��ʵ+����
	-1����ʾû��ԭ���ϵĹ���̨
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
	// �㼶
	vector<Task> totalBench = ts._qTaskList;
	Task maxNumTask = totalBench[totalBench.size() - 1];
	Bench currBench = vBenchInfo[maxNumTask._qBench];
	if (currBench.getBenchType() > 2 && currBench.getBenchType() < 7) {
		int rawNum = GetBenchRawMaterialNum(maxNumTask._qBench, 2);
		// 4567���ȼ�����
		if (sSevenLackRaw.size() != 0) {
			// 7ȱ��һ��ԭ���ϣ��һ���ʱ������7ʱ
			int cnt = count(sSevenLackRaw.begin(), sSevenLackRaw.end(), currBench.getBenchType());
			if (cnt) {
				ts._level = TaskLevel::LEVEL8;
				qTaskPool.push(ts);
				//fprintf(stderr, "seven lack %d a raw material!\n", currBench.getBenchType());
				return;
			}
		}
		if (currBench.getBenchType() == 6) {
			// 7���͹���̨
			if (rawNum == 0) ts._level = TaskLevel::LEVEL5;
			else if (rawNum == 1) ts._level = TaskLevel::LEVEL6;
			else if (rawNum == 2) ts._level = TaskLevel::LEVEL7;
			else ts._level = TaskLevel::LEVEL5;
			//else ts._level = TaskLevel::LEVEL7;
		}
		else {
			// 456���͹���̨
			if (rawNum == 0) ts._level = TaskLevel::LEVEL2;
			else if (rawNum == 1) ts._level = TaskLevel::LEVEL3;
			else ts._level = TaskLevel::LEVEL2;
			//else ts._level = TaskLevel::LEVEL3;
		}
	}
	else if (currBench.getBenchType() == 7) {
		ts._level = TaskLevel::LEVEL9;
		//// ����78 4778 144778���������78���ȼ���ͣ�4778��144778���
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
	// �㼶
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
		// �㼶
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
	// ������������̨��������Ϊż��������ű���
	//if (ts._qTaskList.size() != 2 && ts._qTaskList.size() != 3) return false;
	//if (ts._qTaskList.size() < 2 || ts._qTaskList.size() % 2 == 1) return false;
	if (ts._qTaskList.size() < 2) return false;
	if (ts._qTaskList.size() % 2 == 1) {
		// 144�������һ��
		ts._qTaskList.pop_back();
	}
	// ���֮ǰ����������Ϣ
	ts._income = 0;
	ts._distance = 0.;
	vector<Task> totalBench = ts._qTaskList;
	int maxBenchNum = totalBench[totalBench.size() - 1]._qBench;
	Bench lastBench = vBenchInfo[totalBench[0]._qBench];
	// ���롢����
	for (int i = 0; i < totalBench.size(); i++) {
		Bench currBench = vBenchInfo[totalBench[i]._qBench];
		ts._distance += DistRPoint2RPoint(currBench.getPos(), lastBench.getPos());
		lastBench = currBench;
		if (!totalBench[i]._qCmd) {
			// ��ǰ��һ�������Ӧһ��������ȥ������һ������
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
	���㵱ǰ������������֡��֮��
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
	// ��ǰ����ֱ��ѹ��ջ��
	Task currentT;
	currentT._qBench = benchNum;
	currentT._qCmd = action;
	ts._qTaskList.push_back(currentT);
	//fprintf(stderr, "_________________curr task stack size:%d	task:%d\n", ts._qTaskList.size(), action);
	// ������Ƿ�Ϸ�����Ӧ����̨456û�������û��л���������ִ�������񣨲��Ϸ���?????���߹�ȥ��û������
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
	// �������Ƿ�Ϸ�����Ӧ����̨4567��ԭ���ϣ����Ϸ���---����̨����״̬�͹���̨״̬ͬʱ����ռ��
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
	// ���������������һ���͸����ڵ�ǰ����̨���ܲ�����
	if (action) {
		//fprintf(stderr, "-----------------------curr bench buy---------------------------\n");		
		CreateMyTaskQueue(benchNum, ts, 0, -1);
		return;
	}
	// ��������������һ���͸���������һ��ȥ�ҹ���̨����
	// ��ȡ��ǰbench�����ӹ���̨����map.find(key) == map.end()
	vector<int> subType = mBench2SubBench[vBenchInfo[benchNum].getBenchType()];
	//fprintf(stderr, "bench info:\n");
	//for (int i = 0; i < subType.size(); i++) {
	//	fprintf(stderr, "%d	", subType[i]);
	//}
	//fprintf(stderr, "\n");
	if (subType.empty()) return; // �ӹ���̨��Ų����ڷ���
	for (int i = 0; i < subType.size(); i++) {
		//fprintf(stderr, "bench size:%d\n", mBenchSort[subType[i]].size());
		if (mBenchSort[subType[i]].empty()) continue;
		// ������ǰ���������ӹ���̨���
		vector<int> nowTypeBenchNum = mBenchSort[subType[i]];
		//fprintf(stderr, "next bench list info:\n");
		//for (int num = 0; num < nowTypeBenchNum.size(); num++) {
		//	fprintf(stderr, "%d	", nowTypeBenchNum[num]);
		//}
		//fprintf(stderr, "\n");
		for (int num = 0; num < nowTypeBenchNum.size(); num++) {
			//fprintf(stderr, "curr task stack size:%d\n", ts._qTaskList.size());
			// ����һ��ȥ��
			//fprintf(stderr, "-----------------------next bench sell---------------------------\n");
			CreateMyTaskQueue(nowTypeBenchNum[num], ts, 1, vBenchInfo[benchNum].getBenchType());
		}
	}
	return;
}

bool TaskPool::operator()(TaskStack tq1, TaskStack tq2) {
	// �㼶����
	if (tq1._level != tq2._level) {
		// tq1С�ͽ���
		return tq1._level < tq2._level;
	}

	//// ����tq1��tq2�����������棺
	double totalIncome1 = tq1._income / tq1._distance;
	double totalIncome2 = tq2._income / tq2._distance;
	return totalIncome1 < totalIncome2;
}

