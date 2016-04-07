#include "stdneb.h"
#include "characterPhysicsProperty.h"
#include "basegamefeature/basegameprotocol.h"
#include "physics/physicsserver.h"
#include "graphicsfeature/graphicsfeatureunit.h"
#include "basegametiming/gametimesource.h"
#include "math/polar.h"
#include "physicsfeature/physicsprotocol.h"

namespace PhysicsFeature
{
	__ImplementClass(PhysicsFeature::CharacterPhysicsProperty, 'CRPO', PhysicsFeature::PhysicsProperty);

	using namespace Game;
	using namespace Messaging;
	using namespace Timing;
	using namespace Math;
	using namespace Util;
	using namespace BaseGameFeature;


	CharacterPhysicsProperty::CharacterPhysicsProperty() :
		headingGain(-6.0f),
		positionGain(-25.0f),
		disableProcessing(false),
		smoothingEnabled(false)
	{
	
	}

	CharacterPhysicsProperty::~CharacterPhysicsProperty()
	{
		// empty
	}
	void 
	CharacterPhysicsProperty::SetupCallbacks()
	{
		if (this->IsSimulationHost())
		{
			//this->entity->RegisterPropertyCallback(this, MoveBefore);
			this->entity->RegisterPropertyCallback(this, MoveAfter);
			this->entity->RegisterPropertyCallback(this, LoseActivity);
		}
		this->entity->RegisterPropertyCallback(this, BeginFrame);
	}

	void
	CharacterPhysicsProperty::OnActivate()
	{
		PhysicsProperty::OnActivate();
		this->smoothingEnabled = this->entity->GetBool(Attr::AngularSmoothing);

	}

	void
	CharacterPhysicsProperty::OnDeactivate()
	{
		PhysicsProperty::OnDeactivate();
	}

	//------------------------------------------------------------------------------
	/**
	Creates a Physics::CharEntity instead of a normal Physics::PhysicsEntity.
	*/
	void
	CharacterPhysicsProperty::EnablePhysics()
	{
		if (this->IsSimulationHost())
		{
			n_assert(!this->IsEnabled());
			this->disableProcessing = false;

			// create a char physics entity
			this->charPhysicsEntity = Physics::Character::Create();
			this->charPhysicsEntity->SetUserData(this->GetEntity().upcast<Core::RefCounted>());

			if (this->GetEntity()->HasAttr(Attr::Mass))
			{
				float mass = this->GetEntity()->GetFloat(Attr::Mass);
				n_assert_fmt(mass > 0.0f, "Character mass cant be 0!\nEntity Id: %s\n", this->entity->GetString(Attr::Id).AsCharPtr());
				this->charPhysicsEntity->SetWeight(mass);
			}
			else
			{
				this->charPhysicsEntity->SetWeight(1.0f);
			}
			if (this->GetEntity()->HasAttr(Attr::CapsuleRadius))
			{
				this->charPhysicsEntity->SetRadius(this->GetEntity()->GetFloat(Attr::CapsuleRadius));
			}
			if (this->GetEntity()->HasAttr(Attr::CapsuleHeight))
			{
				this->charPhysicsEntity->SetHeight(this->GetEntity()->GetFloat(Attr::CapsuleHeight));
				n_assert_fmt(this->charPhysicsEntity->GetHeight() > 0.0f, "Character height cant be 0!\nEntity Id: %s\n", this->entity->GetString(Attr::Id).AsCharPtr());
			}
			if (this->GetEntity()->HasAttr(Attr::CrouchingCapsuleHeight))
			{
				this->charPhysicsEntity->SetCrouchingHeight(this->GetEntity()->GetFloat(Attr::CrouchingCapsuleHeight));
			}
			else
			{
				this->charPhysicsEntity->SetCrouchingHeight(this->charPhysicsEntity->GetHeight());
			}

			if (this->GetEntity()->HasAttr(Attr::CharacterShape))
			{
				Util::String shapeString = this->GetEntity()->GetString(Attr::CharacterShape);
				shapeString.ToLower();

				if (shapeString.IsValid())
				{
					Physics::Character::CharacterShape shape;

					if ("capsule" == shapeString)
					{
						shape = Physics::Character::Capsule;
					}
					else if ("cylinder" == shapeString)
					{
						shape = Physics::Character::Cylinder;
					}
					else
					{
						n_error("Unrecognized shape '%s'!", shapeString.AsCharPtr());
					}

					this->charPhysicsEntity->SetShape(shape);
				}
			}

			this->charPhysicsEntity->SetTransform(this->GetEntity()->GetMatrix44(Attr::Transform));
			// attach physics entity to physics level
			const Ptr<Physics::Scene>& physicsLevel = Physics::PhysicsServer::Instance()->GetScene();
			n_assert(physicsLevel);

			this->charPhysicsEntity->SetMaxJumpHeight(this->GetEntity()->GetFloat(Attr::JumpHeight));

			physicsLevel->Attach(this->charPhysicsEntity.upcast<Physics::PhysicsObject>());


			this->charPhysicsEntity->SetJumpSpeed(this->GetEntity()->GetFloat(Attr::JumpSpeed));

			this->charPhysicsEntity->SetMovementSpeed(this->GetEntity()->GetFloat(Attr::MaxVelocity));
			this->charPhysicsEntity->SetMaxLinearAcceleration(this->GetEntity()->GetFloat(Attr::RelVelocity));

			if (this->GetEntity()->HasAttr(Attr::VelocityGain))
			{
				this->charPhysicsEntity->SetVelocityGain(this->GetEntity()->GetFloat(Attr::VelocityGain), this->GetEntity()->GetFloat(Attr::AirVelocityGain));
			}

			// make sure we are standing still
			this->Stop();

			// initialize feedback loops for motion smoothing
			Time time = GameTimeSource::Instance()->GetTime();
			matrix44 entityMatrix = this->GetEntity()->GetMatrix44(Attr::Transform);
			this->smoothedPosition.Reset(time, 0.001f, this->positionGain, entityMatrix.get_position());

			polar headingAngle(entityMatrix.get_zaxis());
			this->smoothedHeading.Reset(time, 0.001f, this->headingGain, headingAngle.rho);

			this->enabled = true;
			if (this->entity->HasAttr(Attr::CollisionFeedback))
			{
				this->charPhysicsEntity->SetEnableCollisionCallback(this->entity->GetBool(Attr::CollisionFeedback));
			}
		}
		else
		{
			Ptr<Physics::Collider> coll = Physics::Collider::Create();
			Math::matrix44 offset;
			float radius = this->GetEntity()->GetFloat(Attr::CapsuleRadius);
			float height = this->GetEntity()->GetFloat(Attr::CapsuleHeight);
			offset.translate(Math::vector(0, height*0.5f + 2.0f* radius, 0));
			Physics::ColliderDescription desc;
			desc.type = Physics::ColliderCapsule;
			desc.capsule.radius = radius;
			desc.capsule.height = height;
			desc.transform = offset;
			coll->AddFromDescription(desc);
			Physics::PhysicsCommon common;
			common.bodyFlags = Physics::Kinematic;
			common.collider = coll;
			common.mass = 80.0f;
			common.type = Physics::PhysicsBody::RTTI.GetFourCC();
			this->physicsEntity = Physics::PhysicsObject::CreateFromTemplate(common).cast<Physics::PhysicsBody>();
			Physics::PhysicsServer::Instance()->GetScene()->Attach(this->physicsEntity.cast<Physics::PhysicsObject>());

			this->physicsEntity->SetUserData(this->entity.cast<Core::RefCounted>());
			this->enabled = true;
		}
	}

	void
	CharacterPhysicsProperty::DisablePhysics()
	{
		n_assert(this->IsEnabled());

		if (this->IsSimulationHost())
		{
			// stop
			this->Stop();

			// remove from level
			const Ptr<Physics::Scene>& physicsLevel = Physics::PhysicsServer::Instance()->GetScene();
			n_assert(physicsLevel);
			physicsLevel->Detach(this->charPhysicsEntity.upcast<Physics::PhysicsObject>());

			// cleanup resource
			this->charPhysicsEntity = 0;
		}
		// call parent
		PhysicsProperty::DisablePhysics();
	}

	void
	CharacterPhysicsProperty::SetupAcceptedMessages()
	{
		if (this->IsSimulationHost())
		{
			this->RegisterMessage(MoveDirection::Id);
			this->RegisterMessage(MoveStop::Id);
			this->RegisterMessage(SetTransform::Id);
			this->RegisterMessage(MoveTurn::Id);
			this->RegisterMessage(MoveSetVelocity::Id);
			this->RegisterMessage(MoveRotate::Id);
			this->RegisterMessage(MoveJump::Id);
			this->RegisterMessage(GetPhysicsObject::Id);
			this->RegisterMessage(PlayerDeath::Id);
			this->RegisterMessage(PlayerEnable::Id);
			this->RegisterMessage(Crouch::Id);
			PhysicsProperty::SetupAcceptedMessages();
		}
		else
		{
			this->RegisterMessage(Crouch::Id);
			TransformableProperty::SetupAcceptedMessages();
		}
	}

	void
	CharacterPhysicsProperty::HandleMessage(const Ptr<Messaging::Message>& msg)
	{

		if (msg->CheckId(PlayerEnable::Id))
		{
			this->disableProcessing = !(msg.cast<PlayerEnable>())->GetEnabled();
		}
		if (!this->IsEnabled() || this->disableProcessing || !this->IsSimulationHost())
		{
			// disabled, don't listen to messages
			PhysicsProperty::HandleMessage(msg);
		}
		else if (msg->CheckId(MoveDirection::Id))
		{
			this->HandleMoveDirection(static_cast<MoveDirection*>(msg.get()));
		}
		else if (msg->CheckId(MoveStop::Id))
		{
			this->Stop();
		}
		else if (msg->CheckId(SetTransform::Id))
		{
			this->HandleSetTransform(static_cast<SetTransform*>(msg.get()));
		}
		else if (msg->CheckId(MoveTurn::Id))
		{
			this->HandleMoveTurn(static_cast<MoveTurn*>(msg.get()));
		}
		else if (msg->CheckId(MoveRotate::Id))
		{
			this->HandleMoveRotate(static_cast<MoveRotate*>(msg.get()));
		}
		else if (msg->CheckId(GetPhysicsObject::Id))
		{
			static_cast<PhysicsFeature::GetPhysicsObject*>(msg.get())->SetObject(this->GetPhysicsEntity().upcast<Physics::PhysicsObject>());
			msg->SetHandled(true);
		}
		else if (msg->CheckId(MoveSetVelocity::Id))
		{
			this->GetEntity()->SetFloat(Attr::RelVelocity, static_cast<MoveSetVelocity*>(msg.get())->GetRelVelocity());
		}
		else if (msg->CheckId(MoveJump::Id))
		{
			charPhysicsEntity->Jump();
		}
		else if (msg->CheckId(Crouch::Id))
		{
			charPhysicsEntity->SetCrouching(msg.downcast<Crouch>()->GetEnable());
		}
		else if (msg->CheckId(PlayerDeath::Id))
		{
			this->Stop();
			this->disableProcessing = true;
		}
		else
		{
			PhysicsProperty::HandleMessage(msg);
		}
	}

	//------------------------------------------------------------------------------
	/**
	The OnMoveBefore() method handles all pending messages, and other
	per-frame-stuff that must happen before the physics subsystem is
	triggered.
	*/
	void
	CharacterPhysicsProperty::OnMoveBefore()
	{
		if (this->IsEnabled())
		{

		}
	}

	//------------------------------------------------------------------------------
	/**
	The OnMoveAfter() method transfers the current physics entity transform to
	the game entity.
	*/
	void
	CharacterPhysicsProperty::OnMoveAfter()
	{
		if (this->IsEnabled())
		{
			matrix44 physicsEntityTransform = this->charPhysicsEntity->GetTransform();
			vector physicsEntityVelocity = this->charPhysicsEntity->GetLinearVelocity();

			// feed the feedback loops
			polar headingAngles(physicsEntityTransform.get_zaxis());
			this->smoothedPosition.SetGoal(physicsEntityTransform.get_position());
			this->smoothedHeading.SetGoal(headingAngles.rho);

			// if current state and goal are already close to each other,
			// we don't send a position update
			// NOTE: this check must be made before the feedback look update!
			vector posError = this->smoothedPosition.GetState() - this->smoothedPosition.GetGoal();
			float headingError = n_abs(this->smoothedHeading.GetState() - this->smoothedHeading.GetGoal());

			// always evaluate the feedback loops to keep them in valid time delta
			Time time = GameTimeSource::Instance()->GetTime();
			this->smoothedPosition.Update(time);
			this->smoothedHeading.Update(time);

			// only send update transform if anything changed

			//   if ((posError.length() > 0.001f) || (headingError > 0.001f))
			{
				// construct the new entity matrix
				matrix44 entityMatrix;
				if (this->smoothingEnabled)
				{
					entityMatrix = matrix44::rotationy(this->smoothedHeading.GetState());
					entityMatrix.translate(this->smoothedPosition.GetState());
				}
				else
				{
					entityMatrix = matrix44::rotationy(this->smoothedHeading.GetGoal());
					entityMatrix.translate(this->smoothedPosition.GetGoal());
				}
				// update game entity
				Ptr<UpdateTransform> msg = UpdateTransform::Create();
				msg->SetMatrix(entityMatrix);
				this->GetEntity()->SendSync(msg.upcast<Message>());
				this->GetEntity()->SetFloat4(Attr::VelocityVector, physicsEntityVelocity);
			}

			this->charPhysicsEntity->OnFrameAfter();
		}
	}

	//------------------------------------------------------------------------------
	/**
	This simply sends a synchronous stop message to myself. This uses
	a message so that everybody else who might be interested in the information
	that I have stopped can listen to the message.
	*/
	void
	CharacterPhysicsProperty::SendStop() const
	{
		Ptr<MoveStop> msg = MoveStop::Create();
		this->GetEntity()->SendSync(msg.upcast<Message>());
	}

	//------------------------------------------------------------------------------
	/**
	Immediately stop the entity.

	26-Jan-06   floh    bugfix: also cancelled MoveFollow
	14-Feb-06   nico    bugfix: now really cancelled MoveFollow ;)
	*/
	void
	CharacterPhysicsProperty::Stop() const
	{
		this->charPhysicsEntity->SetMotionVector(vector(0.0f, 0.0f, 0.0f));
		this->GetEntity()->SetBool(Attr::Moving, false);
		this->GetEntity()->SetBool(Attr::Following, false);
		this->GetEntity()->SetFloat4(Attr::VelocityVector, vector(0.0f, 0.0f, 0.0f));
	}

	//------------------------------------------------------------------------------
	/**
	Handle a MoveDirection message.
	*/
	void
	CharacterPhysicsProperty::HandleMoveDirection(MoveDirection* msg) const
	{
		n_assert(msg);

		vector dir = msg->GetDirection();

		// convert camera relative vector into absolute vector if necessary
		if (msg->GetCameraRelative())
		{
			const Ptr<Graphics::View>& curView = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultView();
			Graphics::CameraEntity* camera = curView->GetCameraEntity();
			n_assert(camera);
			matrix44 camTransform = camera->GetTransform();
			camTransform.set_position(float4(0.0f, 0.0f, 0.0f, 1.0f));
			dir = matrix44::transform(dir, camTransform);
		}
		dir.y() = 0.0f;
		dir = vector::normalize(dir);

		/*
		this->AutoEvade(dir);
		dir = vector::normalize(dir);
		*/

		vector desiredVelocity = dir * this->GetEntity()->GetFloat(Attr::RelVelocity) * this->GetEntity()->GetFloat(Attr::MaxVelocity);
		if (desiredVelocity.length() >  msg->GetMaxMovement())
		{
			desiredVelocity = Math::float4::normalize(desiredVelocity) *  msg->GetMaxMovement();
		}
		this->charPhysicsEntity->SetMotionVector(desiredVelocity);
		this->GetEntity()->SetBool(Attr::Moving, true);
	}

	//------------------------------------------------------------------------------
	/**
	Handle a MoveTurn message.
	*/
	void
	CharacterPhysicsProperty::HandleMoveTurn(MoveTurn* msg) const
	{
		n_assert(msg);
		vector dir = msg->GetDirection();

		if (float4::equal3_all(dir, float4(0, 0, 0, 0)))
		{
			return;
		}

		if (msg->GetCameraRelative())
		{
			const Ptr<Graphics::View>& curView = GraphicsFeature::GraphicsFeatureUnit::Instance()->GetDefaultView();
			Graphics::CameraEntity* camera = curView->GetCameraEntity();
			n_assert(camera);
			matrix44 camTransform = camera->GetTransform();
			camTransform.set_position(float4(0.0f, 0.0f, 0.0f, 1.0f));
			dir = matrix44::transform(dir, camTransform);
		}
		dir.y() = 0.0f;
		dir = vector::normalize(dir);

		matrix44 newTransform = charPhysicsEntity->GetTransform();
		newTransform.set_xaxis(vector::cross3(newTransform.get_yaxis(), dir));
		newTransform.set_zaxis(vector::cross3(newTransform.get_xaxis(), newTransform.get_yaxis()));
		this->charPhysicsEntity->SetTransform(newTransform);
	}

	//------------------------------------------------------------------------------
	/**
	Handle a MoveTurn message.
	*/
	void
	CharacterPhysicsProperty::HandleMoveRotate(MoveRotate* msg) const
	{
		n_assert(msg);

		float angle = msg->GetAngle();
		charPhysicsEntity->SetTransform(matrix44::multiply(matrix44::rotationy(angle), charPhysicsEntity->GetTransform()));
	}

	//------------------------------------------------------------------------------
	/**
	Handle a SetTransform message.
	*/
	void
	CharacterPhysicsProperty::HandleSetTransform(SetTransform* msg)
	{
		n_assert(msg);
		this->charPhysicsEntity->SetTransform(msg->GetMatrix());

		// reset the feedback loops 
		Time time = GameTimeSource::Instance()->GetTime();
		matrix44 entityMatrix = this->GetEntity()->GetMatrix44(Attr::Transform);
		this->smoothedPosition.Reset(time, 0.001f, this->positionGain, entityMatrix.get_position());

		polar headingAngle(entityMatrix.get_zaxis());
		this->smoothedHeading.Reset(time, 0.001f, this->headingGain, headingAngle.rho);
	}

	//------------------------------------------------------------------------------
	/**
	*/

	const Ptr<Physics::Character>&
	CharacterPhysicsProperty::GetPhysicsEntity() const
	{
		return this->charPhysicsEntity;
	}


	//------------------------------------------------------------------------------
	/**
	*/
	void
	CharacterPhysicsProperty::OnLoseActivity()
	{
		if (this->IsEnabled())
		{
			this->charPhysicsEntity->SetMotionVector(vector(0.0f, 0.0f, 0.0f));
		}
	}
}