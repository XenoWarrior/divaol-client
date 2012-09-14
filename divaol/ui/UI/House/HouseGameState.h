#pragma once

#include "ui/Config/DivaUIConfig.h"
#include "app/SoraGameState.h"
#include "app/SoraGameApp.h"
#include "SoraEvent.h"
#include "SoraCanvas.h"
#include "SoraCore.h"
#include "SoraSprite.h"
#include "guichansetup.h"

#include "HouseUI.h"

namespace diva
{
	namespace HouseUI
	{
		class HouseGameState: public sora::SoraGameState, public sora::SoraEventHandler 
		{
		public:
			bool initialized;
			sora::SoraBaseCanvas* uiCanvas;
			HouseUI* houseUI;

			HouseGameState()
			{
				initialized = false;
			}

			void onEnter()
			{
				if (!initialized)
				{
					uiCanvas = new sora::SoraBaseCanvas(config[L"gameWidth"].asInt(), config[L"gameHeight"].asInt());
					houseUI = HouseUI::Instance();

					initialized = true;
				}
				sora::SoraCore::Instance()->setFPS(60);
			}

			void onUpdate(float dt) {   
				houseUI->Update(dt);
			}

			void onRender() {

				uiCanvas->beginRender(0);

				sora::GCN_GLOBAL->gcnDraw();
				houseUI->Render();

				uiCanvas->finishRender();

				// ------------------------------

				sora::SoraGameApp::BeginScene();

				sora::SoraSprite* sprite = uiCanvas->getCanvasSprite();
				sprite->setScale(double(config[L"windowWidth"].asInt()) / config[L"gameWidth"].asInt(), double(config[L"windowHeight"].asInt()) / config[L"gameHeight"].asInt());
				sprite->render(0, 0);

				sora::SoraGameApp::EndScene();
			}
		};

	}

}