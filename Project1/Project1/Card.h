#pragma once
class Card {
public:
	int value; // 牌值
	int exist; // 存在标志
	int belong; // 属于哪位玩家
	Card(){
		this->exist = 1;
	}
	void setCard(int value, int belong) {
		this->value = value;
		this->belong = belong;
	}
	~Card(){}
};