﻿#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<stdio.h>
#include<bitset>
using namespace std;
uint16_t RC[10] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36 };//用于产生轮密钥
uint16_t Sbox[16][16] = { {0x63 ,0x7c ,0x77 ,0x7b ,0xf2 ,0x6b ,0x6f ,0xc5 ,0x30 ,0x01 ,0x67 ,0x2b ,0xfe ,0xd7 ,0xab ,0x76},
						  {0xca ,0x82 ,0xc9 ,0x7d ,0xfa ,0x59 ,0x47 ,0xf0 ,0xad ,0xd4 ,0xa2 ,0xaf ,0x9c ,0xa4 ,0x72 ,0xc0},
						  {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
						  {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
						  {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
						  {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
						  {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
						  {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
						  {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
						  {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
						  {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
						  {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
						  {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
						  {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
						  {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
						  {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16} };//s盒
uint16_t okey[4][4] = { {0x32, 0x30, 0x32, 0x31},
						{0x30, 0x30, 0x34, 0x36},
						{0x30, 0x30, 0x31, 0x38},
						{0x00, 0x00, 0x00, 0x00} };//原始密钥，学号的ASCII后加0x00填充
uint16_t oans[4][4] = { {0x32, 0x30, 0x32, 0x31},
						{0x30, 0x30, 0x34, 0x36},
						{0x30, 0x30, 0x31, 0x38},
						{0x00, 0x00, 0x00, 0x00} };//明文，学号
uint16_t MixC[256][4] = //将s盒与列混合统一到一起的查表方法
{ {0xc6,0x63,0x63,0xa5},
{0xf8,0x7c,0x7c,0x84},
{0xee,0x77,0x77,0x99},
{0xf6,0x7b,0x7b,0x8d},
{0xff,0xf2,0xf2,0x0d},
{0xd6,0x6b,0x6b,0xbd},
{0xde,0x6f,0x6f,0xb1},
{0x91,0xc5,0xc5,0x54},
{0x60,0x30,0x30,0x50},
{0x02,0x01,0x01,0x03},
{0xce,0x67,0x67,0xa9},
{0x56,0x2b,0x2b,0x7d},
{0xe7,0xfe,0xfe,0x19},
{0xb5,0xd7,0xd7,0x62},
{0x4d,0xab,0xab,0xe6},
{0xec,0x76,0x76,0x9a},
{0x8f,0xca,0xca,0x45},
{0x1f,0x82,0x82,0x9d},
{0x89,0xc9,0xc9,0x40},
{0xfa,0x7d,0x7d,0x87},
{0xef,0xfa,0xfa,0x15},
{0xb2,0x59,0x59,0xeb},
{0x8e,0x47,0x47,0xc9},
{0xfb,0xf0,0xf0,0x0b},
{0x41,0xad,0xad,0xec},
{0xb3,0xd4,0xd4,0x67},
{0x5f,0xa2,0xa2,0xfd},
{0x45,0xaf,0xaf,0xea},
{0x23,0x9c,0x9c,0xbf},
{0x53,0xa4,0xa4,0xf7},
{0xe4,0x72,0x72,0x96},
{0x9b,0xc0,0xc0,0x5b},
{0x75,0xb7,0xb7,0xc2},
{0xe1,0xfd,0xfd,0x1c},
{0x3d,0x93,0x93,0xae},
{0x4c,0x26,0x26,0x6a},
{0x6c,0x36,0x36,0x5a},
{0x7e,0x3f,0x3f,0x41},
{0xf5,0xf7,0xf7,0x02},
{0x83,0xcc,0xcc,0x4f},
{0x68,0x34,0x34,0x5c},
{0x51,0xa5,0xa5,0xf4},
{0xd1,0xe5,0xe5,0x34},
{0xf9,0xf1,0xf1,0x08},
{0xe2,0x71,0x71,0x93},
{0xab,0xd8,0xd8,0x73},
{0x62,0x31,0x31,0x53},
{0x2a,0x15,0x15,0x3f},
{0x08,0x04,0x04,0x0c},
{0x95,0xc7,0xc7,0x52},
{0x46,0x23,0x23,0x65},
{0x9d,0xc3,0xc3,0x5e},
{0x30,0x18,0x18,0x28},
{0x37,0x96,0x96,0xa1},
{0x0a,0x05,0x05,0x0f},
{0x2f,0x9a,0x9a,0xb5},
{0x0e,0x07,0x07,0x09},
{0x24,0x12,0x12,0x36},
{0x1b,0x80,0x80,0x9b},
{0xdf,0xe2,0xe2,0x3d},
{0xcd,0xeb,0xeb,0x26},
{0x4e,0x27,0x27,0x69},
{0x7f,0xb2,0xb2,0xcd},
{0xea,0x75,0x75,0x9f},
{0x12,0x09,0x09,0x1b},
{0x1d,0x83,0x83,0x9e},
{0x58,0x2c,0x2c,0x74},
{0x34,0x1a,0x1a,0x2e},
{0x36,0x1b,0x1b,0x2d},
{0xdc,0x6e,0x6e,0xb2},
{0xb4,0x5a,0x5a,0xee},
{0x5b,0xa0,0xa0,0xfb},
{0xa4,0x52,0x52,0xf6},
{0x76,0x3b,0x3b,0x4d},
{0xb7,0xd6,0xd6,0x61},
{0x7d,0xb3,0xb3,0xce},
{0x52,0x29,0x29,0x7b},
{0xdd,0xe3,0xe3,0x3e},
{0x5e,0x2f,0x2f,0x71},
{0x13,0x84,0x84,0x97},
{0xa6,0x53,0x53,0xf5},
{0xb9,0xd1,0xd1,0x68},
{0x00,0x00,0x00,0x00},
{0xc1,0xed,0xed,0x2c},
{0x40,0x20,0x20,0x60},
{0xe3,0xfc,0xfc,0x1f},
{0x79,0xb1,0xb1,0xc8},
{0xb6,0x5b,0x5b,0xed},
{0xd4,0x6a,0x6a,0xbe},
{0x8d,0xcb,0xcb,0x46},
{0x67,0xbe,0xbe,0xd9},
{0x72,0x39,0x39,0x4b},
{0x94,0x4a,0x4a,0xde},
{0x98,0x4c,0x4c,0xd4},
{0xb0,0x58,0x58,0xe8},
{0x85,0xcf,0xcf,0x4a},
{0xbb,0xd0,0xd0,0x6b},
{0xc5,0xef,0xef,0x2a},
{0x4f,0xaa,0xaa,0xe5},
{0xed,0xfb,0xfb,0x16},
{0x86,0x43,0x43,0xc5},
{0x9a,0x4d,0x4d,0xd7},
{0x66,0x33,0x33,0x55},
{0x11,0x85,0x85,0x94},
{0x8a,0x45,0x45,0xcf},
{0xe9,0xf9,0xf9,0x10},
{0x04,0x02,0x02,0x06},
{0xfe,0x7f,0x7f,0x81},
{0xa0,0x50,0x50,0xf0},
{0x78,0x3c,0x3c,0x44},
{0x25,0x9f,0x9f,0xba},
{0x4b,0xa8,0xa8,0xe3},
{0xa2,0x51,0x51,0xf3},
{0x5d,0xa3,0xa3,0xfe},
{0x80,0x40,0x40,0xc0},
{0x05,0x8f,0x8f,0x8a},
{0x3f,0x92,0x92,0xad},
{0x21,0x9d,0x9d,0xbc},
{0x70,0x38,0x38,0x48},
{0xf1,0xf5,0xf5,0x04},
{0x63,0xbc,0xbc,0xdf},
{0x77,0xb6,0xb6,0xc1},
{0xaf,0xda,0xda,0x75},
{0x42,0x21,0x21,0x63},
{0x20,0x10,0x10,0x30},
{0xe5,0xff,0xff,0x1a},
{0xfd,0xf3,0xf3,0x0e},
{0xbf,0xd2,0xd2,0x6d},
{0x81,0xcd,0xcd,0x4c},
{0x18,0x0c,0x0c,0x14},
{0x26,0x13,0x13,0x35},
{0xc3,0xec,0xec,0x2f},
{0xbe,0x5f,0x5f,0xe1},
{0x35,0x97,0x97,0xa2},
{0x88,0x44,0x44,0xcc},
{0x2e,0x17,0x17,0x39},
{0x93,0xc4,0xc4,0x57},
{0x55,0xa7,0xa7,0xf2},
{0xfc,0x7e,0x7e,0x82},
{0x7a,0x3d,0x3d,0x47},
{0xc8,0x64,0x64,0xac},
{0xba,0x5d,0x5d,0xe7},
{0x32,0x19,0x19,0x2b},
{0xe6,0x73,0x73,0x95},
{0xc0,0x60,0x60,0xa0},
{0x19,0x81,0x81,0x98},
{0x9e,0x4f,0x4f,0xd1},
{0xa3,0xdc,0xdc,0x7f},
{0x44,0x22,0x22,0x66},
{0x54,0x2a,0x2a,0x7e},
{0x3b,0x90,0x90,0xab},
{0x0b,0x88,0x88,0x83},
{0x8c,0x46,0x46,0xca},
{0xc7,0xee,0xee,0x29},
{0x6b,0xb8,0xb8,0xd3},
{0x28,0x14,0x14,0x3c},
{0xa7,0xde,0xde,0x79},
{0xbc,0x5e,0x5e,0xe2},
{0x16,0x0b,0x0b,0x1d},
{0xad,0xdb,0xdb,0x76},
{0xdb,0xe0,0xe0,0x3b},
{0x64,0x32,0x32,0x56},
{0x74,0x3a,0x3a,0x4e},
{0x14,0x0a,0x0a,0x1e},
{0x92,0x49,0x49,0xdb},
{0x0c,0x06,0x06,0x0a},
{0x48,0x24,0x24,0x6c},
{0xb8,0x5c,0x5c,0xe4},
{0x9f,0xc2,0xc2,0x5d},
{0xbd,0xd3,0xd3,0x6e},
{0x43,0xac,0xac,0xef},
{0xc4,0x62,0x62,0xa6},
{0x39,0x91,0x91,0xa8},
{0x31,0x95,0x95,0xa4},
{0xd3,0xe4,0xe4,0x37},
{0xf2,0x79,0x79,0x8b},
{0xd5,0xe7,0xe7,0x32},
{0x8b,0xc8,0xc8,0x43},
{0x6e,0x37,0x37,0x59},
{0xda,0x6d,0x6d,0xb7},
{0x01,0x8d,0x8d,0x8c},
{0xb1,0xd5,0xd5,0x64},
{0x9c,0x4e,0x4e,0xd2},
{0x49,0xa9,0xa9,0xe0},
{0xd8,0x6c,0x6c,0xb4},
{0xac,0x56,0x56,0xfa},
{0xf3,0xf4,0xf4,0x07},
{0xcf,0xea,0xea,0x25},
{0xca,0x65,0x65,0xaf},
{0xf4,0x7a,0x7a,0x8e},
{0x47,0xae,0xae,0xe9},
{0x10,0x08,0x08,0x18},
{0x6f,0xba,0xba,0xd5},
{0xf0,0x78,0x78,0x88},
{0x4a,0x25,0x25,0x6f},
{0x5c,0x2e,0x2e,0x72},
{0x38,0x1c,0x1c,0x24},
{0x57,0xa6,0xa6,0xf1},
{0x73,0xb4,0xb4,0xc7},
{0x97,0xc6,0xc6,0x51},
{0xcb,0xe8,0xe8,0x23},
{0xa1,0xdd,0xdd,0x7c},
{0xe8,0x74,0x74,0x9c},
{0x3e,0x1f,0x1f,0x21},
{0x96,0x4b,0x4b,0xdd},
{0x61,0xbd,0xbd,0xdc},
{0x0d,0x8b,0x8b,0x86},
{0x0f,0x8a,0x8a,0x85},
{0xe0,0x70,0x70,0x90},
{0x7c,0x3e,0x3e,0x42},
{0x71,0xb5,0xb5,0xc4},
{0xcc,0x66,0x66,0xaa},
{0x90,0x48,0x48,0xd8},
{0x06,0x03,0x03,0x05},
{0xf7,0xf6,0xf6,0x01},
{0x1c,0x0e,0x0e,0x12},
{0xc2,0x61,0x61,0xa3},
{0x6a,0x35,0x35,0x5f},
{0xae,0x57,0x57,0xf9},
{0x69,0xb9,0xb9,0xd0},
{0x17,0x86,0x86,0x91},
{0x99,0xc1,0xc1,0x58},
{0x3a,0x1d,0x1d,0x27},
{0x27,0x9e,0x9e,0xb9},
{0xd9,0xe1,0xe1,0x38},
{0xeb,0xf8,0xf8,0x13},
{0x2b,0x98,0x98,0xb3},
{0x22,0x11,0x11,0x33},
{0xd2,0x69,0x69,0xbb},
{0xa9,0xd9,0xd9,0x70},
{0x07,0x8e,0x8e,0x89},
{0x33,0x94,0x94,0xa7},
{0x2d,0x9b,0x9b,0xb6},
{0x3c,0x1e,0x1e,0x22},
{0x15,0x87,0x87,0x92},
{0xc9,0xe9,0xe9,0x20},
{0x87,0xce,0xce,0x49},
{0xaa,0x55,0x55,0xff},
{0x50,0x28,0x28,0x78},
{0xa5,0xdf,0xdf,0x7a},
{0x03,0x8c,0x8c,0x8f},
{0x59,0xa1,0xa1,0xf8},
{0x09,0x89,0x89,0x80},
{0x1a,0x0d,0x0d,0x17},
{0x65,0xbf,0xbf,0xda},
{0xd7,0xe6,0xe6,0x31},
{0x84,0x42,0x42,0xc6},
{0xd0,0x68,0x68,0xb8},
{0x82,0x41,0x41,0xc3},
{0x29,0x99,0x99,0xb0},
{0x5a,0x2d,0x2d,0x77},
{0x1e,0x0f,0x0f,0x11},
{0x7b,0xb0,0xb0,0xcb},
{0xa8,0x54,0x54,0xfc},
{0x6d,0xbb,0xbb,0xd6},
{0x2c,0x16,0x16,0x3a},
};
uint16_t Key[20][4][4];//轮密钥
uint16_t Ans[20][4][4];//每轮的加密结果
uint16_t use[100][256][4][4];//产生数据用的数组
clock_t mystart, myend;//用于计时
void datagen()//用于产生符合要求的随机数据
{
	srand((int)time(0));
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			for (int u = 0; u < 4; u++)
			{
				for (int v = 0; v < 4; v++)
				{
					if (u == v && u == 0)
					{
						use[i][j][u][v] = j;
					}
					else
					{
						use[i][j][u][v] = rand() % 256;
					}
				}
			}
		}
	}
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			for (int u = 0; u < 4; u++)
			{
				for (int v = 0; v < 4; v++)
				{
					cout << use[i][j][u][v] << " ";
				}cout << endl;
			}
		}
	}
}
void Keygeneration(int i)//生成轮密钥
{
	uint16_t pur[4][4];
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			pur[j][k] = Key[i - 1][j][k];
			Key[i][j][k] = pur[j][k];
		}
	}
	uint16_t cur = pur[3][3];
	for (int j = 2; j >= 0; j--)
	{
		pur[j + 1][3] = pur[j][3];
	}
	pur[0][3] = cur;
	for (int j = 0; j < 4; j++)
	{
		uint16_t h = (pur[j][3] & 240) >> 4;
		uint16_t l = pur[j][3] & 15;
		pur[j][3] = Sbox[h][l];
	}
	pur[0][3] ^= RC[i - 1];
	for (int k = 0; k < 4; k++)
	{
		Key[i][k][0] ^= pur[k][3];
	}
	for (int j = 0; j < 4; j++)
	{
		for (int k = 1; k < 4; k++)
		{
			Key[i][j][k] = Key[i][j][k] ^ Key[i][j][k - 1];
		}
	}
	cout << "第" << i << "轮密钥" << endl;
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			cout << Key[i][j][k] << " ";
		}cout << endl;
	}
}
void ShiftRows(int i)//行移位
{
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			Ans[i][j][k] = Ans[i - 1][j][(k + j) % 4];
		}
	}
}
void SBMC(int i)//s盒与列混合一起的查表运算
{
	/*生成s盒和列混合表的过程*/
	/*for (uint16_t x = 0; x <= 255; x++)
	{
		uint16_t a = Sbox[(x & 240) >> 4][x & 15];
		MixC[x][0] = (((a >> 7) & 1) == 1 ? (((a << 1) & ((1 << 8) - 1)) ^ (0x1b)) : (a << 1) & ((1 << 8) - 1));
		MixC[x][1] = MixC[x][2] = a;
		MixC[x][3] = MixC[x][0] ^ a;
	}
	cout << "{";
	for (uint16_t x = 0; x <= 255; x++)
	{
		cout << "{";
		for (uint16_t j = 0; j < 4; j++)
		{
			if (j == 3)
			{
				cout << hex << (MixC[x][j] <= 15 ? "0x0" : "0x") << MixC[x][j];
			}
			else cout << hex << (MixC[x][j] <= 15 ? "0x0" : "0x") << MixC[x][j] << ",";
		}cout << "}," << endl;
	}
	cout << "}";
	*/
	/*查表过程*/
	uint16_t pre[4][4];//先用pre将数组复制出来
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			pre[j][k] = MixC[Ans[i][0][k]][j];
		}
	}
	for (int x = 1; x < 4; x++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				pre[j][k] ^= MixC[Ans[i][x][k]][(j - x + 4) % 4];
			}
		}
	}
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			Ans[i][j][k] = pre[j][k];
		}
	}
}
void AddRKey(int i)//轮密钥加
{
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			Ans[i][j][k] = Ans[i][j][k] ^ Key[i][j][k];
		}
	}
}
void subBytes(int i)//单独的s盒过程
{
	uint16_t pre[4][4];
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			pre[j][k] = Sbox[(Ans[i][j][k] & 240) >> 4][Ans[i][j][k] & 15];
		}
	}
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			Ans[i][j][k] = pre[j][k];
		}
	}
}
void AKinit(uint16_t myans[4][4], uint16_t mykey[4][4])//用于初始化明文和密钥，方便使用循环
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Key[0][i][j] = mykey[i][j];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Ans[0][i][j] = myans[i][j] ^ mykey[i][j];
		}
	}
}
void myAES128()
{
	for (int i = 1; i <= 10; i++)
	{
		if (i < 10)//前九轮正常进行
		{
			ShiftRows(i);
			SBMC(i);
			Keygeneration(i);
			AddRKey(i);
		}
		else//最后一轮没有列混合
		{
			ShiftRows(10);
			subBytes(10);
			Keygeneration(10);
			AddRKey(10);
		}
		cout << "第" << i << "轮:" << endl;
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				cout << (Ans[i][j][k] <= 15 ? "0" : "") << Ans[i][j][k] << " ";
			}cout << endl;
		}
	}
}
int main()
{
	//产生数据并输出至文件用于测试密码库用时，同时对于产生的数据测试自己用时
	//FILE* stream1;
	//freopen_s(&stream1, "out.txt", "w", stdout);//文件输出
	//datagen();
	/*mystart = clock();
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			AKinit(use[i][j], okey);
			myAES128();
		}
	}
	myend = clock();
	double endtime = (double)(myend - mystart) / CLOCKS_PER_SEC;
	cout << "用时：" << endtime * 1000 << "ms";
	*/
	AKinit(oans,okey);
	myAES128();
}