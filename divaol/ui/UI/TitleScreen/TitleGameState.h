#pragma once

#include "SoraCanvas.h"
#include "guichan.hpp"
#include "guichansetup.h"

#include "SoraCore.h"
#include "TitleScreenUI.h"

#include "SoraCore.h"

#include "SoraWindowInfo.h"
#include "SoraFont.h"

#include "SoraDirectoryIterator.h"
#include "SoraFileUtility.h"
#include "SoraFont.h"
#include "SoraText.h"

#include "app/SoraGameApp.h"
#include "app/SoraGameState.h"

#include "SoraResourceFile.h"

#include "SoraFMODSoundSystem.h"
#include "ui/Config/DivaUIConfig.h"
#include "Utility/DivaSettings.h"
#include "cmd/SoraConsole.h"
#include "SoraTextureMap.h"

namespace diva
{

	class TitleGameState: public sora::SoraGameState, public sora::SoraEventHandler {
	public:
		TitleGameState() {
			//this->app = app;
			isInitialized = false;
			state = 0;
			nowTime = 0;
			//nextState = "";
		}

		~TitleGameState()
		{
			if (uiCanvas)
				delete uiCanvas;
		}

		void onRender() {


			uiCanvas->beginRender(0xFFFFFFFF);

			//room->Render();
			titleScreen->Render();
			sora::GCN_GLOBAL->gcnDraw();

			uiCanvas->finishRender();



			sora::SoraGameApp::BeginScene();

			sora::SoraSprite* sprite = uiCanvas->getCanvasSprite();
			sprite->setScale(double(setConfig[L"windowWidth"].asInt()) / setConfig[L"gameWidth"].asInt(), double(setConfig[L"windowHeight"].asInt()) / setConfig[L"gameHeight"].asInt());
			sprite->render(0, 0);

			sora::SoraGameApp::EndScene();

		}

		void beginLeave(const std::string& nextState)
		{
			if (nowTime < config[L"titleScreenDelayTime"].asDouble())
				return;
			if (nextState == "init" || state == 1)
				return;
			
			sora::SoraBGMManager::Instance()->playSE(soundConfig[L"pressAnyKey"].asString(), SETTINGS.getSEVolume());

			this->nextState = nextState;
			titleScreen->BeginLeave();
			uiCanvas->getCanvasSprite()->addEffect(sora::CreateEffectFade(1.0, 0.0, 0.5));
			state = 1;
		}

		void onLeave()
		{
			titleScreen->Leave();
		}

		void onUpdate(float dt) {   
			//DivaCEGUI::Instance()->Update(dt);
			//room->Update(dt);
			if (state == 1)
			{
				if (!uiCanvas->getCanvasSprite()->hasEffect())
					NextState = nextState, state = 0;
			}
			if (NextState!="")
			{
				getGameApp()->setState(NextState);
				titleScreen->Release();

				std::string fuck = sora::SoraTextureMap::Instance()->getTexRefInfo();
				NextState = "";
				return;
			}
			titleScreen->Update(dt);
			uiCanvas->update(dt);
			nowTime += dt;
		}


		void onKeyPressed(sora::SoraKeyEvent& keyEvent) {
			sora::SoraBGMManager::Instance()->setFadeTime(setConfig[L"bgmFadeTime"].asDouble(), setConfig[L"bgmFadeTime"].asDouble());
			//getGameApp()->setState("house");
			beginLeave("house");
		}

		void onMouseClicked(SoraMouseEvent& event) {
			sora::SoraBGMManager::Instance()->setFadeTime(setConfig[L"bgmFadeTime"].asDouble(), setConfig[L"bgmFadeTime"].asDouble());
			
			beginLeave("house");
		}

		void onMouseDragged(sora::SoraMouseEvent& from, sora::SoraMouseEvent& to) {
			//y += to.x - from.x;
			//getGameApp()->setState("house");
		}

		void onEnter() {

			if (!isInitialized)
			{
				//diva::MapMgr::Instance()->Load("map_manifest.json");

				//room = UIScreenRoom::Instance();

				std::string fuck = sora::SoraTextureMap::Instance()->getTexRefInfo();
				sora::Logger.log(fuck);
					
					

				titleScreen = TitleScreenUI::Instance();

				fuck = sora::SoraTextureMap::Instance()->getTexRefInfo();

				uiCanvas = new sora::SoraBaseCanvas(setConfig[L"gameWidth"].asInt(), setConfig[L"gameHeight"].asInt());

				isInitialized = true;
			}

			sora::SoraCore::Ptr->setFPS(60);
			uiCanvas->getCanvasSprite()->addEffect(sora::CreateEffectFade(0.0, 1.0, 0.5));
			titleScreen->Enter();

		}

	private:

		sora::SoraBaseCanvas* uiCanvas;
		//std::string nextState;
		TitleScreenUI* titleScreen;
		bool isInitialized;
		int state;
		std::string nextState;
		float nowTime;
		//sora::SoraGameApp* app;
	};

}