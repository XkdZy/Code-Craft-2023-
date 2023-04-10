#include "header.h"

/*
	当给机器人下达指令后，工作台虚拟状态置位（4567）
*/
void SetBenchGoingState(Robot& r, int num) {
	for (int j = 0; j < r._vTask.size(); j++) {
		//fprintf(stderr, "------------------4---------------\n");
		// 工作台虚拟状态置位，只对4567工作台有效
		if (vBenchInfo[r._vTask[j]._qBench].getBenchType() > 2 &&
			vBenchInfo[r._vTask[j]._qBench].getBenchType() < 7) {
			if (r._vTask[j]._qCmd) {
				// 卖：把原材料格状态更新
				vBenchInfo[r._vTask[j]._qBench]._rawStateGoing |=
					0x01 << (vBenchInfo[r._vTask[j - 1]._qBench].getBenchType() + 1);
			}
			else {
				vBenchInfo[r._vTask[j]._qBench]._buyGoing = 1;		
			}
		}
		//fprintf(stderr, "------------------5---------------\n");
		if (!r._vTask[j]._qCmd) {
			// 如果当前为买，记录原材料类型，方便卖时清空
			//fprintf(stderr, "------------------7---------------\n");
			//fprintf(stderr, "curr robot id:%d	\n", num);
			for (int i = 0; i < vRobotInfo[num]._vTask.size(); i++) {
				fprintf(stderr, "bench:%d(%d)	", vRobotInfo[num]._vTask[i]._qBench, vRobotInfo[num]._vTask[i]._qCmd);
			}
			vBenchInfo[r._vTask[j + 1]._qBench]._vRawGoingLast.emplace_back(pair<int, int>(num, vBenchInfo[r._vTask[j]._qBench].getBenchType()));
		}
		//fprintf(stderr, "------------------6---------------\n");
	}
}

/*
	当给机器人到达目标后，工作台虚拟状态复位（4567）
*/
void ResetBenchGoingState(Robot& r, int num) {
	r._radCnt = 0;
	if (!r._vTask[0]._qCmd) {
		//// 如果当前为买，记录原材料类型，方便卖时清空
		//vBenchInfo[r._vTask[1]._qBench]._vRawGoingLast.emplace_back(pair<int, int>(num, vBenchInfo[r._vTask[0]._qBench].getBenchType()));
		//vBenchInfo[r._vTask[1]._qBench]._rawStateGoingLast = vBenchInfo[r._vTask[0]._qBench].getBenchType();
		if (vBenchInfo[r._vTask[0]._qBench].getBenchType() > 2 &&
			vBenchInfo[r._vTask[0]._qBench].getBenchType() < 7) {
			vBenchInfo[r._vTask[0]._qBench]._buyGoing = 0;
		}
	}
	else {
		// 卖
		if (vBenchInfo[r._vTask[0]._qBench].getBenchType() > 2 &&
			vBenchInfo[r._vTask[0]._qBench].getBenchType() < 7) {
			// 找到对应机器人
			for (int i = 0; i < vBenchInfo[r._vTask[0]._qBench]._vRawGoingLast.size(); i++) {
				if (vBenchInfo[r._vTask[0]._qBench]._vRawGoingLast[i].first == num) {
					vBenchInfo[r._vTask[0]._qBench]._rawStateGoing &=
						(~(0x01 << (vBenchInfo[r._vTask[0]._qBench]._vRawGoingLast[i].second + 1)));
					vBenchInfo[r._vTask[0]._qBench]._vRawGoingLast.erase(i + vBenchInfo[r._vTask[0]._qBench]._vRawGoingLast.begin());
				}
			}
		}
		//{
		//	if (r._vTask.size()) {
		//		fprintf(stderr, "		reset robot info:%d task level:%d(length:%d)",
		//			num, r._tstack._level, r._vTask.size());
		//			int benchId = r._vTask[0]._qBench;
		//			fprintf(stderr, "bench id:%d(_rawStateGoing:%d	type:%d)\n\n",
		//				benchId, vBenchInfo[benchId]._rawStateGoing | vBenchInfo[benchId].getRawState(), vBenchInfo[benchId].getBenchType());
		//		
		//	}
		//}
	}


	return;
}

/*
	判断机器人当前任务的合法性，是否是之前任务的耦合任务1-11(12)-11(12)-7
*/
bool VertifyBenchGoingAvailable(Robot& r, int num) {
	//fprintf(stderr, "task stack:%d\n", r._vTask.size());
	// 滤除不能在规定时间完成的任务
	double robot2First = DistRPoint2RPoint(vBenchInfo[r._vTask[0]._qBench].getPos(), r.getPos());
	if ((int)ceil((r._tstack._distance + robot2First) * 50 / 4.2) > (SYS_MAX_FRAMENUM - currentFrameId)) {
		return false;
	}

	for (int j = 0; j < r._vTask.size(); j++) {
		// 工作台虚拟状态置位，只对4567工作台有效
		if (vBenchInfo[r._vTask[j]._qBench].getBenchType() > 2 &&
			vBenchInfo[r._vTask[j]._qBench].getBenchType() < 7) {
			if (r._vTask[j]._qCmd) {
				//fprintf(stderr, "vertify bench num:%d(raw:%d,product:%d) ",
				//	r._vTask[j]._qBench, vBenchInfo[r._vTask[j]._qBench]._rawStateGoing,
				//	vBenchInfo[r._vTask[j]._qBench]._productStateGoing);
				
				int rawState = vBenchInfo[r._vTask[j]._qBench].getRawState();
				int rawStateGoing = vBenchInfo[r._vTask[j]._qBench]._rawStateGoing;
				rawStateGoing |= rawState;
				// 卖：判断当前卖给工作台的虚拟状态生产格是否有该货物
				if ((rawStateGoing >> (vBenchInfo[r._vTask[j - 1]._qBench].getBenchType() + 1)) & 0x01) {
					//fprintf(stderr, "验证任务是否可靠 err! 工作台%d冲突，有货物:%d\n",
					//	r._vTask[j]._qBench, (vBenchInfo[r._vTask[j - 1]._qBench].getBenchType() + 1));
					return false;
				}
			}
			else {
				// 买
				if (vBenchInfo[r._vTask[j]._qBench]._buyGoing == 1) {
					//// print test
					//if (vBenchInfo[r._vTask[j]._qBench].getBenchType() == 6) {
					//	fprintf(stderr, "验证任务是否可靠 err! 有机器人正在买\n");
					//}
					return false;
				}
			}
		}
	}

	return true;
}

// 处理当机器人是否移动到目标点
bool VerityRobot2Goal(Robot& r, int num) {
	if (r._vTask.empty()) return false; // 没有任务
	//if (r.getId() != r._vTask[0]._qBench) {
	if (r.getId() != r._vTask[0]._qBench) {
		//fprintf(stderr, "robot %d going to target!\n", num);
		// 机器人没有到达目标点：计算旋转、平移偏差
		Bench goal = vBenchInfo[r._vTask[0]._qBench];
		double dist = DistRPoint2RPoint(r.getPos(), goal.getPos()); // 当前机器人和工作台距离
		double rotate = Point2Rad2(r.getPos(), goal.getPos()) - r.getRad();
		int frame = rotate * 1000 / 20 / ROBOT_MAX_ROTATESPEED;
		// 限幅，限制在-Π到Π
		if (rotate > RPI) rotate -= 2 * RPI;
		else if (rotate < -RPI) rotate += 2 * RPI;

#if SYS_DEBUG
		double radSpd = 4 * rotate;
		double speed = param1 + param2 * (RPI - abs(r.getAngleVel())) / RPI;
#else 
		double radSpd = 4 * rotate;;
		double speed = 4.1 + 5.7 * (RPI - abs(r.getAngleVel())) / RPI; // 64.5
#endif

		if (abs(rotate) < 0.08) {
			radSpd = 0;
		}
		else {
			int mindis = 1.5;
			int middis = 2;
			int minspeed = 0.06;
			int midspeed = 0.6;

			if (r.getRad() > 0 && r.getRad() < RPI / 2) {
				if (((50 - r.getPos().y) <= mindis) || ((50 - r.getPos().x) <= mindis))
					speed = minspeed;
				else if (((50 - r.getPos().y) <= middis) || ((50 - r.getPos().x) <= middis))
					speed = midspeed;
			}
			else if (r.getRad() > RPI / 2 && r.getRad() < RPI) {
				if ((50 - r.getPos().y) <= mindis || r.getPos().x <= mindis)
					speed = minspeed;
				else if ((50 - r.getPos().y) <= middis || r.getPos().x <= middis)
					speed = midspeed;
			}
			else if (r.getRad() > -RPI && r.getRad() < -RPI / 2) {
				if (r.getPos().y <= mindis || r.getPos().x <= mindis)
					speed = minspeed;
				else if (r.getPos().y <= middis || r.getPos().x <= middis)
					speed = midspeed;
			}
			else if (r.getRad() > -RPI / 2 && r.getRad() < 0) {
				if (r.getPos().y <= mindis || (50 - r.getPos().x) <= mindis)
					speed = minspeed;
				if (r.getPos().y <= middis || (50 - r.getPos().x) <= middis)
					speed = midspeed;
			}
		}

		if (abs(r.getAngleVel() < RPI / 6.)) {
			r._radCnt = 0;
		}
		if (abs(r.getAngleVel()) > RPI / 6.) {
			r._radCnt++;
		}

		if (r._radCnt > 120 && abs(r.getAngleVel()) > RPI / 2.5) {
			speed = 0;
			r._radCnt++;
		}
		if (r._radCnt > 160) {
			r._radCnt = 0;
		}

		printf("rotate %d %f\n", num, radSpd);
		printf("forward %d %f\n", num, speed);
	}
	else {
		// 到达目标
		printf("rotate %d %f\n", num, 0.);
		printf("forward %d %f\n", num, 0.);
		if (r._vTask[0]._qCmd) printf("sell %d\n", num);
		else printf("buy %d\n", num);

		needResetNum.emplace_back(num);
	}
	return true;
}

/*
* 当优先级（小于等于8），加上机器人距离，计算单位距离收益
*/
bool SameLevelSelectMinRobot2(Robot& r, int num, priority_queue<TaskStack, vector<TaskStack>, TaskPool>& pq) {
	if (pq.empty()) return false;
	int currentLevel = 0;
	// 确定满足要求的一个任务所处的优先级
	while (1) {
		currentLevel = pq.top()._level;
		r._tstack = pq.top();
		r._vTask = pq.top()._qTaskList;
		if (VertifyBenchGoingAvailable(r, num)) break;
		pq.pop();
		if (pq.empty()) return false;
	}
	// 大根堆存放单位距离收益
	priority_queue<pair<double, int>, vector<pair<double, int>>, less<pair<double, int>>> pqMinTime;
	int stCount = 0;
	vector<TaskStack> vSameLevel = vector<TaskStack>();
	while (!pq.empty()) {
		// 当前任务
		TaskStack currTS = pq.top();
		pq.pop();
		r._tstack = currTS;
		r._vTask = currTS._qTaskList;
		if (currentLevel != currTS._level) break;
		if (!VertifyBenchGoingAvailable(r, num)) continue;
		double dist = DistRPoint2RPoint(r.getPos(), vBenchInfo[currTS._qTaskList[0]._qBench].getPos());
		double profit = 0.;
		profit = currTS._income / (dist + currTS._distance);
		pqMinTime.push(pair<double, int>(profit, stCount));
		vSameLevel.push_back(currTS);
		stCount++;
	}
	//fprintf(stderr, "当前任务数量:%d,同优先级数量:%d\n", pqMinTime.size(), vSameLevel.size());
	if (vSameLevel.empty())  return false;
	r._tstack = vSameLevel[pqMinTime.top().second];
	r._vTask = r._tstack._qTaskList;

	return true;
}

void Task_Pool() {
	needResetNum.clear();


	for (int i = 0; i < ROBOT_NUM; i++) {
		//fprintf(stderr, "task pool num:%d\n", qTaskPool.size());
		// 当多个机器人去同一个123时，可能有的机器人没有拿到123，此时删除当前任务
		if (vRobotInfo[i].getGoodsId() == 0 && vRobotInfo[i]._vTask.size() > 0 && vRobotInfo[i]._vTask[0]._qCmd) {
			for (int j = 0; j < vRobotInfo[i]._vTask.size(); j++) {
				ResetBenchGoingState(vRobotInfo[i], i);
			}
			vRobotInfo[i]._vTask.clear();
			vRobotInfo[i]._tstack = TaskStack();
		}
		// 判断机器人是否有任务，如果有直接执行任务
		if (VerityRobot2Goal(vRobotInfo[i], i)) continue;

		// 没有任务的机器人取从任务池中取任务
		// 取任务同一优先级的任务，加上机器人距离重新排序
		priority_queue<TaskStack, vector<TaskStack>, TaskPool> qTmp = qTaskPool;
		//bool ret = SameLevelSelectMinRobot(vRobotInfo[i], qTmp);
		bool ret = SameLevelSelectMinRobot2(vRobotInfo[i], i, qTmp);
		if (!ret) {
			// 没有合法任务
			vRobotInfo[i]._tstack = TaskStack();
			vRobotInfo[i]._vTask = vector<Task>();
		}

		SetBenchGoingState(vRobotInfo[i], i);
	}

	// 最后更新虚拟状态
	for (int i = 0; i < needResetNum.size(); i++) {
		// 机器人就继续下发下一个任务
		ResetBenchGoingState(vRobotInfo[needResetNum[i]], needResetNum[i]);
		vRobotInfo[needResetNum[i]]._vTask.erase(vRobotInfo[needResetNum[i]]._vTask.begin());
	}
}


void RobotCollosion(Robot & r1, Robot & r2, int num1, int num2) {
	double dist = DistRPoint2RPoint(r1.getPos(), r2.getPos());
	double rotate = abs(abs(r1.getRad() - r2.getRad()) - RPI);
	double rotateDir = Point2Rad2(r1.getPos(), r2.getPos()) - r1.getRad();
	if (rotateDir > RPI) rotateDir -= 2 * RPI;
	else if (rotateDir < -RPI) rotateDir += 2 * RPI;
	if (dist < 2) {
		if (rotate < RPI / 8.) {
			//printf("forward %d %f\n", num1, 0.);
			printf("rotate %d %f\n", num1, rotateDir > 0 ? -RPI : RPI);
			//printf("forward %d %f\n", num2, 0.);
			//printf("rotate %d %f\n", num2, rotateDir > 0 ? -RPI : RPI);
			//r1._rotateFlag = true;
			//r2._rotateFlag = true;
		}
	}
}

void IsRobotCollosion() {
	for (int i = 0; i < ROBOT_NUM; i++) {
		for (int j = i + 1; j < ROBOT_NUM; j++) {
			if (j != i)
				RobotCollosion(vRobotInfo[i], vRobotInfo[j], i, j);
			else continue;
		}
	}
}

