#include "oled_data.h"

// 温度符号
code unsigned char tempSymbol[16] = { 0x00,0x07,0x05,0x07,0x00,0x7C,0x82,0x82,0x82,0x44,0x00,0x00 };

// standard ascii 5x7 font    纵向取模
// defines ascii characters 0x20-0x7f (32-127)
unsigned char code Fonts[] = {
        0x00, 0x00, 0x00, 0x00, 0x00,// ' '
        0x00, 0x00, 0x5F, 0x00, 0x00,// !
        0x00, 0x07, 0x00, 0x07, 0x00,// "
        0x14, 0x7F, 0x14, 0x7F, 0x14,// #
        0x24, 0x2A, 0x07, 0x2A, 0x12,// $
        0x23, 0x13, 0x08, 0x64, 0x62,// %
        0x37, 0x49, 0x55, 0x22, 0x50,// &
        0x00, 0x05, 0x03, 0x00, 0x00,// '
        0x00, 0x1C, 0x22, 0x41, 0x00,// (
        0x00, 0x41, 0x22, 0x1C, 0x00,// )
        0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
        0x08, 0x08, 0x3E, 0x08, 0x08,// +
        0x00, 0x50, 0x30, 0x00, 0x00,// ,
        0x08, 0x08, 0x08, 0x08, 0x08,// -
        0x00, 0x60, 0x60, 0x00, 0x00,// .
        0x20, 0x10, 0x08, 0x04, 0x02,// /
        0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
        0x00, 0x42, 0x7F, 0x40, 0x00,// 1
        0x42, 0x61, 0x51, 0x49, 0x46,// 2
        0x21, 0x41, 0x45, 0x4B, 0x31,// 3
        0x18, 0x14, 0x12, 0x7F, 0x10,// 4
        0x27, 0x45, 0x45, 0x45, 0x39,// 5
        0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
        0x01, 0x71, 0x09, 0x05, 0x03,// 7
        0x36, 0x49, 0x49, 0x49, 0x36,// 8
        0x06, 0x49, 0x49, 0x29, 0x1E,// 9
        0x00, 0x36, 0x36, 0x00, 0x00,// :
        0x00, 0x56, 0x36, 0x00, 0x00,// ;
        0x00, 0x08, 0x14, 0x22, 0x41,// <
        0x14, 0x14, 0x14, 0x14, 0x14,// =
        0x41, 0x22, 0x14, 0x08, 0x00,// >
        0x02, 0x01, 0x51, 0x09, 0x06,// ?
        0x32, 0x49, 0x79, 0x41, 0x3E,// @
        0x7E, 0x11, 0x11, 0x11, 0x7E,// A
        0x7F, 0x49, 0x49, 0x49, 0x36,// B
        0x3E, 0x41, 0x41, 0x41, 0x22,// C
        0x7F, 0x41, 0x41, 0x22, 0x1C,// D
        0x7F, 0x49, 0x49, 0x49, 0x41,// E
        0x7F, 0x09, 0x09, 0x01, 0x01,// F
        0x3E, 0x41, 0x41, 0x51, 0x32,// G
        0x7F, 0x08, 0x08, 0x08, 0x7F,// H
        0x00, 0x41, 0x7F, 0x41, 0x00,// I
        0x20, 0x40, 0x41, 0x3F, 0x01,// J
        0x7F, 0x08, 0x14, 0x22, 0x41,// K
        0x7F, 0x40, 0x40, 0x40, 0x40,// L
        0x7F, 0x02, 0x04, 0x02, 0x7F,// M
        0x7F, 0x04, 0x08, 0x10, 0x7F,// N
        0x3E, 0x41, 0x41, 0x41, 0x3E,// O
        0x7F, 0x09, 0x09, 0x09, 0x06,// P
        0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
        0x7F, 0x09, 0x19, 0x29, 0x46,// R
        0x46, 0x49, 0x49, 0x49, 0x31,// S
        0x01, 0x01, 0x7F, 0x01, 0x01,// T
        0x3F, 0x40, 0x40, 0x40, 0x3F,// U
        0x1F, 0x20, 0x40, 0x20, 0x1F,// V
        0x7F, 0x20, 0x18, 0x20, 0x7F,// W
        0x63, 0x14, 0x08, 0x14, 0x63,// X
        0x03, 0x04, 0x78, 0x04, 0x03,// Y
        0x61, 0x51, 0x49, 0x45, 0x43,// Z
        0x00, 0x00, 0x7F, 0x41, 0x41,// [
        0x02, 0x04, 0x08, 0x10, 0x20,// "\"
        0x41, 0x41, 0x7F, 0x00, 0x00,// ]
        0x04, 0x02, 0x01, 0x02, 0x04,// ^
        0x40, 0x40, 0x40, 0x40, 0x40,// _
        0x00, 0x01, 0x02, 0x04, 0x00,// `
        0x20, 0x54, 0x54, 0x54, 0x78,// a
        0x7F, 0x48, 0x44, 0x44, 0x38,// b
        0x38, 0x44, 0x44, 0x44, 0x20,// c
        0x38, 0x44, 0x44, 0x48, 0x7F,// d
        0x38, 0x54, 0x54, 0x54, 0x18,// e
        0x08, 0x7E, 0x09, 0x01, 0x02,// f
        0x08, 0x14, 0x54, 0x54, 0x3C,// g
        0x7F, 0x08, 0x04, 0x04, 0x78,// h
        0x00, 0x44, 0x7D, 0x40, 0x00,// i
        0x20, 0x40, 0x44, 0x3D, 0x00,// j
        0x00, 0x7F, 0x10, 0x28, 0x44,// k
        0x00, 0x41, 0x7F, 0x40, 0x00,// l
        0x7C, 0x04, 0x18, 0x04, 0x78,// m
        0x7C, 0x08, 0x04, 0x04, 0x78,// n
        0x38, 0x44, 0x44, 0x44, 0x38,// o
        0x7C, 0x14, 0x14, 0x14, 0x08,// p
        0x08, 0x14, 0x14, 0x18, 0x7C,// q
        0x7C, 0x08, 0x04, 0x04, 0x08,// r
        0x48, 0x54, 0x54, 0x54, 0x20,// s
        0x04, 0x3F, 0x44, 0x40, 0x20,// t
        0x3C, 0x40, 0x40, 0x20, 0x7C,// u
        0x1C, 0x20, 0x40, 0x20, 0x1C,// v
        0x3C, 0x40, 0x30, 0x40, 0x3C,// w
        0x44, 0x28, 0x10, 0x28, 0x44,// x
        0x0C, 0x50, 0x50, 0x50, 0x3C,// y
        0x44, 0x64, 0x54, 0x4C, 0x44,// z
        0x00, 0x08, 0x36, 0x41, 0x00,// {
        0x00, 0x00, 0x7F, 0x00, 0x00,// |
        0x00, 0x41, 0x36, 0x08, 0x00,// }
        0x02, 0x01, 0x02, 0x04, 0x02,// ~
        0xff, 0xff, 0xff, 0xff, 0xff, //black block
};



code unsigned char Num[16][16] = {	// 横向，逆序， 高位在下，低位在上，从左往右， 16(纵)* 8(横)
	{0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00},/*"0",0*/
	{0x00,0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00},/*"1",1*/
	{0x00,0x70,0x08,0x08,0x08,0x08,0xF0,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00},/*"2",2*/	
	{0x00,0x30,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x18,0x20,0x21,0x21,0x22,0x1C,0x00},/*"3",3*/	
	{0x00,0x00,0x80,0x40,0x30,0xF8,0x00,0x00,0x00,0x06,0x05,0x24,0x24,0x3F,0x24,0x24},/*"4",4*/	
	{0x00,0xF8,0x88,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x20,0x20,0x20,0x11,0x0E,0x00},/*"5",5*/	
	{0x00,0xE0,0x10,0x88,0x88,0x90,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x20,0x1F,0x00},/*"6",6*/	
	{0x00,0x18,0x08,0x08,0x88,0x68,0x18,0x00,0x00,0x00,0x00,0x3E,0x01,0x00,0x00,0x00},/*"7",7*/	
	{0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00},/*"8",8*/	
	{0x00,0xF0,0x08,0x08,0x08,0x10,0xE0,0x00,0x00,0x01,0x12,0x22,0x22,0x11,0x0F,0x00},/*"9",9*/	
	{0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},/*"A",10*/	
	{0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00},/*"B",11*/	
	{0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00},/*"C",12*/	
	{0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00},/*"D",13*/	
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},/*"E",14*/	
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00},/*"F",15*/
};




code unsigned char LETTER[52][16] = {
	{0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x19,0x24,0x24,0x12,0x3F,0x20,0x00},/*"a",0*/	
	{0x10,0xF0,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00},/*"b",1*/	
	{0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00},/*"c",2*/	
	{0x00,0x00,0x80,0x80,0x80,0x90,0xF0,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},/*"d",3*/	
	{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x24,0x24,0x24,0x24,0x17,0x00},/*"e",4*/	
	{0x00,0x80,0x80,0xE0,0x90,0x90,0x20,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/*"f",5*/	
	{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00},/*"g",6*/	
	{0x10,0xF0,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},/*"h",7*/	
	{0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/*"i",8*/	
	{0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00},/*"j",9*/	
	{0x10,0xF0,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x06,0x29,0x30,0x20,0x00},/*"k",10*/	
	{0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/*"l",11*/	
	{0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F},/*"m",12*/	
	{0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20},/*"n",13*/	
	{0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},/*"o",14*/	
	{0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0x91,0x20,0x20,0x11,0x0E,0x00},/*"p",15*/	
	{0x00,0x00,0x00,0x80,0x80,0x00,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0x91,0xFF,0x80},/*"q",16*/	
	{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00},/*"r",17*/	
	{0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00},/*"s",18*/	
	{0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x10,0x00},/*"t",19*/	
	{0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20},/*"u",20*/	
	{0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x03,0x0C,0x30,0x0C,0x03,0x00,0x00},/*"v",21*/	
	{0x80,0x80,0x00,0x80,0x80,0x00,0x80,0x80,0x01,0x0E,0x30,0x0C,0x07,0x38,0x06,0x01},/*"w",22*/	
	{0x00,0x80,0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x20,0x31,0x0E,0x2E,0x31,0x20,0x00},/*"x",23*/	
	{0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x81,0x86,0x78,0x18,0x06,0x01,0x00},/*"y",24*/	
	{0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00},/*"z",25*/	
	{0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20},/*"A",26*/	
	{0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00},/*"B",27*/	
	{0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00},/*"C",28*/	
	{0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00},/*"D",29*/	
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00},/*"E",30*/	
	{0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00},/*"F",31*/	
	{0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00},/*"G",32*/	
	{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20},/*"H",33*/	
	{0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00},/*"I",34*/	
	{0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00},/*"J",35*/	
	{0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00},/*"K",36*/	
	{0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00},/*"L",37*/	
	{0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x01,0x3E,0x01,0x3F,0x20,0x00},/*"M",38*/	
	{0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00},/*"N",39*/	
	{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00},/*"O",40*/	
	{0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00},/*"P",41*/	
	{0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x28,0x28,0x30,0x50,0x4F,0x00},/*"Q",42*/	
	{0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20},/*"R",43*/	
	{0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00},/*"S",44*/	
	{0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},/*"T",45*/	
	{0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00},/*"U",46*/	
	{0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00},/*"V",47*/	
	{0x08,0xF8,0x00,0xF8,0x00,0xF8,0x08,0x00,0x00,0x03,0x3E,0x01,0x3E,0x03,0x00,0x00},/*"W",48*/	
	{0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20},/*"X",49*/	
	{0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00},/*"Y",50*/	
	{0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00},/*"Z",51*/	
};

code unsigned char HM[][16] = {
	
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xC0,0x40,0x00,0x00,0x80,0x00,0x00,0x00,0x00},
	{0x00,0x00,0xE0,0x40,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0xE0,0xC0,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0},
	{0xC0,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x41,0x49,0x49,0x49,0x49,0x49,0xFF,0xFF,0x49,0x49,0x4D,0x69,0x41,0x01,0x49,0x49},
	{0x49,0x49,0xFF,0x49,0x49,0x49,0x49,0x4D,0x49,0x61,0x40,0x00,0x00,0x00,0x00,0x00},
	{0xC0,0xFF,0x2F,0x22,0x20,0xFF,0xFF,0x20,0x20,0x30,0x30,0x20,0x00,0xC0,0x7C,0xDF},
	{0x11,0x10,0x10,0x10,0x10,0xF0,0xF0,0x10,0x18,0x18,0x10,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x04,0x24,0x24,0x25,0x25,0x24,0x24,0x24,0x24,0x24,0x24,0x24,0x24},
	{0x24,0x24,0x25,0x24,0x24,0xFC,0xFC,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x88,0x86},
	{0x83,0xC0,0xC0,0x40,0x60,0xFF,0xFF,0x10,0x10,0x08,0x08,0x14,0x0E,0x01,0x00,0x07},
	{0x78,0xC0,0x00,0xC0,0xFC,0x1F,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x80,0xC0,0x70,0x09,0x01,0x01,0x01,0xFD,0xF9,0x01,0x01,0x03,0x0D,0x39,0x11,0x01},
	{0x01,0x01,0xC1,0xF1,0x81,0x03,0x0B,0x10,0x70,0xE0,0x00,0x00,0x00,0x00,0x00,0x01},
	{0x01,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x80,0x80,0x40,0x60,0x30},
	{0x18,0x0D,0x07,0x0F,0x18,0x30,0x60,0xE0,0xC0,0x80,0x80,0x80,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01},
	{0x01,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x03,0x01,0x00,0x02,0x01,0x01,0x01,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"F:\生命标签\界面\慧牧.bmp",0*/
	

};


code unsigned char stateBase[48] = {

	0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x1C,0xF0,0x1C,0xFE,0x00,0xFE,0x92,0x92,0x6C,
	0x00,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x44,0x28,0x10,0x28,0x44,0x00,0x10,0x00,
	0x10,0x00,0x10,0x00,0x00,0x28,0x10,0xFE,0x54,0x28,0x00,0x00,0x00,0x00,0x00,0x00,

};

void OledFlushBaseState()
{
	OledFlushPage(0, 0, (unsigned char *)stateBase, 48);	
}

code unsigned char stateBtConnect[2][6] = {
	{0x44,0x28,0x10,0x28,0x44,0x00},	// 0 : NO
	{0x08,0x10,0x20,0x10,0x08,0x04},	// 1 : YES

};

/*
	功能 : 显示蓝牙的链接状态
	输入 : 0 : 未连接 ; 1 : 已连接
*/
void OledBtConnectState(unsigned char state)
{
	if(state > 1)
		state = 1;
	OledFlushPage(0, 24, (unsigned char *)stateBtConnect[state], 6);	
}




code unsigned char stateBattery[7][18] = {
	{0x00,0x00,0xFE,0xFE,0xFE,0xFE, 0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x38,0x00},	// 100%
	{0x00,0x00,0xFE,0xFE,0xFE,0xFE, 0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0x82,0x82,0xFE,0xFE,0x38,0x00},	// 80%
	{0x00,0x00,0xFE,0xFE,0xFE,0xFE, 0xFE,0xFE,0xFE,0xFE,0x82,0x82,0x82,0x82,0xFE,0xFE,0x38,0x00},	// 60%
	{0x00,0x00,0xFE,0xFE,0xFE,0xFE, 0xFE,0xFE,0x82,0x82,0x82,0x82,0x82,0x82,0xFE,0xFE,0x38,0x00},	// 40%
	{0x00,0x00,0xFE,0xFE,0xFE,0xFE, 0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xFE,0xFE,0x38,0x00},	// 20%	
	{0x00,0x00,0xFE,0xFE,0x82,0x82, 0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0xFE,0xFE,0x38,0x00},	// 0%
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}		// 消失
};


/*
	功能 : 显示电量状态
	输入 : 0 : 100% 
			1 : 80%
			2 : 60%
			3 : 40%
			4 : 20%
			5 : 0%
			6 : 消失
*/
void OledBatteryState(unsigned char batteryState)
{
	if(batteryState > 6)
		batteryState = 6;
	OledFlushPage(0, 66, (unsigned char *)stateBattery[batteryState], 18);	
}

/*
	功能 : 显示电量数值
	输入 : 电量
*/
char OledBatteryPower(unsigned char batteryPower)
{
	unsigned char ten = batteryPower / 10;
	unsigned char bits = batteryPower % 10;

	if(batteryPower > 100)
		return 1;
	OledAssicForState(0, 48, '0' + ten);
	OledAssicForState(0, 54, '0' + bits);
	OledAssicForState(0, 60, '%');
	return 0;
}



code unsigned char stateCharging[2][6] ={
	{0x00, 0x00,0x00,0x00,0x00,0x00,},
	{0x30,0x18,0xFE,0x30,0x18,0x00,}
	
};

/*
	功能 : 显示充电状态
	输入 : 0 : 未充电; 1 : 充电
*/
void OledChargingState(unsigned char state)
{
	if(state > 1)
		state = 1;
	OledFlushPage(0, 84, (unsigned char *)stateCharging[state], 6);	
}





