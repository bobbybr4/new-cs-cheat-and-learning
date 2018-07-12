#pragma once

#define Miscellaneous CMiscellaneous::GetInstance()

class CMiscellaneous {
	IMPLEMENT_SINGLETON(CMiscellaneous);
public:
	void Tick();
	void FixMovement(QAngle& wish_angle);
	void AntiAim();
	void Fakelag();

private:
	void Bunnyhop();
	void KnifeTrigger();
	void Autostrafe();
	void Airstuck();
};
