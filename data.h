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

#define MAPWIDTH (int)100 // ��ͼ��pixel
#define MAPHEIGHT (int)100 // ��ͼ��pixel
#define MAX_BENCH_SIZE (int)50 // �����̨����
#define ROBOT_NUM (int)4 // ����������
#define ROBOT_MAX_FORWARDSPEED (double)6 // ���ǰ��6m/s
#define ROBOT_MAX_RETREATSPEED (double)2. // ������2m/s
#define SYS_MAX_FRAMENUM (int)9000 // ���֡��
#define SYS_SECOND_FRAMENUM (int)8500 // ���֡��
#define SYS_THIRD_FRAMENUM (int)8000 // ���֡��
#define ROBOT_MAX_ROTATESPEED RPI // �����ٶ�
#define ROBOT_RADUIS_WITHOUT (double)0.45 // �����˿��ذ뾶
#define ROBOT_RADUIS_WITH (double)0.53 // ����������뾶

RPoint Pos2Actual(RPoint); // ���굽ʵ�ʳߴ��ת��
#define Pos2Actual(p) RPoint({0.5 * (p.x + 0.5), 0.5 * (99.5 - p.y)}) 

double DistRPoint2RPoint(RPoint, RPoint); // ����㵽�����
#define DistRPoint2RPoint(p1, p2) (double)sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)) 

double Point2Rad2(RPoint, RPoint); // �������0radΪ��ת��
#define Point2Rad2(p1, p2) atan2(p2.y - p1.y, p2.x - p1.x)

extern unordered_map<int, vector<int>> mBench2SubBench; // ��ǰ����̨����һ������̨��ӳ���ϵ
extern unordered_map<int, int> mMaxProfit; // �����Ӧ�������
extern unordered_map<int, vector<int>> mRawMaterial; // ����̨���Ͷ�Ӧԭ����
extern unordered_map<int, vector<int>> mBenchSort; // ÿ�ֹ���̨�����б��

extern int currentFrameId; // ��ǰ����֡�����������ʼ0���ԣ�
extern int actualBenchNum; // ��Ч����̨����
extern vector<Bench> vBenchInfo; // ���й���̨
extern vector<Robot> vRobotInfo; // ���л�������Ϣ

// task pool
extern unordered_map<int, int> mBuyBenchGoods; // �����Ӧ�������
extern unordered_map<int, int> mSellBenchGoods; // �����Ӧ�������
//// 7ȱʧ456��������ԭ���ϣ�0����ȱ456���ڼӹ���1
extern set<int> sSevenLackRaw; // 7���͹���̨ȱ��ԭ��������
extern unordered_map<int, set<int>> mSevenRawState; // 7���͹���̨�е�ԭ��������
extern vector<int> needResetNum; // ����Ŀ�����Ҫ���λ�Ļ�����

// �����ļ�·��
extern double param1;
extern double param2;
extern int profitMoney;
extern string savePath;
extern string mapNum;

void InitMapInfo(const vector<string>& vs);
void UpDataInfo(const vector<string>& vs);
void SysInit(); // ϵͳ��ʼ������ȡ��ͼ��Ϣ
void PrintRobotInfo();
void PrintBenchInfo();
void PrintTaskPoolInfo();

#define SYS_DEBUG 1

#endif

