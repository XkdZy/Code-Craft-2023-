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
#define DISTANCE_2_FRAME (double)(8.33333 * 1.1) // ������ȫ��ʱ�����֡����ת����ϵ

//#define DISTANCE_2_FRAME (double)8.5
struct Task {
	int _qBench; // ��ǰ������̨���
	int _qCmd; // ��ǰ����0����1����
};
enum TaskLevel {
	LEVEL1 = 0, // 1�����ʲô��û��
	LEVEL2_LOWEST = 1, // 123����9
	LEVEL2 = 3, // 2��14 24ʲô��û��
	LEVEL3 = 7, // 3��456ȱһ��ԭ����
	LEVEL3_HEIGTH = 8, // 3��456ȱһ��ԭ����
	LEVEL3_LOWEST = 9, // 456����9
	LEVEL4 = 11, // 7ȱһ��ԭ���Ͻ�14��15��26���ȼ����
	LEVEL5 = 15, // 5��7ȱ����ԭ���ϣ�ִ����������񻹲�����ԭ���ϣ�
	LEVEL6_1 = 18, // 6��7ȱ����ԭ���ϣ���һ��Going״̬ԭ����
	LEVEL6 = 19, // 6��7ȱ����ԭ���ϣ�ִ����������񻹲�һ��ԭ���ϣ�
	LEVEL7_2 = 21, // 7��7ȱһ��ԭ���ϣ�������Going״̬ԭ����
	LEVEL7_1 = 22, // 7��7ȱһ��ԭ���ϣ���һ��Going״̬ԭ����
	LEVEL7 = 23, // 7��7ȱһ��ԭ����
	LEVEL8 = 888, // 8��������Ż�ʹ��
	LEVEL9 = 999, // 9��7�������ˣ����ҿ�����456

	LEVEL_SECOND_1 = 1, // ��123��9
	LEVEL_SECOND_2 = 2, // ��123��456
	LEVEL_SECOND_3 = 3, // ��456��79
	LEVEL_SECOND_4 = 4, // ��7��8

	LEVEL_THIRD_0 = 0, // ����̫��>=2
	LEVEL_THIRD_1 = 1, // ��123��
	LEVEL_THIRD_2 = 2, // ��456��
	LEVEL_THIRD_3 = 3, // ��7��
	//LEVEL1 = 0, // 1�����ʲô��û��
	//LEVEL2_LOWEST = 1, // 123����9
	//LEVEL2 = 3, // 2��14 24ʲô��û��
	//LEVEL3 = 7, // 3��456ȱһ��ԭ����
	//LEVEL3_LOWEST = 8, // 456����9
	////LEVEL4 = 11, // 4��7��������456��������
	//LEVEL5 = 15, // 5��7ȱ����ԭ���ϣ�ִ����������񻹲�����ԭ���ϣ�
	//LEVEL6 = 19, // 6��7ȱ����ԭ���ϣ�ִ����������񻹲�һ��ԭ���ϣ�
	//LEVEL7 = 23, // 7��7ȱһ��ԭ����
	//LEVEL8 = 24, // 8��7ȱһ��ԭ���Ͻ�14��15��26���ȼ����
	//LEVEL9 = 27, // 9��7�������ˣ����ҿ�����456
};

class TaskStack {
public:
	int _level = TaskLevel::LEVEL1; // ��ǰ����Ĳ㼶7��456��֮
	int _income = 0; // ��ǰ��������
	double _distance = 0.; // ��ǰ�����߹��ľ���
	vector<Task> _qTaskList; // ��ǰ�����б����մӸߵ�������
};

class TaskPool {
public:
	bool operator()(TaskStack tq1, TaskStack tq2);
};

extern priority_queue<TaskStack, vector<TaskStack>, TaskPool> qTaskPool;

int GetBenchRawMaterialNum(int benchNum, int state);
//void CreateMyTaskQueue(int benchNum, TaskStack& ts, bool action, int sellType);
void CreateMyTaskQueue(int benchNum, TaskStack ts, bool action, int sellType);

