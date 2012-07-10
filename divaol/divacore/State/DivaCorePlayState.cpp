/*
 *  DivaCoreGameState.cpp
 *
 *  Created by Hyf042 on 1/15/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 */

#include "DivaCorePlayState.h"
#include "Component/DivaStandardCoreFlow.h"
//#include "SoraSoundManager/SoraSoundEffectManager.h"

namespace divacore
{
	using namespace sora;

	void CorePlayState::onInitiate()
	{
		mFont = sora::SoraFont::LoadFromFile("cour.ttf", 20);

		mText.setFont(mFont);
		mText.setText(L"|#FF0000|Playing");

		bShowDetail = false;

		coreFlow = NULL;
	}
	void CorePlayState::onEnter()
	{
		coreFlow = core->getCoreFlow();
		
		if(coreFlow->getState()==CoreFlow::READY)
		{
			coreFlow->start();
			bFinish = false;
			start_time = new SoraTimestamp();
			volume = 1.0,bPressed = false;
		}
		setCoreState(Core::RUN);

		sora::SoraCore::Instance()->showMouse(false);
	}

	void CorePlayState::onLeave()
	{
		sora::SoraCore::Instance()->showMouse(true);

		SAFE_DELETE(start_time);
	}

	void CorePlayState::onRender()
	{	
		if(!bShowDetail)
			return;

#ifdef _DEBUG
		mText.render();

		char buffer[1000];
		sprintf(buffer,"|#FF0000|Time: %0.6lf\nSong: %0.6lf\nScore: %d\nHP: %d\nCombo: %d\nCT: %d\nLevel: %d\nVolume:%0.2f\nKey:%d",coreFlow->getRealTime(),/*MUSIC_MANAGER_PTR->getPosition("background")+*/0.419,Core::Ptr->getScore(),Core::Ptr->getHP(),GAME_MODE_PTR->getCombo(),0,0,volume,bPressed);
		mText.Render(sora::s2ws(buffer),0,200);
#endif
	}

	void CorePlayState::onUpdate(float dt)
	{
		if(bFinish)
		{
			if(!RENDER_SYSTEM_PTR->isFade())
			{
				setCoreState(Core::RESULT);

				core->flowOver();
				core->setState("result");
			}
		}
		else if(coreFlow->getState()==CoreFlow::END)
		{
			bFinish = true;
			mText.setText(L"|#FF0000|Finish!");

			RENDER_SYSTEM_PTR->fadeOut(Color::White.getHWColor());
		}
		else
		{
			static int count = 0;
			count = (count+1)%40;
			SoraWString text = L"|#FF0000|Playing";
			for(int i = 0; i < count/10; i++)
				text += L".";
			mText.setText(text);
		}
	}

	void CorePlayState::load(sora::SoraTask* task)
	{
	}

	void CorePlayState::onDestroy()
	{
	}

	void CorePlayState::onKeyPressed(SoraKeyEvent& event)
	{
		/*if(event.getKey()==SORA_KEY_SPACE)
		{
			core->getMusicManager()->playDirect("hit","sound_effect");
			//sora::SoraSoundEffectManager::Instance()->playDirect(L"hit.wav");
			bPressed = true;
		}
		else if(event.getKey()==SORA_KEY_UP)
		{
			volume += 0.01;
			volume = 1<volume?1:volume;
			core->getMusicManager()->setTagVolume("music",volume);
		}
		else if(event.getKey()==SORA_KEY_DOWN)
		{
			volume -= 0.01;
			volume = 0>volume?0:volume;
			core->getMusicManager()->setTagVolume("music",volume);
		}
		else if(event.getKey()==SORA_KEY_R)
		{
			core->restart();
		}
		else if(event.getKey()==SORA_KEY_H)
		{
			UI_PAINTER_PTR->hide();
		}
		else if(event.getKey()==SORA_KEY_N)
		{
			UI_PAINTER_PTR->display();
		}*/
#ifdef _DEBUG
		if(event.getKey()==SORA_KEY_TAB)
		{
			bShowDetail = !bShowDetail;
		}
#endif
		if(event.getKey()==SORA_KEY_J)
		{
			StandardEditUtility::instance().init();
			StandardEditUtility::instance().setPosition(400);
		}
		else if(event.getKey()==SORA_KEY_D)
		{
			StandardEditUtility::instance().init();
			MapEvent event;
			event.position = 1;
			event.eventType = "bpm";
			event.arg["value"] = 100.0;

			StandardEditUtility::instance().addEvent(event);
			StandardEditUtility::instance().reCaltTime();
		}
		else if(event.getKey()==SORA_KEY_R)
		{
			StandardEditUtility::instance().init();
			MapEvent event;
			event.position = 2;
			event.eventType = "bpm";
			event.arg["value"] = 175.0;

			StandardEditUtility::instance().addEvent(event);
			StandardEditUtility::instance().reCaltTime();
		}
		/*else if(event.getKey()==SORA_KEY_C)
		{
			Packet packet((uint32)1,0);
			NETWORK_SYSTEM_PTR->send(packet);
		}*/
	}
	void CorePlayState::onKeyReleased(SoraKeyEvent& event)
	{
#ifdef _DEBUG
		if(event.getKey()==SORA_KEY_P)
			core->setState("pause");
#endif
		if(event.getKey()==SORA_KEY_P)
			core->setState("pause");
	}
}