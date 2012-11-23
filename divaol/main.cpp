//#define NET
//#define EDIT

#include <winsock2.h>

#include "SoraPlatform.h"

#include "app/SoraGameApp.h"
#include "Utility/DivaInitialize.h"
#include "divaeditor/EditorInitialize.h"

#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")


#ifdef OS_WIN32
int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
  int nCmdShow
) {
#else
    int main(int argc, const char** argv) {
        
#endif

	try
	{
		///////////////////////////////
		AUTH_CLIENT.setNetworkSystem(new divanet::TCPGNetworkSystem);
		CHAT_CLIENT.setNetworkSystem(new divanet::TCPGNetworkSystem);
		SCHEDULER_CLIENT.setNetworkSystem(new divanet::TCPGNetworkSystem);
		STAGE_CLIENT.setNetworkSystem(new divanet::TCPGNetworkSystem);
		///////////////////////////////

		divacore::Config config;
		divacore::configloader::loadWithJson(config,"system/common.json");

		sora::SoraGameAppDef def;
		sora::SoraGameApp app(def);

		divaeditor::EditorPtr editor = divaeditor::standard::initialize();

		app.addState(editor, "editor");

		def.width(config.getAsInt("windowWidth"));
		def.height(config.getAsInt("windowHeight"));

		app.run("editor");

		//sora::SoraTaskManager::defaultManager(true).joinAll();
	}
	catch (divacore::Exception&ev)
	{
		LOGGER->error(ev);
		MessageBox(
			NULL,
			sora::s2ws(ev.getContent()).c_str(),
			sora::s2ws(ev.getModuleName()).c_str(),
			MB_ICONERROR | MB_DEFBUTTON2
			);
        //sora::SoraCore::Instance()->messageBox(ev.getContent(), ev.getModuleName(), 0);
	}

	//ExitProcess(0);

	return 0;

}