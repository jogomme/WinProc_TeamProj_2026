#pragma once
#include"GameObject.h"
#include"Rock.h"

class Feed : public Rock
{
public :
	void drop(const Rock& r);
private:
	double m_price;
};