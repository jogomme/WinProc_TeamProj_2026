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
void Enforce::Set_TAP(int t, double a, int p)
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
	//type 1-공격, 2-이동속도, 3-연료, 4-공격속도
	if (cnt == 0) {
		ef.Set_Pos(0, 0);
		ef.Set_TAP(3, 2, 1);
		ef.Set_Draw(1);
		ef.Set_Drawing(1, 7, 15);
	}
	else if (cnt == 1) {
		ef.Set_Pos(1, 0);
		ef.Set_TAP(3, 3, 1);
		ef.Set_Drawing(2, -1, -1);
	}
	else if (cnt == 2) {
		ef.Set_Pos(2, 0);
		ef.Set_TAP(3, 3, 1);
		ef.Set_Drawing(3, -1, -1);
	}
	else if (cnt == 3) {
		ef.Set_Pos(3, 0);
		ef.Set_TAP(3, 4, 1);
		ef.Set_Drawing(4, 5, -1);
	}
	else if (cnt == 4) {
		ef.Set_Pos(4, 0);
		ef.Set_TAP(3, 5, 1);
		ef.Set_Drawing(6, -1, -1);
	}
	else if (cnt == 5) {
		ef.Set_Pos(3, 1);
		ef.Set_TAP(0, 0, 1);
		ef.Set_Drawing(-1, -1, -1);
	}
	else if (cnt == 6) {
		ef.Set_Pos(4, -1);
		ef.Set_TAP(0, 0, 1);
		ef.Set_Drawing(-1, -1, -1);
	}
	else if (cnt == 7) {
		ef.Set_Pos(0, 1);
		ef.Set_TAP(4, 20, 1);
		ef.Set_Drawing(8, -1, -1);
	}
	else if (cnt == 8) {
		ef.Set_Pos(0, 2);
		ef.Set_TAP(4, 25, 1);
		ef.Set_Drawing(9, -1, -1);
	}
	else if (cnt == 9) {
		ef.Set_Pos(0, 3);
		ef.Set_TAP(4, 35, 1);
		ef.Set_Drawing(10, 11, 13);
	}
	else if (cnt == 10) {
		ef.Set_Pos(0, 4);
		ef.Set_TAP(4, 20, 1);
		ef.Set_Drawing(-1, -1, -1);
	}
	else if (cnt == 11) {
		ef.Set_Pos(1, 3);
		ef.Set_TAP(0, 0, 1);
		ef.Set_Drawing(12, -1, -1);
	}
	else if (cnt == 12) {
		ef.Set_Pos(2, 3);
		ef.Set_TAP(0, 0, 1);
		ef.Set_Drawing(-1, -1, -1);
	}
	else if (cnt == 13) {
		ef.Set_Pos(-1, 3);
		ef.Set_TAP(2, 1, 1);
		ef.Set_Drawing(14, -1, -1);
	}
	else if (cnt == 14) {
		ef.Set_Pos(-2, 3);
		ef.Set_TAP(2, 2, 1);
		ef.Set_Drawing(-1, -1, -1);
	}
	else if (cnt == 15) {
		ef.Set_Pos(-1, 0);
		ef.Set_TAP(1, 1, 1);
		ef.Set_Drawing(16, -1, -1);
	}
	else if (cnt == 16) {
		ef.Set_Pos(-2, 0);
		ef.Set_TAP(1, 1, 1);
		ef.Set_Drawing(-1, -1, -1);
	}
}