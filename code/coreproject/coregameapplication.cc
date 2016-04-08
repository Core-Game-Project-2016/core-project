//------------------------------------------------------------------------------
//  Coregameapplication.cc
//  (C) 2011-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsprotocol.h"
#include "messaging/message.h"
#include "gamestates/coregamestate.h"
#include "physicsfeature/physicsprotocol.h"
#include "graphicsfeature/graphicsfeatureprotocol.h"
#include "basegamefeature/basegameprotocol.h"
#include "scriptingfeature/scriptingcommands.h"
#include "Coregameapplication.h"
#include "coregraphics/displaymode.h"
#include "ui/uicommands.h"
#include <nidl/NetworkCommands.h>


namespace Tools
{
using namespace Graphics;
using namespace Math;
using namespace Input;
using namespace BaseGameFeature;
using namespace Game;

//------------------------------------------------------------------------------
/**
*/
CoreProjectApplication::CoreProjectApplication(void)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
CoreProjectApplication::~CoreProjectApplication(void)
{
	if (this->IsOpen())
	{
		this->Close();
	}
}

//------------------------------------------------------------------------------
/**
*/
bool CoreProjectApplication::Open()
{
	n_assert(!this->IsOpen());
	if (GameApplication::Open())
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void CoreProjectApplication::Close()
{
	GameApplication::Close();
}


//------------------------------------------------------------------------------
/**
*/
void 
CoreProjectApplication::SetupStateHandlers()
{
	Ptr<CoreGameState> gameState = CoreGameState::Create();
	gameState->SetSetupMode(CoreGameState::LoadLevel);
	gameState->SetName("CoreState");
	// select the level to be loaded explicitly instead of the default one
	gameState->SetLevelName("menu_level");

	this->AddStateHandler(gameState.get());

	this->SetState("CoreState");


}

//------------------------------------------------------------------------------
/**
*/
void 
CoreProjectApplication::SetupGameFeatures()
{
	GameApplication::SetupGameFeatures();

	// create and attach default graphic features
	this->graphicsFeature = GraphicsFeature::GraphicsFeatureUnit::Create();
	this->graphicsFeature->SetCmdLineArgs(this->GetCmdLineArgs());
	// disable resizing of window
    this->graphicsFeature->SetResizeable(false);
	// enable debug rendering of graphics
	this->graphicsFeature->SetRenderDebug(true);
	this->graphicsFeature->SetupDisplay();

	// game features
	this->baseGameFeature = BaseGameFeatureUnit::Create();
	this->baseGameFeature->SetRenderDebug(true);

	// create fx feature
	this->fxFeature = EffectsFeature::EffectsFeatureUnit::Create();
		
	// create and attach core features
	this->physicsFeature = PhysicsFeature::PhysicsFeatureUnit::Create(); 
	this->physicsFeature->SetRenderDebug(true);

	// setup scripting
	this->scriptingFeature = ScriptingFeature::ScriptingFeatureUnit::Create();

	// setup ui
	this->uiFeature = UI::UiFeatureUnit::Create();

	// setup posteffect
	this->postEffectFeature = PostEffect::PostEffectFeatureUnit::Create();

	// setup multiplayerfeature
	this->multiplayerFeature = MultiplayerFeature::MultiplayerFeatureUnit::Create();

	this->networkGame = MultiplayerFeature::NetworkGame::Create();
	this->networkGame->SetGameID(App::Application::Instance()->GetAppID());
	this->player = MultiplayerFeature::NetworkPlayer::Create();
	this->multiplayerFeature->SetPlayer(this->player);
	// must come before basegamefeature!
	this->gameServer->AttachGameFeature(this->multiplayerFeature.cast<Game::FeatureUnit>());

	this->gameServer->AttachGameFeature(this->graphicsFeature.cast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());	
	this->gameServer->AttachGameFeature(this->fxFeature.upcast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->scriptingFeature.upcast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->uiFeature.cast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->postEffectFeature.cast<Game::FeatureUnit>());

	this->multiplayerFeature->Setup(MultiplayerFeature::MultiplayerFeatureUnit::LAN);

	Commands::ScriptingCommands::Register();
	Commands::NetworkCommands::Register();
	Commands::PhysicsProtocol::Register();
	Commands::GraphicsFeatureProtocol::Register();
	Commands::BaseGameProtocol::Register();
	Commands::UICommands::Register();

	this->mainLayout = uiFeature->GetLayout("main_menu");
	this->mainLayout->Show();

	
#if !PUBLIC_BUILD
	this->uiFeature->SetRenderDebug(true);	
#endif

	// modify resolution
	Ptr<UpdateDisplay> upd = UpdateDisplay::Create();
	upd->SetFullscreen(true);
	CoreGraphics::DisplayMode mode;
	mode.SetWidth(1920);
	mode.SetHeight(1200);
	mode.SetAspectRatio(1920 / float(1200));
	upd->SetTripleBufferingEnabled(true);
	upd->SetDisplayMode(mode);
	// uncomment for setting fullscreen
	//GraphicsInterface::Instance()->Send(upd.upcast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreProjectApplication::CleanupGameFeatures()
{
	this->mainLayout = 0;
	this->player = 0;
	this->gameServer->RemoveGameFeature(this->postEffectFeature.upcast<Game::FeatureUnit>());
	this->postEffectFeature = 0;
	this->gameServer->RemoveGameFeature(this->fxFeature.upcast<Game::FeatureUnit>());
	this->gameServer->RemoveGameFeature(this->multiplayerFeature.upcast<Game::FeatureUnit>());
	this->multiplayerFeature = 0;
	this->networkGame->Close();
	this->networkGame = 0;
	this->fxFeature = 0;
	this->gameServer->RemoveGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());
	this->baseGameFeature = 0;
	this->gameServer->RemoveGameFeature(this->uiFeature.upcast<Game::FeatureUnit>());
	this->uiFeature = 0;
	this->gameServer->RemoveGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
	this->physicsFeature = 0;
	this->gameServer->RemoveGameFeature(this->scriptingFeature.upcast<Game::FeatureUnit>());
	this->scriptingFeature = 0;
	this->gameServer->RemoveGameFeature(this->graphicsFeature.upcast<Game::FeatureUnit>());
	this->graphicsFeature = 0;

	GameApplication::CleanupGameFeatures();
}
}
