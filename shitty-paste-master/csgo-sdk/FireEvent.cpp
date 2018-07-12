#include "csgo-sdk.h"

#pragma comment(lib, "Winmm.lib") 

FireEventFn oFireEvent;

bool __fastcall Hooks::FireEvent(void* thisptr, void* edx, IGameEvent* event) // idx 9
{
	if(!event)
		return oFireEvent(thisptr, event);

	std::string event_name = event->GetName();

	/*if (!strcmp(event_name.c_str(), "player_connect")) // server-side event, creds: kidua
	{
		if (event->GetInt("bot") == 0)
		{
			try
			{
				std::string player_name = event->GetString("name");
				std::string player_ip = event->GetString("address");

				std::cout << "[IP] " << player_name << " --> " << player_ip << std::endl;
			}
			catch (...)
			{
				std::cout << "[IP] Failed getting IP, possibly in a local game?" << std::endl;
			}
		}
	}*/

	// antiaim stuff
	if (event_name.find("round_prestart") != std::string::npos || event_name.find("round_end") != std::string::npos)
	{
		if (event_name.find("round_end") != std::string::npos)
		{
			Backtracking->InitializeLagRecord();
			memset(&G::ShotsFired, 0, sizeof(G::ShotsFired)); // reset shotsfired
		}
		else
		{
			Backtracking->InitializeLagRecord();
			memset(&G::ShotsFired, 0, sizeof(G::ShotsFired)); // reset shotsfired
			G::ShouldAA = false;
		}
	}

	if (event_name.find("round_freeze_end") != std::string::npos)
	{
		G::ShouldAA = true;
	}

	// brute-force and hitmarker, damage log
	if (event_name.find("player_hurt") != std::string::npos)
	{
		auto user_id = Interfaces->EngineClient->GetPlayerForUserID(event->GetInt("userid"));
		auto attacker_id = Interfaces->EngineClient->GetPlayerForUserID(event->GetInt("attacker"));

		CBaseEntity* victim = Interfaces->ClientEntityList->GetClientEntity(user_id);
		CBaseEntity* attacker = Interfaces->ClientEntityList->GetClientEntity(attacker_id);

		if (attacker == G::LocalPlayer)
		{
			if(G::ShotsFired[user_id])
				G::ShotsFired[user_id]--; // cancel out any changes done by the aimbot
		
			auto health_remaining = event->GetInt("health"); // also old health is still the same client-side bc its networked before being passed
															// onto the netvar xd
			auto damage_inflicted = clamp(victim->GetHealth() - health_remaining, 0, 100); // assume that 100 is max bc fuck it

			//DamageLog->Add(victim, damage_inflicted);
			
			if (g_pSettings->VisualsHitMarker->Checked)
			{
				flHitmarkerAlpha = 1.0f;
			}

			if (g_pSettings->VisualsHitMarkerSound->Checked)
			{
				const char*             _soundFileName = "csgo\\sound\\hitsound.wav";
				try
				{
					PlaySoundA(_soundFileName, NULL, SND_ASYNC);
				}
				catch (...)
				{
					// dont fucking crash people that dont have the sound installed ok?
				}
			}
		}
	}


	return oFireEvent(thisptr, event);
}