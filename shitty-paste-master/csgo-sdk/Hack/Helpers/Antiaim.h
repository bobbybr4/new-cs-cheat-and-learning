#pragma once

/*
pitch: off / default / minimal / fake down / fake zero
yaw: off / 180 / 180 jitter / 90 directional / spin
fake: off / face / inverse real / half real
moving yaw: off / 180 / 180 jitter / 90 directional / spin
edge: off / static / freestanding
dynamic
*/

#define Antiaim CAntiAim::GetInstance()

class CAntiAim {
	IMPLEMENT_SINGLETON(CAntiAim);

public:
	void Run();
};

float GetServerTime();
bool ShouldFake();
