#pragma once

typedef struct tagRECTX
{
	LONG    left;
	LONG    top;
	LONG    right;
	LONG    bottom;
} RECTX, * PRECTX, NEAR* NPRECTX, FAR* LPRECTX;

void GetClientRect(sk::pose_t pose, sk::bounds_t bounds, RECTX* rect);