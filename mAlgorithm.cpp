#include "header.h"

/*
	�����������´�ָ��󣬹���̨����״̬��λ��4567��
*/
void SetBenchGoingState(Robot& r, int num) {
	for (int j = 0; j < r._vTask.size(); j++) {
		//fprintf(stderr, "------------------4---------------\n");
		// ����̨����״̬��λ��ֻ��4567����̨��Ч
		if (vBenchInfo[r._vTask[j]._qBench].getBenchType() > 2 &&
			vBenchInfo[r._vTask[j]._qBench].getBenchType() < 7) {
			if (r._vTask[j]._qCmd) {
				// ������ԭ���ϸ�״̬����
				vBenchInfo[r._vTask[j]._qBench]._rawStateGoing |=
					0x01 << (vBenchInfo[r._vTask[j - 1]._qBench].getBenchType() + 1);
			}
			else {
				vBenchInfo[r._vTask[j]._qBench]._buyGoing = 1;		
			}
		}
		//fprintf(stderr, "------------------5---------------\n");
		if (!r._vTask[j]._qCmd) {
			// �����ǰΪ�򣬼�¼ԭ�������ͣ�������ʱ���
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
	���������˵���Ŀ��󣬹���̨����״̬��λ��4567��
*/
void ResetBenchGoingState(Robot& r, int num) {
	r._radCnt = 0;
	if (!r._vTask[0]._qCmd) {
		//// �����ǰΪ�򣬼�¼ԭ�������ͣ�������ʱ���
		//vBenchInfo[r._vTask[1]._qBench]._vRawGoingLast.emplace_back(pair<int, int>(num, vBenchInfo[r._vTask[0]._qBench].getBenchType()));
		//vBenchInfo[r._vTask[1]._qBench]._rawStateGoingLast = vBenchInfo[r._vTask[0]._qBench].getBenchType();
		if (vBenchInfo[r._vTask[0]._qBench].getBenchType() > 2 &&
			vBenchInfo[r._vTask[0]._qBench].getBenchType() < 7) {
			vBenchInfo[r._vTask[0]._qBench]._buyGoing = 0;
		}
	}
	else {
		// ��
		if (vBenchInfo[r._vTask[0]._qBench].getBenchType() > 2 &&
			vBenchInfo[r._vTask[0]._qBench].getBenchType() < 7) {
			// �ҵ���Ӧ������
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
	�жϻ����˵�ǰ����ĺϷ��ԣ��Ƿ���֮ǰ������������1-11(12)-11(12)-7
*/
bool VertifyBenchGoingAvailable(Robot& r, int num) {
	//fprintf(stderr, "task stack:%d\n", r._vTask.size());
	// �˳������ڹ涨ʱ����ɵ�����
	double robot2First = DistRPoint2RPoint(vBenchInfo[r._vTask[0]._qBench].getPos(), r.getPos());
	if ((int)ceil((r._tstack._distance + robot2First) * 50 / 4.2) > (SYS_MAX_FRAMENUM - currentFrameId)) {
		return false;
	}

	for (int j = 0; j < r._vTask.size(); j++) {
		// ����̨����״̬��λ��ֻ��4567����̨��Ч
		if (vBenchInfo[r._vTask[j]._qBench].getBenchType() > 2 &&
			vBenchInfo[r._vTask[j]._qBench].getBenchType() < 7) {
			if (r._vTask[j]._qCmd) {
				//fprintf(stderr, "vertify bench num:%d(raw:%d,product:%d) ",
				//	r._vTask[j]._qBench, vBenchInfo[r._vTask[j]._qBench]._rawStateGoing,
				//	vBenchInfo[r._vTask[j]._qBench]._productStateGoing);
				
				int rawState = vBenchInfo[r._vTask[j]._qBench].getRawState();
				int rawStateGoing = vBenchInfo[r._vTask[j]._qBench]._rawStateGoing;
				rawStateGoing |= rawState;
				// �����жϵ�ǰ��������̨������״̬�������Ƿ��иû���
				if ((rawStateGoing >> (vBenchInfo[r._vTask[j - 1]._qBench].getBenchType() + 1)) & 0x01) {
					//fprintf(stderr, "��֤�����Ƿ�ɿ� err! ����̨%d��ͻ���л���:%d\n",
					//	r._vTask[j]._qBench, (vBenchInfo[r._vTask[j - 1]._qBench].getBenchType() + 1));
					return false;
				}
			}
			else {
				// ��
				if (vBenchInfo[r._vTask[j]._qBench]._buyGoing == 1) {
					//// print test
					//if (vBenchInfo[r._vTask[j]._qBench].getBenchType() == 6) {
					//	fprintf(stderr, "��֤�����Ƿ�ɿ� err! �л�����������\n");
					//}
					return false;
				}
			}
		}
	}

	return true;
}

// �����������Ƿ��ƶ���Ŀ���
bool VerityRobot2Goal(Robot& r, int num) {
	if (r._vTask.empty()) return false; // û������
	//if (r.getId() != r._vTask[0]._qBench) {
	if (r.getId() != r._vTask[0]._qBench) {
		//fprintf(stderr, "robot %d going to target!\n", num);
		// ������û�е���Ŀ��㣺������ת��ƽ��ƫ��
		Bench goal = vBenchInfo[r._vTask[0]._qBench];
		double dist = DistRPoint2RPoint(r.getPos(), goal.getPos()); // ��ǰ�����˺͹���̨����
		double rotate = Point2Rad2(r.getPos(), goal.getPos()) - r.getRad();
		int frame = rotate * 1000 / 20 / ROBOT_MAX_ROTATESPEED;
		// �޷���������-������
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
		// ����Ŀ��
		printf("rotate %d %f\n", num, 0.);
		printf("forward %d %f\n", num, 0.);
		if (r._vTask[0]._qCmd) printf("sell %d\n", num);
		else printf("buy %d\n", num);

		needResetNum.emplace_back(num);
	}
	return true;
}

/*
* �����ȼ���С�ڵ���8�������ϻ����˾��룬���㵥λ��������
*/
bool SameLevelSelectMinRobot2(Robot& r, int num, priority_queue<TaskStack, vector<TaskStack>, TaskPool>& pq) {
	if (pq.empty()) return false;
	int currentLevel = 0;
	// ȷ������Ҫ���һ���������������ȼ�
	while (1) {
		currentLevel = pq.top()._level;
		r._tstack = pq.top();
		r._vTask = pq.top()._qTaskList;
		if (VertifyBenchGoingAvailable(r, num)) break;
		pq.pop();
		if (pq.empty()) return false;
	}
	// ����Ѵ�ŵ�λ��������
	priority_queue<pair<double, int>, vector<pair<double, int>>, less<pair<double, int>>> pqMinTime;
	int stCount = 0;
	vector<TaskStack> vSameLevel = vector<TaskStack>();
	while (!pq.empty()) {
		// ��ǰ����
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
	//fprintf(stderr, "��ǰ��������:%d,ͬ���ȼ�����:%d\n", pqMinTime.size(), vSameLevel.size());
	if (vSameLevel.empty())  return false;
	r._tstack = vSameLevel[pqMinTime.top().second];
	r._vTask = r._tstack._qTaskList;

	return true;
}

void Task_Pool() {
	needResetNum.clear();


	for (int i = 0; i < ROBOT_NUM; i++) {
		//fprintf(stderr, "task pool num:%d\n", qTaskPool.size());
		// �����������ȥͬһ��123ʱ�������еĻ�����û���õ�123����ʱɾ����ǰ����
		if (vRobotInfo[i].getGoodsId() == 0 && vRobotInfo[i]._vTask.size() > 0 && vRobotInfo[i]._vTask[0]._qCmd) {
			for (int j = 0; j < vRobotInfo[i]._vTask.size(); j++) {
				ResetBenchGoingState(vRobotInfo[i], i);
			}
			vRobotInfo[i]._vTask.clear();
			vRobotInfo[i]._tstack = TaskStack();
		}
		// �жϻ������Ƿ������������ֱ��ִ������
		if (VerityRobot2Goal(vRobotInfo[i], i)) continue;

		// û������Ļ�����ȡ���������ȡ����
		// ȡ����ͬһ���ȼ������񣬼��ϻ����˾�����������
		priority_queue<TaskStack, vector<TaskStack>, TaskPool> qTmp = qTaskPool;
		//bool ret = SameLevelSelectMinRobot(vRobotInfo[i], qTmp);
		bool ret = SameLevelSelectMinRobot2(vRobotInfo[i], i, qTmp);
		if (!ret) {
			// û�кϷ�����
			vRobotInfo[i]._tstack = TaskStack();
			vRobotInfo[i]._vTask = vector<Task>();
		}

		SetBenchGoingState(vRobotInfo[i], i);
	}

	// ����������״̬
	for (int i = 0; i < needResetNum.size(); i++) {
		// �����˾ͼ����·���һ������
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

