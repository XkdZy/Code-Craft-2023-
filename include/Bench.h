#ifndef _BENCH_H
#define _BENCH_H

#include <iostream>
using namespace std;
#include <string>

struct RPoint {
	double x;
	double y;
};

// ����̨��ԭ����

class Bench {
private:
	int _benchNum;
	RPoint _pos; // ����̨����
	int _benchType; // ����̨����0~8
	int _productTime; // ʣ������ʱ��-1��ʾû������
	int _rawState; // ԭ���ϸ�״̬110000ӵ��4��5��Ʒ
	int _produceState; // ������״̬
public:
	Bench();
	Bench(int benchNum, int type, double x, double y);
	Bench(int benchNum, int type, double x, double y, int productTime, int rawState, int produceState);

	//int _rawState; // ԭ���ϸ�״̬110000ӵ��4��5��Ʒ
	vector<pair<int, int>> _robot_buyOrSell; // first�������˱�ţ�-1���ޣ���ţ�0123����second�����������Ļ����Ϊ-1
	vector<int> _sell2WhichBench; // ���345����̨Ӧ�����6�Ź���̨

	// ����̨����״̬��λ
	int _rawStateGoing = 0; // ��ǰ����̨ԭ���ϵĴ���״̬���л��������ڴ����״̬�� 
	int _productStateGoing = 0; // ��ǰ����̨������Ĵ���״̬���л��������ڴ����״̬�� 
	//int _rawStateGoingLast = 0;
	int _buyGoing = 0; // ��ǰ����̨��������Ʒ�Ѿ������ˣ��л��������ڴ���
	vector<pair<int, int>> _vRawGoingLast; // pair first ������id second ԭ��������

	int getBenchNum();
	void setBenchNum(int benchNum);
	RPoint getPos();
	void setPos(RPoint p);
	int getBenchType();
	void setBenchType(int benchType);
	int getProductTime();
	void setProductTime(int productTime);
	int getRawState();
	void setRawState(int rawState);
	int getProduceState();
	void setProduceState(int produceState);

	~Bench();
};

#endif
