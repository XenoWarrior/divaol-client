/*
 *  DivaJsonGameModule.h
 *
 *  Created by Hyf042 on 2/7/12.
 *  Copyright 2012 Hyf042. All rights reserved.
 *
 *  ---------------------------------------------
 *
 *	Modified by XenoWarrior (Ashley) on 20/10/16
 *
 *  Definition for JsonGameModule.
 *
 *  //! TODO:
 *		- Identify what access level the variables without access levels should be placed under.
 *		- Move implemented functions to DivaJsonGameModule.cpp.
 *
 *  //! FIXME:
 *		- None found yet.
 *
 */

#ifndef DIVA_JSON_GAME_MODULE
#define DIVA_JSON_GAME_MODULE

#include "Core/DivaGameModule.h"

namespace divacore
{
	class JsonGameModule : public GameModule
	{
		Config config;
		std::string name,path;
		std::string noteFile;
		std::string uiFile;
		std::string renderFile;
		std::string effectFile;
		std::string resultFile;
		Config noteConfig;
		Config renderConfig;
		Config effectConfig;
		std::vector<std::string> hooks;

		std::vector<sora::SoraResourceHandle> folders;
		std::vector<std::string> folderPaths;

	public:
		void init() {}

		void load(const std::string &path);
		void loadConfig(const std::string &path);
		void gameStop();
		void registerHook();
		void distribute();
		void clear();

		std::string getName() { return name; }
		std::string getNoteFile() {return noteFile;}
		std::string getUIFile() {return uiFile;}
		std::string getRenderFile() {return renderFile;}
		std::string getEffectFile() {return effectFile;}
		std::string getResultFile() {return resultFile;}
	};
}

#endif