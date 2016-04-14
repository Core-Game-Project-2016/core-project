//------------------------------------------------------------------------------
//  CoreGameState.cc
//  (C) 2013-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "Coregamestate.h"
#include "graphicsfeatureunit.h"
#include "input/keyboard.h"
#include "coreproject/coregameapplication.h"
#include "input/mouse.h"
#include "physicsfeature/physicsprotocol.h"
#include "graphics/graphicsserver.h"
#include "basegamefeature/basegameprotocol.h"

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
	const Ptr<UI::UiLayout>& layout = UI::UiFeatureUnit::Instance()->GetLayout("lobby");

	if (layout->IsShown())
		layout->Hide();
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
	const Ptr<Input::Mouse>& mouse = Input::InputServer::Instance()->GetDefaultMouse();
	
	if (mouse->ButtonUp(Input::MouseButton::LeftButton))
	{
		Math::matrix44 m, t, vT;
		t = this->player->GetMatrix44(Attr::Transform);
		Math::float4 v(0, 1, 0, 1);
		
		Ptr<Game::Entity> boll = FactoryManager::Instance()->CreateEntityByTemplate("Ball", "Bollen2", true);
		
		m.scale(Math::float4(0.1, 0.1, 0.1, 1));
		boll->SetMatrix44(Attr::Transform, m);
		vT = Graphics::GraphicsServer::Instance()->GetCurrentView()->GetCameraEntity()->GetViewTransform();
		v = vT.get_zaxis();
		Ptr<PhysicsFeature::ApplyImpulseAtPos> msg = PhysicsFeature::ApplyImpulseAtPos::Create();
		Ptr<BaseGameFeature::SetTransform> tMsg = BaseGameFeature::SetTransform::Create();
		tMsg->SetMatrix(t);
		
		
		msg->SetImpulse(v * 2);
		msg->SetMultiplyByMass(true);
		msg->SetPosition(m.get_position());
		
		EntityManager::Instance()->AttachEntity(boll);
		__SendSync(boll, tMsg);
		__SendSync(boll, msg);

	}
	
	if (kbd->KeyDown(Input::Key::H))
	{
		this->player = FactoryManager::Instance()->CreateEntityByTemplate("Player", "dummychar");
		EntityManager::Instance()->AttachEntity(this->player);
		FocusManager::Instance()->SetFocusEntity(this->player, false);
	}
	if (kbd->KeyDown(Input::Key::J))
	{
		if (player != NULL)
		{
			FocusManager::Instance()->SetFocusEntity(this->player, false);
		}
	}

}
} // namespace Tools