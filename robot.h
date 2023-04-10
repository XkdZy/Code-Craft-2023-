#ifndef _ROBOT_H
#define _ROBOT_H

#include <iostream>
using namespace std;
#include "Bench.h"
#include "Task.h"

//struct RPID {
//	float SetSpeed;//�����趨ֵ
//	float ActualSpeed;//����ʵ��ֵ
//	float err;//����ƫ��ֵ
//	float err_last;//������һ��ƫ��ֵ
//	float Kp, Ki, Kd;//������������֡�΢��ϵ��
//	float voltage;//�����ѹֵ(����ִ�����ı���)
//	float integral;//�������ֵ
//};
//void PID_init(RPID& pid);
//double PID_realize(RPID& pid, double speed);

class Robot {
private:
	int _id; // ��������������̨id��-1��ʾ��
	RPoint _pos; // �����˵�ǰλ��
	double _rad; // ����rad	0�����ң���/2������
	int _goodsId; // 0����ʾû��1~7��ʾ��Ʒ1~7
	double _timeCoff; // ʱ��ϵ��,��Ю������Ϊ0
	double _collisionCoff; // ��ײϵ��,��Ю������Ϊ0
	double _angleVel; // ���ٶ�
	double _lineVelx; // ���ٶ�
	double _lineVely; // ���ٶ�
public:
	Robot();
	Robot(int id, double x, double y, double rad, int goodsId, double timeCoff, double collisionCoff, double angleVel, double lineVelx, double lineVely);

	int _crashState = 0;
	int _2bench = -1; // ����ָ������̨ȥ-1��û������
	int _sell2WhichBench = -1; // ���������������Ǹ�����̨��
	int _priorityTask = -1;
	TaskStack _tstack = TaskStack();
	vector<Task> _vTask = vector<Task>();
	int _radCnt = 0;
	double _lastRad = 0.;
	int _robotNum;

	void setId(int id);
	int getId();

	void setPos(RPoint p);
	RPoint getPos();

	void setGoodsId(int goodsId);
	int getGoodsId();

	double getRad();
	void setRad(double rad);

	double getTimeCoff();
	void setTimeCoff(double timeCoff);

	double getCollisionCoff();
	void setCollisionCoff(double collisionCoff);

	double getAngleVel();
	void setAngleVel(double angleVel);

	double getLineVelx();
	void setLineVelx(double lineVelx);

	double getLineVely();
	void setLineVely(double lineVely);


	~Robot();
};

#endif // !_ROBOT_H

