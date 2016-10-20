#include "PreDefines.h"

namespace divapomelo {
	const char* EventCode[] =  {
		"onConnect",
		"onLogin",
		"onClose",
		"onEnter",
		"onLeave",
		"onChat",
		"onKick",

		"stage.onSetMode",
		"stage.onSetHook",
		"stage.onSetSong",
		"stage.onDraw",
		"stage.onReady",
		"stage.onUnready",
		"stage.onJoin",
		"stage.onLeave",
		"stage.onCreate",
		"stage.onClose",
		"stage.onAllInfo",
		"stage.onStart",
		"stage.onReturn",
		"stage.onKick",

		"game.onStart",
		"game.onHeartbeat",
		"game.onFailure",
		"game.onRenew",
		"game.relay.onVoidPeriod",
		"game.relay.onChance",
		"game.relay.onNewPlayer",

		"connector.connectorHandler.send",
		"connector.connectorHandler.getStageList",
		"connector.connectorHandler.createStage",
		"connector.connectorHandler.joinStage",
		"connector.connectorHandler.leaveStage",
		"connector.connectorHandler.draw",
		"connector.connectorHandler.setSong",
		"connector.connectorHandler.setMode",
		"connector.connectorHandler.setHook",
		"connector.connectorHandler.ready",
		"connector.connectorHandler.start",
		"connector.connectorHandler.kick",

		"connector.connectorHandler.back",
		"connector.connectorHandler.link",
		"connector.connectorHandler.update",
		"connector.connectorHandler.over",
		"connector.connectorHandler.eval",
		"connector.connectorHandler.failure",
		"connector.connectorHandler.renew",
		"connector.connectorHandler.relayIamPlaying",
		"connector.connectorHandler.relayRelayWantToChange",
		"connector.connectorHandler.relayRelayWantToPlay",
		"connector.connectorHandler.songUI"
	};

	const char* PlayerStatusCode[] = {"load", "ready", "play", "leave", "over", "back"};
}