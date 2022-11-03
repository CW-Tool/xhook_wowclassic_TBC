#pragma once
#include "WObject.h"
#ifndef _H_NAV
#define _H_NAV


class nav
{
public:
	inline static bool StartNavigator = false;
	static bool GetPath(WObject* gobject);
	static void Move();
	inline static std::vector<Vector3> Waypoints;
private:
	//static std::vector<Vector3> _Waypoints;
	
};
#endif
