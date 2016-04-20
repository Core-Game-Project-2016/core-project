#pragma once
//------------------------------------------------------------------------------
/**
@class NotA::NotANetworkGame

(C) 2015 Individual contributors, see AUTHORS file
*/

#include "stdneb.h"
#include "networkgame.h"
// #include "notanetworkplayer.h"
#include "multiplayer/bitwriter.h"
#include "multiplayer/bitreader.h"
#include "messaging/message.h"
#include "ui/uielement.h"
#include "util/ringbuffer.h"

//#define GETLOCALPLAYER (MultiplayerFeature::MultiplayerFeatureUnit::Instance()->GetPlayer().downcast<NotANetworkPlayer>())

//------------------------------------------------------------------------------
namespace CoreG
{
	class CoreNetworkGame : public MultiplayerFeature::NetworkGame
{
	__DeclareClass(CoreNetworkGame);
	__DeclareInterfaceSingleton(CoreNetworkGame);
public:

	/// constructor
	CoreNetworkGame();
	/// destructor
	~CoreNetworkGame();


	bool CanJoinInGame() override;

	/// receives the master servers room list
	virtual void OnReceiverMasterList();
	/// called when successfully joined a room
	virtual void OnJoinedRoom();
	
	/// we received a message
	virtual void OnHandleMessage(const Ptr<Messaging::Message> &msg);
	/// called when player joins room
	virtual void OnPlayerJoined(const Ptr<MultiplayerFeature::NetworkPlayer>& player);

	///
	void JoinServerEntry(int entry);
    ///
    void ConnectToIP(const Util::String & ip);

	/// game started
	virtual void OnGameStarted();
	
	/// update player name
	void UpdatePlayerName(const Util::String & name);

	/// overload to add own variables to the serialization stream when creating game object
	virtual void SerializeConstruction(const Ptr<Multiplayer::BitWriter> & writer);
	/// overload to add own variables to the deserialization stream when creating game object
	virtual void DeserializeConstruction(const Ptr<Multiplayer::BitReader> & reader);

	/// overload to add own variables to the serialization stream
	virtual void Serialize(const Ptr<Multiplayer::BitWriter> & writer);
	/// overload to add own variables to the deserialization stream
	virtual void Deserialize(const Ptr<Multiplayer::BitReader> & reader);
	///
	void CancelSession();

	///
	void UpdateChat(const Util::String & msg);

	///
	bool HasWon();

	///
	void AddChatMessage(const Util::String & sender, const Util::String & body);

	///
	void UpdatePlayerReady(int id, bool val);

	///
	void SetWinner(int team);

	///
	void ClearServerList();
	Util::Array<Ptr<UI::UiElement>> serverButtons;

protected:
	///
	void OnAllReady(bool dummy);




private:


};


}
