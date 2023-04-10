#include "header.h"

int actualBenchNum = 0;
int currentFrameId = 0;
vector<Bench> vBenchInfo(MAX_BENCH_SIZE, Bench());
vector<Robot> vRobotInfo(ROBOT_NUM, Robot());
unordered_map<int, vector<int>> mBench2SubBench;
unordered_map<int, int> mMaxProfit;
unordered_map<int, vector<int>> mRawMaterial;
unordered_map<int, vector<int>> mBenchSort;

// task pool
unordered_map<int, int> mBuyBenchGoods; // �����Ӧ�������
unordered_map<int, int> mSellBenchGoods; // �����Ӧ�������
set<int> sSevenLackRaw; // 7���͹���̨ȱ��ԭ��������
unordered_map<int, set<int>> mSevenRawState;
vector<int> needResetNum; // ����Ŀ�����Ҫ���λ�Ļ�����

double param1 = 0.;
double param2 = 0.;
int profitMoney = 0;
string savePath = "d:\\result\\result";
string mapNum;

void InitMapInfo(const vector<string>& vs) {
	int robotId = 0;
	for (int i = 0; i < vs.size(); i++) {
		string sTmp(vs[i]);
		int len = strlen(vs[i].c_str());
		remove(sTmp.begin(), sTmp.end(), ' ');
		for (int j = 0; j < 100; j++) {
			RPoint nowPos = RPoint{ (double)j, (double)i };
			RPoint actualPos = Pos2Actual(nowPos);
			if (sTmp[j] == '.') continue;
			else if (sTmp[j] == 'A') {
				Robot r(-1, actualPos.x, actualPos.y, 0, 0, 0, 0, 0, 0, 0);
				vRobotInfo[robotId] = r;
				robotId++;
			}
			else {
				Bench bench(actualBenchNum, sTmp[j] - '1', actualPos.x, actualPos.y, -1, 0, 0);
				vBenchInfo[actualBenchNum] = bench;
				actualBenchNum++;
			}
		}
	}
}

void SysInit() {
	mBench2SubBench[0] = vector<int>({ 4 - 1, 5 - 1, 9 - 1 });
	mBench2SubBench[1] = vector<int>({ 4 - 1, 6 - 1, 9 - 1 });
	mBench2SubBench[2] = vector<int>({ 5 - 1, 6 - 1, 9 - 1 });
	mBench2SubBench[3] = vector<int>({ 7 - 1, 9 - 1 });
	mBench2SubBench[4] = vector<int>({ 7 - 1, 9 - 1 });
	mBench2SubBench[5] = vector<int>({ 7 - 1, 9 - 1 });
	mBench2SubBench[6] = vector<int>({ 8 - 1, 9 - 1 });

	mBuyBenchGoods[0] = 3000; mSellBenchGoods[0] = 6000; mMaxProfit[0] = 3000;
	mBuyBenchGoods[1] = 4400; mSellBenchGoods[1] = 7600; mMaxProfit[1] = 3200;
	mBuyBenchGoods[2] = 5800; mSellBenchGoods[2] = 9200; mMaxProfit[2] = 3400;
	mBuyBenchGoods[3] = 15400; mSellBenchGoods[3] = 22500; mMaxProfit[3] = 7100;
	mBuyBenchGoods[4] = 17200; mSellBenchGoods[4] = 25000; mMaxProfit[4] = 7800;
	mBuyBenchGoods[5] = 19200; mSellBenchGoods[5] = 27500; mMaxProfit[5] = 8300;
	mBuyBenchGoods[6] = 76000; mSellBenchGoods[6] = 105000; mMaxProfit[6] = 29000;

	mRawMaterial[4 - 1] = vector<int>({ 1 - 1, 2 - 1 });
	mRawMaterial[5 - 1] = vector<int>({ 1 - 1, 3 - 1 });
	mRawMaterial[6 - 1] = vector<int>({ 2 - 1, 3 - 1 });
	mRawMaterial[7 - 1] = vector<int>({ 4 - 1, 5 - 1, 6 - 1 });

	//InitBenchList();
	for (int i = 0; i < actualBenchNum; i++) {
		// ������̨�����ͽ�������
		Bench curBench = vBenchInfo[i];
		mBenchSort[curBench.getBenchType()].emplace_back(i);
	}
}

void ConvertString2VectorInfo(const string& str, vector<double>& v) {
	string tmpStr = str;
	int dataIndex = 0;
	while (1) {
		int index = tmpStr.find(' ');
		//fprintf(stderr, "dataIndex:%d������' '��������:%d\n", dataIndex, index);
		if (index == -1) {
			v[dataIndex] = stold(tmpStr.c_str());
			return;
		}
		v[dataIndex] = stold(tmpStr.substr(0, index).c_str());
		tmpStr = tmpStr.substr(index + 1); // �����ַ���
		dataIndex++;
	}
}

void UpDataInfo(const vector<string>& vs) {
	string tmpStr = vs[0];
	int index = tmpStr.find(' ');
	profitMoney = atoi(tmpStr.substr(index + 1).c_str());
	int benchNum = atoi(vs[1].c_str());
	// ���¹���̨��Ϣ
	for (int i = 0; i < benchNum; i++) {
		vector<double> v(6, -1);
		ConvertString2VectorInfo(vs[i + 2], v);
		vBenchInfo[i].setProductTime((int)v[3]);
		vBenchInfo[i].setRawState((int)v[4]);
		vBenchInfo[i].setProduceState((int)v[5]);
	}
	// ���»�������Ϣ
	for (int i = 0; i < 4; i++) {
		vector<double> v(10, -1);
		ConvertString2VectorInfo(vs[i + 2 + benchNum], v);
		vRobotInfo[i].setId(v[0]);
		vRobotInfo[i].setGoodsId(v[1]);
		vRobotInfo[i].setTimeCoff(v[2]);
		vRobotInfo[i].setCollisionCoff(v[3]);
		vRobotInfo[i].setAngleVel(v[4]);
		vRobotInfo[i].setLineVelx(v[5]);
		vRobotInfo[i].setLineVely(v[6]);
		vRobotInfo[i].setRad(v[7]);
		vRobotInfo[i].setPos(RPoint{ v[8], v[9] });
	}

	mSevenRawState.clear();
	sSevenLackRaw.clear();

	// ��ȡ7���͹���̨��ԭ����Ϣ
	for (int i = 0; i < mBenchSort[6].size(); i++) {
		int sevenNum = mBenchSort[6][i];
		for (int raw = 0; raw < 3; raw++) {
			int rawState = vBenchInfo[sevenNum].getRawState() | vBenchInfo[sevenNum]._rawStateGoing;
			if ((rawState >> (raw + 4)) & 0x01) {
				mSevenRawState[sevenNum].insert(raw + 4);
			}
		}
		if (mSevenRawState[sevenNum].size() == 2) {
			// ֻ��¼��һ��ԭ����ʱ
			set<int> sTotal({ 4,5,6 });
			vector<int> diffOut(1);
			//sort(mSevenRawState[sevenNum].begin(), mSevenRawState[sevenNum].end());
			set_difference(sTotal.begin(), sTotal.end(),
				mSevenRawState[sevenNum].begin(), mSevenRawState[sevenNum].end(),
				diffOut.begin());
			// �Ƿ��ж�Ӧ��ԭ�������ڴ���
			bool whetherHaveRaw = false; // û���ڴ���
			vector<int> lackRawTotal = mBenchSort[diffOut[0] - 1];
			vector<int> rawType = mRawMaterial[diffOut[0] - 1];
			for (int rawIndex = 0; rawIndex < lackRawTotal.size(); rawIndex++) {
				int rawCnt = 0;
				for (int rawT = 0; rawT < rawType.size(); rawT++) {
					int needRaw = vBenchInfo[lackRawTotal[rawIndex]].getRawState() | vBenchInfo[lackRawTotal[rawIndex]]._rawStateGoing;
					if ((needRaw >> (rawT + 1)) & 0x01) {
						rawCnt++;
					}
				}
				if (rawCnt == 2) {
					// �Ѿ���������
					whetherHaveRaw = true;
					break;
				}
			}
			if (!whetherHaveRaw) {
				//fprintf(stderr, "7ȱʧ��ԭ�������ڼӹ�:%d\n", diffOut[0] - 1);
				sSevenLackRaw.insert(diffOut[0] - 1);
			}
			//else fprintf(stderr, "7ȱʧ��ԭ����Ϊ:%d\n", diffOut[0] - 1);
		}
	}
}

void PrintRobotInfo() {
	for (int i = 0; i < vRobotInfo.size(); i++) {
	}
	fprintf(stderr, "--------------------------------------CurrentRobotInfo:--------------------------------------\n");
	for (int i = 0; i < ROBOT_NUM; i++) {
		//fprintf(
		//	stderr,
		//	"num:%d	benchId:%d	goodsId:%d	timeCoff:%0.2f	collisionCoff:%0.2f	angleVel:%0.2f	LineVelx:%0.2f	LineVely:%0.2f	rad:%0.2f	x:%0.2f   y:%0.2f\n",
		//	i, vRobotInfo[i].getId(), vRobotInfo[i].getGoodsId(), vRobotInfo[i].getTimeCoff(),
		//	vRobotInfo[i].getCollisionCoff(), vRobotInfo[i].getAngleVel(),
		//	vRobotInfo[i].getLineVelx(), vRobotInfo[i].getLineVely(), 
		//	vRobotInfo[i].getRad(), vRobotInfo[i].getPos().x, vRobotInfo[i].getPos().y
		//);	
		fprintf(stderr, "robotid:%d		task:", i);
		if (vRobotInfo[i]._vTask.size()) {
			for (int j = 0; j < vRobotInfo[i]._vTask.size(); j++) {
				fprintf(stderr, "%d(%d)->",
					vRobotInfo[i]._vTask[j]._qBench,
					vBenchInfo[vRobotInfo[i]._vTask[j]._qBench].getBenchType());
			}
		}
		fprintf(stderr, "\n");
	}
	//fprintf(stderr, "\n");
}

void PrintBenchInfo() {
	fprintf(stderr, "--------------------------------------CurrentBenchInfo:--------------------------------------\n");
	for (int i = 0; i < vBenchInfo.size(); i++) {
		fprintf(
			stderr,
			"num: %d	type:%d	raw material:%d	goingState:%d	product:%d	buyGoing:%d\n",
			i, vBenchInfo[i].getBenchType(), vBenchInfo[i].getRawState(),
			vBenchInfo[i]._rawStateGoing, vBenchInfo[i].getProduceState(), vBenchInfo[i]._buyGoing
		);
	}
	//fprintf(stderr, "\n");
}

void PrintTaskPoolInfo() {
	PrintBenchInfo();
	fprintf(stderr, "cur frame:%d   task pool info:%d\n", currentFrameId, qTaskPool.size());
	{
		priority_queue<TaskStack, vector<TaskStack>, TaskPool> qtemp = qTaskPool;
		fprintf(stderr, "frame id:%d\n", currentFrameId);
		// ��ӡ�����
		while (!qtemp.empty()) {
			TaskStack ts = qtemp.top();
			qtemp.pop();
			for (int i = 0; i < ts._qTaskList.size(); i++) {
				fprintf(stderr, "num:%d(%d, level:%d) ",
					ts._qTaskList[i]._qBench, vBenchInfo[ts._qTaskList[i]._qBench].getBenchType(),
					ts._level);
			}
			fprintf(stderr, "\n");
		}
	}
}