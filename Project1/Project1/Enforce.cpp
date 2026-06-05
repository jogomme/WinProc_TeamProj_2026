#include "Enforce.h"
#include "GameObject.h"

#include <iostream>
#include <Windows.h>


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
void Enforce::Move_Mid_Pos(Point rectViewMid, int intrv)
{
	x = rectViewMid.x + x * intrv;
	y = rectViewMid.y + y * intrv;
}
void Enforce::Set_TAP(int t, int a, int p)
{
	type = t;
	amount = a;
	price = p;
}
void Enforce::Set_Draw(int a)
{
	if (a >= 0) {
		draw = a;
	}
}
void Enforce::Set_Drawing(int a, int b, int c)
{
	drawing[0] = a;
	drawing[1] = b;
	drawing[2] = c;
}
void Enforce::Set_Open(int a)
{
	open = a;
}

void Enforce::Start_Setting(Enforce& ef, int cnt)
{
	//TAP / t-type, a-amount, p-price
	//type 1-공격, 2-이동속도, 3-연료, 4-공격속도, 5-공격타입 해금
	//amount.공격타입 1-공속업, 2-다중발사, 3-총알 크기 증가
	if (cnt == 0) {
		ef.Set_Pos(0, 0);
		ef.Set_TAP(1, 1, 1);
		ef.Set_Draw(1);
		ef.Set_Drawing(1, 2, 3);
	}
	else if (cnt == 1) {
		ef.Set_Pos(1, 0);
		ef.Set_TAP(2, 1, 1);
		ef.Set_Drawing(-1, -1, -1);
	}
	else if (cnt == 2) {
		ef.Set_Pos(-1, 0);
		ef.Set_TAP(3, 1, 1);
		ef.Set_Drawing(-1, -1, -1);
	}
}