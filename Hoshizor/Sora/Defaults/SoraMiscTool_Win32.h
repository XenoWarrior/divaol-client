#ifndef SORA_MISCTOOL_WIN32
#define SORA_MISCTOOL_WIN32

#include "SoraStringConv.h"
#include "SoraPlatform.h"
#include "SoraStringTokenlizer.h"
#include "SoraDefaultMiscTool.h"

#ifdef OS_WIN32

namespace sora {

class SoraMiscToolWin32: public SoraDefaultMiscTool {
public:
	SoraMiscToolWin32();

	int32 messageBox(const StringType& sMessage, const StringType& sTitle, int32 msgCode);

	void setMainWindowHandle(SoraHandle mainWindowHandle);
	
	StringType fileOpenDialog(const char* filter = NULL, const char* defaultPath = NULL);
	
	StringType fileSaveDialog(const char* filter = NULL, const char* defaultPath = NULL, const char* defaultExt = NULL);
    
    uint32 getProcessorSpeed() const;
	StringType getOSVersion() const;
    uint64 getSystemMemorySize() const;

private:
	HWND _hWnd;
};

} // namespace sora

#endif // OS_WIN32

#endif // SORA_MISCTOOL_WIN32