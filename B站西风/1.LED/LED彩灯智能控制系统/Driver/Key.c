#include <KEY.H>

//unsigned char Key_Read() {
//	unsigned char tmp = 0;
//	P44 = 0, P42 = 1, P35 = 1, P34 = 1;
//	if (P33 == 0) tmp = 4;
//	if (P32 == 0) tmp = 5;
//	if (P31 == 0) tmp = 6;
//	if (P30 == 0) tmp = 7;
//	P44 = 1, P42 = 0, P35 = 1, P34 = 1;
//	if (P33 == 0) tmp = 8;
//	if (P32 == 0) tmp = 9;
//	if (P31 == 0) tmp = 10;
//	if (P30 == 0) tmp = 11;
//	P44 = 1, P42 = 1, P35 = 0, P34 = 1;
//	if (P33 == 0) tmp = 12;
//	if (P32 == 0) tmp = 13;
//	if (P31 == 0) tmp = 14;
//	if (P30 == 0) tmp = 15;
//	P44 = 1, P42 = 1, P35 = 1, P34 = 0;
//	if (P33 == 0) tmp = 16;
//	if (P32 == 0) tmp = 17;
//	if (P31 == 0) tmp = 18;
//	if (P30 == 0) tmp = 19;
//	return tmp;
//}

unsigned char Key_Read() {
	unsigned char tmp = 0;
	P44 = 0, P42 = 1, P35 = 1, P34 = 1;
	if (P33 == 0) tmp = 4;
	if (P32 == 0) tmp = 5;
	if (P31 == 0) tmp = 6;
	if (P30 == 0) tmp = 7;
	return tmp;
}