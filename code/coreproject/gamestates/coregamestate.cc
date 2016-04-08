//------------------------------------------------------------------------------
//  CoreGameState.cc
//  (C) 2013-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "Coregamestate.h"
#include "math/matrix44.h"
#include "graphicsfeatureunit.h"
#include "input/keyboard.h"
#include "effects/effectsfeatureunit.h"
#include "coreproject/coregameapplication.h"

namespace Tools
{
__ImplementClass(Tools::CoreGameState, 'CMGS', BaseGameFeature::GameStateHandler);

using namespace BaseGameFeature;
using namespace GraphicsFeature;
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
CoreGameState::CoreGameState()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
CoreGameState::~CoreGameState()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreGameState::OnStateEnter( const Util::String& prevState )
{
	GameStateHandler::OnStateEnter(prevState);	

	//const Ptr<UI::UiLayout>& layout = UI::UiFeatureUnit::Instance()->GetLayout("main_menu");
	//Ptr<UI::UiElement> element = layout->GetElement("updatetext");
	//return element->GetText();
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreGameState::OnStateLeave( const Util::String& nextState )
{
	GameStateHandler::OnStateLeave(nextState);
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
CoreGameState::OnFrame()
{
	//handle all user input
	if (Input::InputServer::HasInstance())
	{
		this->HandleInput();
	}
		
	return GameStateHandler::OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreGameState::OnLoadBefore()
{
	
}

//------------------------------------------------------------------------------
/**
*/
void 
CoreGameState::OnLoadAfter()
{

}

//------------------------------------------------------------------------------
/**
*/
void 
CoreGameState::HandleInput()
{
	const Ptr<Input::Keyboard>& kbd = Input::InputServer::Instance()->GetDefaultKeyboard();

	// reload layout if key gets pressed
	if (kbd->KeyDown(Input::Key::F1))
	{
        const Ptr<UI::UiLayout>& layout = UI::UiFeatureUnit::Instance()->GetLayout("main_menu");
        layout->Reload();
	}
	if(kbd->KeyDown(Input::Key::X))
	{
		CoreProjectApplication::Instance()->RequestState("Exit");
	}
	if (kbd->KeyDown(Input::Key::F2))
	{
		EffectsFeature::EffectsFeatureUnit::Instance()->EmitGraphicsEffect(Math::matrix44::translation(n_rand(-5, 5), 10, n_rand(-5, 5)), "mdl:particles/newparticle.n3", 10.0f);
	}
	if (kbd->KeyDown(Input::Key::G))
	{
		MultiplayerFeature::NetworkGame::Instance()->UpdateRoomList();
		CoreProjectApplication::Instance()->FindStateHandlerByName("CoreState").cast<BaseGameFeature::GameStateHandler>()->SetLevelName("demo_full");
		CoreProjectApplication::Instance()->FindStateHandlerByName("CoreState").cast<BaseGameFeature::GameStateHandler>()->SetSetupMode(BaseGameFeature::GameStateHandler::LoadNetworkedLevel);
		CoreProjectApplication::Instance()->RequestState("CoreState");
	}

	if (kbd->KeyDown(Input::Key::A))
	{
		MultiplayerFeature::NetworkGame::Instance()->CreateRoom();
	}
	if (kbd->KeyDown(Input::Key::S))
	{
		MultiplayerFeature::NetworkServer::Instance()->SearchForGames();
	}
	if (kbd->KeyDown(Input::Key::D))
	{
		Util::String test = "130.240.54.238";
		MultiplayerFeature::NetworkGame::Instance()->JoinRoom(test + "|61111", true);
	}
}
} // namespace Tools