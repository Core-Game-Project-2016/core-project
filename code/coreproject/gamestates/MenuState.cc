//------------------------------------------------------------------------------
//  CoreGameState.cc
//  (C) 2013-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "MenuState.h"
#include "graphicsfeatureunit.h"
#include "input/keyboard.h"
#include "coreproject/coregameapplication.h"

namespace Tools
{
	__ImplementClass(Tools::MenuState, 'MMSt', BaseGameFeature::GameStateHandler);

using namespace BaseGameFeature;
using namespace GraphicsFeature;
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
MenuState::MenuState()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
MenuState::~MenuState()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
MenuState::OnStateEnter(const Util::String& prevState)
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
MenuState::OnStateLeave(const Util::String& nextState)
{
	GameStateHandler::OnStateLeave(nextState);
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
MenuState::OnFrame()
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
MenuState::OnLoadBefore()
{
	
}

//------------------------------------------------------------------------------
/**
*/
void 
MenuState::OnLoadAfter()
{

}

//------------------------------------------------------------------------------
/**
*/
void 
MenuState::HandleInput()
{
	const Ptr<Input::Keyboard>& kbd = Input::InputServer::Instance()->GetDefaultKeyboard();
	

}
} // namespace Tools