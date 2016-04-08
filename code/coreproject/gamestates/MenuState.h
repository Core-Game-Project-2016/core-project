#pragma once
//------------------------------------------------------------------------------
/**
    @class Tools::CoreGameState
    
    A basic game state
    
    (C) 2013-2014 Individual contributors, see AUTHORS file
*/
#include "statehandlers/gamestatehandler.h"

//------------------------------------------------------------------------------
namespace Tools
{
class MenuState : public BaseGameFeature::GameStateHandler
{
	__DeclareClass(MenuState);
public:
	/// constructor
	MenuState();
	/// destructor
	virtual ~MenuState();

	/// called when the state represented by this state handler is entered
	void OnStateEnter(const Util::String& prevState) override;
	/// called when the state represented by this state handler is left
	void OnStateLeave(const Util::String& nextState) override;
	/// called each frame as long as state is current, return new state
	Util::String OnFrame() override;
	/// called after Db is opened, and before entities are loaded
	void OnLoadBefore() override;
	/// called after entities are loaded
	void OnLoadAfter() override;

	// handle all user input; called @ LevelEditorState::OnFrame()
	void HandleInput();

private:
	
}; 
} // namespace Tools
//------------------------------------------------------------------------------