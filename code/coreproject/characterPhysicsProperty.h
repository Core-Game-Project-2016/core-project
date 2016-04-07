#pragma once
#include "physicsfeature/properties/physicsproperty.h"
#include "basegamefeature/basegameprotocol.h"

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
		void SetupCallbacks() override;
		void OnActivate() override;
		void OnDeactivate() override;
		void EnablePhysics() override;
		void DisablePhysics() override;
		void SetupAcceptedMessages() override;
		void HandleMessage(const Ptr<Messaging::Message>& msg) override;
		void OnMoveBefore() override;
		void OnMoveAfter() override;
		void SendStop() const;
		void Stop() const;
		void HandleMoveDirection(BaseGameFeature::MoveDirection* msg) const;
		void HandleMoveTurn(BaseGameFeature::MoveTurn* msg) const;
		void HandleMoveRotate(BaseGameFeature::MoveRotate* msg) const;
		void HandleSetTransform(BaseGameFeature::SetTransform* msg);
		const Ptr<Physics::Character>& GetPhysicsEntity() const;
		void OnLoseActivity() override;
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
