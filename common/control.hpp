#pragma once
#include<cmath>
#define NOW_MODEL "suzanne" //"suzanne" , "teapot" , "monster"
#define WIDTH 1024
#define HEIGHT 1024
#define WORK_GROUP_SIZE_X 8
#define WORK_GROUP_SIZE_Y 8
#define MOVE_VIEW false
const double PI = acos(-1);
namespace constants {
	const double gravity = 9.8;
}