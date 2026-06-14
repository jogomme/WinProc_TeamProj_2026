#pragma once
#include"GameObject.h"
#include"Rock.h"
#include"Player.h"

class Feed : public Rock
{
public :
	Feed();
	~Feed();
	// 복사 생성자와 대입 연산자 삭제
	Feed(const Feed& other) = delete;
	Feed& operator=(const Feed& other) = delete;

	void Drop(const Rock& r);

	void SetLength(Player& p);

	void SetActive(bool b);

	void Move(double x, double y) override;
	void Drop(double x, double y, double price, int rockType);
private:

};