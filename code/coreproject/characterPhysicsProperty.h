#pragma once
#include "physicsfeature/properties/physicsproperty.h"
#include "physicsfeature/physicsprotocol.h"
#include "basegamefeature/basegameprotocol.h"
#include "game/entity.h"

#include "physics/character.h"
#include "math/pfeedbackloop.h"
#include "math/angularpfeedbackloop.h"

//------------------------------------------------------------------------------
namespace PhysicsFeature
{
	class CharacterPhysicsProperty : public PhysicsProperty
	{
		__DeclareClass(CharacterPhysicsProperty);
		__SetupExternalAttributes();
	public:
		/// constructor
		CharacterPhysicsProperty();
		/// destructor
		virtual ~CharacterPhysicsProperty();
		void SetupCallbacks();
		void OnActivate();
		void OnDeactivate();
		void EnablePhysics();
		void DisablePhysics();
		void SetupAcceptedMessages();
		void HandleMessage(const Ptr<Messaging::Message>& msg);
		void OnMoveBefore();
		void OnMoveAfter();
		void SendStop();
		void Stop();
		void HandleMoveDirection(BaseGameFeature::MoveDirection* msg);
		void HandleMoveTurn(BaseGameFeature::MoveTurn* msg);
		void HandleMoveRotate(BaseGameFeature::MoveRotate* msg);
		void HandleSetTransform(BaseGameFeature::SetTransform* msg);
		const Ptr<Physics::Character>& GetPhysicsEntity() const;
		void OnLoseActivity();
		/// called from Entity::ActivateProperties()
	protected:

		Ptr<Physics::Character> charPhysicsEntity;

		Math::PFeedbackLoop<Math::vector> smoothedPosition;
		Math::AngularPFeedbackLoop smoothedHeading;

		float headingGain;
		float positionGain;
		bool smoothingEnabled;

		bool disableProcessing;
		Math::float4 lastUpdatePos;
		Util::String physicsMaterial;
	};
	__RegisterClass(CharacterPhysicsProperty);
}

//------------------------------------------------------------------------------
