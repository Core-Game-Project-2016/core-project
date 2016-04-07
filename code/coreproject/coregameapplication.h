#pragma once
#include "graphicsfeatureunit.h"
#include "physicsfeatureunit.h"
#include "basegamefeatureunit.h"
#include "appgame/gameapplication.h"
#include "scriptingfeature/scriptingfeature.h"
#include "ui/uifeatureunit.h"
#include "effects/effectsfeatureunit.h"
#include "posteffect/posteffectfeatureunit.h"
#include <multiplayer/networkgame.h>
#include <multiplayer/multiplayerfeatureunit.h>

//------------------------------------------------------------------------------
/**
    @class Tools::CoreProjectApplication
    
    NebulaT Core project application.

    (C) 2011-2013 Individual contributors, see AUTHORS file
*/
namespace  Tools
{
class CoreProjectApplication : public App::GameApplication
{
public:
	/// constructor
	CoreProjectApplication(void);
	/// destructor
	virtual ~CoreProjectApplication(void);
	/// open application
	bool Open() override;
	/// close application
	void Close() override;

private:

	/// setup application state handlers
	void SetupStateHandlers() override;
	/// setup game features
	void SetupGameFeatures() override;
	/// cleanup game features
	void CleanupGameFeatures() override;

	Ptr<UI::UiFeatureUnit> uiFeature;
	Ptr<UI::UiLayout> mainLayout;
	Ptr<PhysicsFeature::PhysicsFeatureUnit> physicsFeature;
	Ptr<EffectsFeature::EffectsFeatureUnit> fxFeature;
	Ptr<GraphicsFeature::GraphicsFeatureUnit> graphicsFeature;
	Ptr<BaseGameFeature::BaseGameFeatureUnit> baseGameFeature;
	Ptr<ScriptingFeature::ScriptingFeatureUnit> scriptingFeature;	
	Ptr<PostEffect::PostEffectFeatureUnit> postEffectFeature;
	Ptr<MultiplayerFeature::MultiplayerFeatureUnit> multiplayerFeature;
	Ptr<MultiplayerFeature::NetworkGame> networkGame;
	Ptr<MultiplayerFeature::NetworkPlayer> player;

};

}
