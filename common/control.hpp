#pragma once
#include<cmath>
#define NOW_MODEL "suzanne" //"suzanne" , "teapot" , "monster"
const int SCREEN_WIDTH = (1920 / 3);
const int SCREEN_HEIGHT = (1080 / 3);
const int SCREEN_SCALE_X = 2;
const int SCREEN_SCALE_Y = 2;
const int WORK_GROUP_SIZE_X = 4;
const int WORK_GROUP_SIZE_Y = 4;
const bool MOVE_VIEW = false;
const double PI = acos(-1);
namespace constants {
	const double gravity = 9.8;
	const double block_width = 6.0;
	const double block_height = 6.0;
	const double block_depth = 5.0;
}