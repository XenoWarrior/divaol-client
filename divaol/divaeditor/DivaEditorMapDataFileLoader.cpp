﻿#include <algorithm>
#include <cmath>
#include <map>
#include <vector>
#include <direct.h>
#include <locale>
#include "stdio.h"

#include "divaeditor/DivaEditorMapData.h"
#include "divaeditor/DivaEditorCommon.h"

#include "divacore/Component/DivaStandardCoreFlow.h"

#include "WJson/wjson.h"

namespace divaeditor
{
	
	DivaEditorMapData::InitSourceFileType DivaEditorMapData::parseInitFileType(std::wstring path)
	{
		std::wstring ext=L"";
		for(int i=path.length()-1;i>=0;i--)
		{
			if(path[i]==L'.')
				break;
			ext = path[i] + ext;
		}

		tolower(ext);

		const int fileTypeCount = 8;
		std::wstring fileTypes[fileTypeCount] = {
			L"diva",
			L"divaproject",
			L"divaol",
			L"divaolproject" , 
			L"mp3,wav,ogg", 
			L"bms",
			L"osu",
			L"mid"};

		for(int i=0;i<fileTypeCount;i++)
		{
			if(fileTypes[i].find(ext)!=std::wstring::npos)
				return (InitSourceFileType)i;
		}
		return InitSourceFileType::Unknown;
	}

	std::wstring DivaEditorMapData::InitFromFile(std::wstring path)
	{
		InitSourceFileType fileType = parseInitFileType(path);

		if(fileType==InitSourceFileType::DivaPCFile)
			return InitFromDivaPCFile(path);
		else if(fileType==InitSourceFileType::DivaPCProFile)
			return InitFromDivaPCProFile(path);
		else if(fileType==InitSourceFileType::DivaOLFile)
			return InitFromDivaOLFile(path);
		else if(fileType==InitSourceFileType::DivaOLProFile)
			return InitFromDivaOLProFile(path);
		else if(fileType==InitSourceFileType::MusicFile)
			return InitFromMusicFile(path);
		else if(fileType==InitSourceFileType::BMSFile)
			return InitFromBMSFile(path);
		else if(fileType==InitSourceFileType::OSUFile)
			return InitFromOSUFile(path);
		else if(fileType==InitSourceFileType::MIDIFile)
			return InitFromMIDIFile(path);
		else
			return path + L" is not supported.";
	}
	std::wstring DivaEditorMapData::ChooseWorkingFile()
	{
		wchar_t cwd[_MAX_PATH];
		_wgetcwd(cwd,_MAX_PATH);

		wstring saveFile = sora::SoraCore::Instance()->fileSaveDialogW(L"Diva Online Map Project(*.divaolproject)\0*.divaolproject\0",NULL,L".divaproject");

		_wchdir(cwd);

		if(saveFile!=L"")
		{
			int pathPos,dotPos;
			for(pathPos=saveFile.length()-1;pathPos>=0;pathPos--)
				if(saveFile[pathPos]==L'\\'||saveFile[pathPos]==L'/')
					break;

			for(dotPos=saveFile.length()-1;dotPos>=0;dotPos--)
				if(saveFile[dotPos]==L'.')
					break;

			if(pathPos==-1) return L"Diva OL Project Path Error: " + saveFile;

			workingDirectory = saveFile.substr(0,pathPos);
			workingDivaOLFile = saveFile.substr(pathPos+1, dotPos-(pathPos+1)) + L".divaol";
			workingDivaOLProFile = saveFile.substr(pathPos+1, saveFile.length()-(pathPos+1));

			return L"OK";
		}
		else
			return L"Canceled";
	}
	bool DivaEditorMapData::SaveFile(bool olFile, bool proFile, divacore::MapInfo *infoToSave)
	{
		if(workingDirectory==L"" || workingDivaOLFile==L"" || workingDivaOLProFile==L"")
			return false;

		CreateDirectoryW(workingDirectory.c_str(),NULL);

#pragma region Save DivaOL Play File
		if(olFile)
		{
			if(infoToSave==NULL)
				infoToSave=coreInfoPtr;

			WJson::StyledWriter writer;
			
			WJson::Value rootJsonValue;

			WJson::Value eventJsonValue;
			WJson::Value headerJsonValue;
			WJson::Value noteListJsonValue;
			WJson::Value resourceListJsonValue;

#pragma region Convert Event to json
			for(int i=0;i<infoToSave->events.size();i++)
			{
				WJson::Value thisEventJsonValue;
				divacore::MapEvent &thisEvent = infoToSave->events[i];

				thisEventJsonValue[L"type"] = s2ws(thisEvent.eventType);
				thisEventJsonValue[L"position"] = thisEvent.position;

				for (map<std::string,sora::SoraAny>::iterator argI=thisEvent.arg.begin();argI!=thisEvent.arg.end();argI++)
				{
					if(argI->first=="value")
						thisEventJsonValue[s2ws(argI->first)] = Argument::asDouble(argI->first, thisEvent.arg);

					else if(argI->first=="width"
							|| argI->first=="height"
							|| argI->first=="tex_width"
							|| argI->first=="tex_height"
							)
						thisEventJsonValue[s2ws(argI->first)] = Argument::asInt(argI->first, thisEvent.arg);

					else if(argI->first=="id"
							|| argI->first=="tag"
							|| argI->first=="channel"
							)
						thisEventJsonValue[s2ws(argI->first)] = s2ws(Argument::asString(argI->first, thisEvent.arg));
				}
				eventJsonValue.append(thisEventJsonValue);
			}
			rootJsonValue[L"event"] = eventJsonValue;
#pragma endregion Convert Event to json

#pragma region Convert header to json

			headerJsonValue[L"bpm"] = infoToSave->header.BPM;
			headerJsonValue[L"difficulty"] = infoToSave->header.difficulty;
			headerJsonValue[L"hard"] = infoToSave->header.hardLevel;
			headerJsonValue[L"mainSound"] = s2ws(infoToSave->header.mainSound);
			headerJsonValue[L"name"] = infoToSave->header.mapName;
			
			WJson::Value noterListJsonValue;
			for (int i=0;i<infoToSave->header.noter.size();i++)
			{
				WJson::Value noterNameJsonValue;
				noterNameJsonValue[L"name"] = infoToSave->header.noter[i];
				noterListJsonValue.append(noterNameJsonValue);
			}
			headerJsonValue[L"noter"] = noterListJsonValue;
			headerJsonValue[L"speedScale"] = (double)infoToSave->header.speedScale;
			headerJsonValue[L"version"] = s2ws(infoToSave->header.versionStr);

			rootJsonValue[L"header"] = headerJsonValue;
#pragma endregion Convert header to json

			rootJsonValue[L"module"] = s2ws(infoToSave->module);

#pragma region notes
			for (int i=0;i<infoToSave->notes.size();i++)
			{
				WJson::Value noteJsonValue;
				divacore::MapNote &thisNote = infoToSave->notes[i];

				noteJsonValue[L"type"] = s2ws(thisNote.noteType);
				
				WJson::Value notePointListJsonValue;
				for (int notePointI=0;notePointI<thisNote.notePoint.size();notePointI++)
				{
					WJson::Value notePointJsonValue;
					divacore::NotePoint &thisNotePoint = thisNote.notePoint[notePointI];

					notePointJsonValue[L"type"] = thisNotePoint.type;
					notePointJsonValue[L"position"] = thisNotePoint.position;
					notePointJsonValue[L"x"] = thisNotePoint.x;
					notePointJsonValue[L"y"] = thisNotePoint.y;
					if(thisNotePoint.key!="")
						notePointJsonValue[L"key"] = s2ws(thisNotePoint.key);

					notePointListJsonValue.append(notePointJsonValue);
				}

				noteJsonValue[L"notePoint"] = notePointListJsonValue;

				for (map<std::string, sora::SoraAny>::iterator argI = thisNote.arg.begin();argI!=thisNote.arg.end();argI++)
				{
					if(argI->first == "tailx"
						|| argI->first == "taily"
						|| argI->first == "standard_num"
						)
						noteJsonValue[s2ws(argI->first)] = Argument::asInt(argI->first, thisNote.arg);
				}
				noteListJsonValue.append(noteJsonValue);
			}

			rootJsonValue[L"note"] = noteListJsonValue;
#pragma endregion notes

#pragma region resource
			for(map<std::string, divacore::MapResourceInfo>::iterator resourceI=infoToSave->resources.begin();resourceI!=infoToSave->resources.end();resourceI++)
			{
				WJson::Value resourceJsonValue;
				divacore::MapResourceInfo &resourceInfo = resourceI->second;

				resourceJsonValue[L"file"] = resourceInfo.filePath;
				resourceJsonValue[L"id"] = s2ws(resourceInfo.ID);

				if(resourceInfo.type==divacore::MapResourceInfo::VIDEO)
					resourceJsonValue[L"type"] = L"video";
				if(resourceInfo.type==divacore::MapResourceInfo::AUDIO)
				{
					resourceJsonValue[L"type"] = L"audio";
					resourceJsonValue[L"stream"] = resourceInfo.flag;
				}
				if(resourceInfo.type==divacore::MapResourceInfo::IMAGE)
					resourceJsonValue[L"type"] = L"image";

				resourceListJsonValue.append(resourceJsonValue);
			}

			rootJsonValue[L"resource"] = resourceListJsonValue;
#pragma endregion resource
			/*
			std::string jsonStrToSave = to_utf8(writer.write(rootJsonValue));

			locale &loc=locale::global(locale(locale(),"",LC_CTYPE));
			std::wofstream outFile(workingDirectory+L"/"+workingDivaOLFile);
			locale::global(loc);
			*/
			std::string jsonStrToSave = to_utf8(writer.write(rootJsonValue));
			std::ofstream outFile(workingDirectory+L"/"+workingDivaOLFile);

			outFile << jsonStrToSave;
			outFile.close();
		}
#pragma endregion Save DivaOL Play File
		
#pragma region Save Diva Online Project File

		if(proFile)
		{
			WJson::StyledWriter writer;

			WJson::Value rootJsonValue;

			WJson::Value headerJsonValue;
			WJson::Value stopListJsonValue;
			WJson::Value beatNumListJsonValue;
			WJson::Value descriptionListJsonValue;

			//header
			headerJsonValue[L"divaol"] = workingDivaOLFile;
			headerJsonValue[L"offset"] = mapOffset;
			headerJsonValue[L"version"] = s2ws(EditorVer);

			rootJsonValue[L"header"] = headerJsonValue;

#pragma region editor info

			//Stops
			for (map<int,int>::iterator stopI=stopLength.begin();stopI!=stopLength.end();stopI++)
			{
				WJson::Value stopJsonValue;
				stopJsonValue[L"position"] = stopI->first;
				stopJsonValue[L"value"] = stopI->second;

				stopListJsonValue.append(stopJsonValue);
			}

			rootJsonValue[L"stopblock"] = stopListJsonValue;


			//Beat Num
			for (map<int,int>::iterator beatNumI = beatNumChanged.begin();beatNumI!=beatNumChanged.end();beatNumI++)
			{
				WJson::Value beatJsonValue;
				beatJsonValue[L"period"] = beatNumI->first;
				beatJsonValue[L"beat"] = beatNumI->second;

				beatNumListJsonValue.append(beatJsonValue);
			}

			rootJsonValue[L"beat"] = beatNumListJsonValue;

			//Resource Description
			for(map<std::string,std::wstring>::iterator descriptionI = resourceDescription.begin();descriptionI!=resourceDescription.end();descriptionI++)
			{
				WJson::Value descriptionJsonValue;
				descriptionJsonValue[L"id"] = s2ws(descriptionI->first);
				descriptionJsonValue[L"label"] = descriptionI->second;

				descriptionListJsonValue.append(descriptionJsonValue);
			}

			rootJsonValue[L"label"] = descriptionListJsonValue;

#pragma endregion editor info

			/*
			std::wstring jsonStrToSave = writer.write(rootJsonValue);

			locale &loc=locale::global(locale(locale(),"",LC_CTYPE));
			std::wofstream outFile(workingDirectory+L"/"+workingDivaOLProFile);
			locale::global(loc);
			*/
			std::string jsonStrToSave = to_utf8(writer.write(rootJsonValue));
			std::ofstream outFile(workingDirectory+L"/"+workingDivaOLProFile);

			outFile << jsonStrToSave;
			outFile.close();
		}

#pragma endregion Save Diva Online Project File


		return true;
	}

	std::wstring DivaEditorMapData::InitFromDivaOLProFile(std::wstring path)
	{
		int pathPos,dotPos;
		for(pathPos=path.length()-1;pathPos>=0;pathPos--)
			if(path[pathPos]==L'\\'||path[pathPos]==L'/')
				break;

		for(dotPos=path.length()-1;dotPos>=0;dotPos--)
			if(path[dotPos]==L'.')
				break;

		if(pathPos==-1) return L"File path error : " + path;

		workingDirectory = path.substr(0,pathPos);
		workingDivaOLProFile = path.substr(pathPos+1, path.length()-(pathPos+1));


		//Read file

		std::wstring jsonStrToParse;

		FILE* readFile;
		if(_wfopen_s(&readFile, (workingDirectory+L"/"+workingDivaOLProFile).c_str(),L"rt, ccs=UTF-8")!=0)
			return L"Error while open file: " + path;

		wchar_t buffer[1000];
		while(fgetws(buffer,sizeof(buffer),readFile))
			jsonStrToParse += std::wstring(buffer);

		
		WJson::Reader reader;

		WJson::Value rootJsonValue;
		if(!reader.parse(jsonStrToParse,rootJsonValue))
			return L"Error while parse json file: " + path;

		//Parse header
		if(rootJsonValue.isMember(L"header"))
		{
			WJson::Value header = rootJsonValue[L"header"];
			if(header.isMember(L"divaol"))
				workingDivaOLFile = header[L"divaol"].asString();
			else
				return L"Error: divaol tag not exist";

			if(header.isMember(L"offset"))
				mapOffset = header[L"offset"].asInt();
			else
				return L"Error: offset tag not exist";
		}
		else
			return L"Error: header not exist";

		//Parse stop
		stopLength.clear();
		if(rootJsonValue.isMember(L"stopblock"))
		{
			WJson::Value stopList = rootJsonValue[L"stopblock"];
			if(!stopList.isNull() && stopList.isArray())
				for (int i=0;i<stopList.size();i++)
				{
					WJson::Value stopJsonValue = stopList[i];
					stopLength[stopJsonValue[L"position"].asInt()] = stopJsonValue[L"value"].asInt();
				}
		}
		else
			return L"Error: stopblock not exist";

		//Parse beat num
		beatNumChanged.clear();
		if(rootJsonValue.isMember(L"beat"))
		{
			WJson::Value beatNumList = rootJsonValue[L"beat"];
			if(!beatNumList.isNull() && beatNumList.isArray())
				for (int i=0;i<beatNumList.size();i++)
				{
					WJson::Value beatNumValue = beatNumList[i];
					beatNumChanged[beatNumValue[L"period"].asInt()] = beatNumValue[L"beat"].asInt();
				}
		}
		else
			return L"Error: beat info not exist";

		//Parse label
		resourceDescription.clear();
		if(rootJsonValue.isMember(L"label"))
		{
			WJson::Value labelList = rootJsonValue[L"label"];
			if(!labelList.isNull() && labelList.isArray())
				for (int i=0;i<labelList.size();i++)
				{
					WJson::Value labelValue = labelList[i];
					resourceDescription[ws2s(labelValue[L"id"].asString())] = labelValue[L"label"].asString();
				}
		}
		else
			return L"Error: label info not exist";


		CORE_PTR->setSong(workingDirectory, workingDivaOLFile);

		return L"OK";
	}
	std::wstring DivaEditorMapData::InitFromDivaOLFile(std::wstring path)
	{
		int pathPos,dotPos;
		for(pathPos=path.length()-1;pathPos>=0;pathPos--)
			if(path[pathPos]==L'\\'||path[pathPos]==L'/')
				break;

		for(dotPos=path.length()-1;dotPos>=0;dotPos--)
			if(path[dotPos]==L'.')
				break;

		if(pathPos==-1) return L"File path error : " + path;

		workingDirectory = path.substr(0,pathPos);
		workingDivaOLFile = path.substr(pathPos+1, path.length()-(pathPos+1));
		workingDivaOLProFile = path.substr(pathPos+1, dotPos-(pathPos+1)) + L".divaolproject";

		//Init editor default value
		ResetEditorMapData();

		SaveFile(false);

		CORE_PTR->setSong(workingDirectory,workingDivaOLFile);

		return L"OK";
	}
	std::wstring DivaEditorMapData::InitFromMusicFile(std::wstring path)
	{
		std::wstring chooseWorkingFileResult = ChooseWorkingFile();
		if(chooseWorkingFileResult==L"OK")
		{


			divacore::MapInfo contructMapInfo;
			registerMapInfo(&contructMapInfo);

			//Need to copy file now
			contructMapInfo.header.mainSound = resource_add(path);

			divacore::MapEvent initBPM;
			initBPM.position = 0;
			initBPM.eventType = "bpm";
			initBPM.arg["value"] = 120.0;

			contructMapInfo.events.push_back(initBPM);
			
			//Init header
			contructMapInfo.header.BPM = 120.0;
			contructMapInfo.header.difficulty = 0;
			contructMapInfo.header.hardLevel = 0;
			contructMapInfo.header.mapName = L"no name";
			contructMapInfo.header.speedScale = 1;
			contructMapInfo.header.versionStr = EditorVer;

			//Init module
			contructMapInfo.module = "standard";

			//Init editor default value
			ResetEditorMapData();
			
			if(!SaveFile())
			{
				std::string soundID = coreInfoPtr->header.mainSound = "";
				resource_delete(soundID);
				bpm_delete(0);

				return L"Save diva project file error.";
			}

			registerMapInfo(NULL);
			CORE_PTR->setSong(workingDirectory,workingDivaOLFile);
			return L"OK";
		}
		else
			return chooseWorkingFileResult;
		
	}
	

}

