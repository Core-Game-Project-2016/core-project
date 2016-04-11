//------------------------------------------------------------------------------
//  CoreGameState.cc
//  (C) 2013-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "Coregamestate.h"
#include "graphicsfeatureunit.h"
#include "input/keyboard.h"
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
	this->focusset = false;
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

void CoreGameState::OnNetworkStarted()
{
	
}

	//------------------------------------------------------------------------------
/**
*/
void 
CoreGameState::HandleInput()
{
	const Ptr<Input::Keyboard>& kbd = Input::InputServer::Instance()->GetDefaultKeyboard();
	
	if (kbd->KeyDown(Input::Key::H))
	{
		this->player = FactoryManager::Instance()->CreateEntityByTemplate("Player", "dummychar");
		EntityManager::Instance()->AttachEntity(this->player);
		FocusManager::Instance()->SetCameraFocusEntity(this->player, false);
		FocusManager::Instance()->SetInputFocusEntity(this->player, false);
	}
	if (kbd->KeyDown(Input::Key::J))
	{
		if (player != NULL)
		{
			FocusManager::Instance()->SetCameraFocusEntity(this->player, false);
			FocusManager::Instance()->SetInputFocusEntity(this->player, false);
		}
	}

}
} // namespace Tools