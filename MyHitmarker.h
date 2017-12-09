#pragma once

#include <Windows.h>
#pragma comment(lib, "Winmm.lib") 
#include "Features.h"
#include "Sounds.h"

#define EVENT_DEBUG_ID_INIT 0x2A

class hitmarker
{
	class player_hurt_listener
		: public IGameEventListener2
	{
	public:
		void start()
		{
			if (!pGameEvent->AddListener(this, "player_hurt", false)) {
				//throw misc::exception("Failed to register the event");
			}
		}
		void stop()
		{
			pGameEvent->RemoveListener(this);
		}
		void FireGameEvent(IGameEvent *event) override
		{
			hitmarker::singleton()->on_fire_event(event);
		}
		int GetEventDebugID(void) override
		{
			return EVENT_DEBUG_ID_INIT /*0x2A*/;
		}
	};
public:
	static hitmarker* singleton()
	{
		static hitmarker* instance = new hitmarker;
		return instance;
	}

	void initialize()
	{
		_listener.start();
	}

	void on_fire_event(IGameEvent* event)
	{
		if (!strcmp(event->GetName(), "player_hurt")) {
			int attacker = event->GetInt("attacker");
			int damage = event->GetInt("dmg_health");
			if (pEngine->GetPlayerForUserID(attacker) == pEngine->GetLocalPlayer()) {
				
				if (!Settings::Misc::MyHitmarker::enabled)
					return;

				int sound = Settings::Misc::MyHitmarker::sound;
				switch (sound)
				{
				case 0:
					_soundFileName = "csgo\\sound\\hitmarker\\hitsound.wav";
					PlaySoundA(_soundFileName, NULL, SND_ASYNC);
					break;
				case 1:
					PlaySoundA(SDK::roblox, NULL, SND_ASYNC);
					break;
				case 2:
					PlaySoundA(SDK::hitler_wav, NULL, SND_ASYNC);
					break;
				case 3:
					_soundFileName = "csgo\\sound\\hitmarker\\nice.wav";
					PlaySoundA(_soundFileName, NULL, SND_ASYNC);
					break;
				case 4:
					PlaySoundA(SDK::pew, NULL, SND_ASYNC);
					break;
				}

				
				_flHurtTime = pGlobalVars->curtime;
			}
		}
	}

	void on_paint()
	{
		auto curtime = pGlobalVars->curtime;
		auto lineSize = 8;
		if (_flHurtTime + 0.25f >= curtime) {
			int screenSizeX, screenCenterX;
			int screenSizeY, screenCenterY;
			pEngine->GetScreenSize(screenSizeX, screenSizeY);

			screenCenterX = screenSizeX / 2;
			screenCenterY = screenSizeY / 2;

			
			//pSurface->DrawSetColor(Settings::ESP::Hitmarker::color.Color().Value.x, Settings::ESP::Hitmarker::color.Color().Value.y, Settings::ESP::Hitmarker::color.Color().Value.z, Settings::ESP::Hitmarker::color.Color().Value.w);
			pSurface->DrawSetColor(200, 200, 200, 255);
			pSurface->DrawLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4));
			pSurface->DrawLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4));
			pSurface->DrawLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4));
			pSurface->DrawLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4));
		}
	}

private:
	player_hurt_listener    _listener;
	const char*             _soundFileName = "csgo\\sound\\hitmarker\\hitsound.wav";
	float                   _flHurtTime;
};