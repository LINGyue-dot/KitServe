#pragma once
class Card {
public:
	int value; // ��ֵ
	int exist; // ���ڱ�־
	int belong; // ������λ���
	Card(){
		this->exist = 1;
	}
	void setCard(int value, int belong) {
		this->value = value;
		this->belong = belong;
	}
	~Card(){}
};