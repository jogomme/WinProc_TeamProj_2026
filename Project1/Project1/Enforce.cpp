#include "Enforce.h"
#include <iostream>

Enforce::Enforce()
{
	x, y = 0, 0;
	type = 0;
	amount = 0;
	price = 0;

	draw = 0;
	drawing[3];
	open = 0;
}
Enforce::~Enforce()
{

}

// Getter 함수
int Enforce::Get_Enforce_Point_x()
{
	return x;
}
int Enforce::Get_Enforce_Point_y()
{
	return y;
}
int Enforce::Get_Enforce_Type()
{
	return type;
}
int Enforce::Get_Enforce_Amount()
{
	return amount;
}
int Enforce::Get_Enforce_Price()
{
	return price;
}
int Enforce::Get_Enforce_Draw()
{
	return draw;
}
int Enforce::Get_Enforce_Drawing(int a)
{
	return drawing[a];
}
int Enforce::Get_Enforce_Open()
{
	return open;
}

void Enforce::Set_Pos(double a, double b)
{
	x = a;
	y = b;
}
void Enforce::Set_TAP(int t, int a, int p)
{
	type = t;
	amount = a;
	price = p;
}
void Enforce::Set_Draw(int a)
{
	draw = a;
}
void Enforce::Set_Drawing(int a, int b, int c)
{
	drawing[0] = a;
	drawing[1] = b;
	drawing[2] = c;
}