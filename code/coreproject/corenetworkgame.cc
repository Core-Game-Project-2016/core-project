//------------------------------------------------------------------------------
//  notanetworkgame.cc
//  (C) 2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "corenetworkgame.h"
#include "RakNetTypes.h"
#include "networkserver.h"
#include "multiplayerattrs.h"
#include "ui/uilayout.h"
#include "ui/uifeatureunit.h"
#include "basegamefeature/basegameattr/basegameattributes.h"
#include "scriptingfeature/scriptingfeature.h"
#include "multiplayerfeatureunit.h"


using namespace RakNet;
using namespace Multiplayer;

namespace CoreG
{
	__ImplementClass(CoreG::CoreNetworkGame, 'NOGA', MultiplayerFeature::NetworkGame);
	__ImplementInterfaceSingleton(CoreG::CoreNetworkGame);

//------------------------------------------------------------------------------
/**
*/
CoreNetworkGame::CoreNetworkGame()
{
	__ConstructInterfaceSingleton;

}

//------------------------------------------------------------------------------
/**
*/

CoreNetworkGame::~CoreNetworkGame()
{
	__DestructInterfaceSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::OnJoinedRoom()
{	

}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::OnHandleMessage(const Ptr<Messaging::Message> &msg)
{
	
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::OnPlayerJoined(const Ptr<MultiplayerFeature::NetworkPlayer>& player)
{

}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::OnGameStarted()
{

}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::SerializeConstruction(const Ptr<Multiplayer::BitWriter> & writer)
{

}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::DeserializeConstruction(const Ptr<Multiplayer::BitReader> & reader)
{

}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::Serialize(const Ptr<Multiplayer::BitWriter> & writer)
{

}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::Deserialize(const Ptr<Multiplayer::BitReader> & reader)
{

}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::ClearServerList()
{
	const Ptr<UI::UiLayout>& layout = UI::UiFeatureUnit::Instance()->GetLayout("serverlist");
	Ptr<UI::UiElement> basediv = layout->GetElement("serverlist1");
	for (int i = 0; i < this->serverButtons.Size(); i++)
	{
		basediv->RemoveElement(this->serverButtons[i]);
		this->serverButtons[i] = 0;
	}
	this->serverButtons.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::OnReceiverMasterList()
{
	this->ClearServerList();
	const Ptr<UI::UiLayout>& layout = UI::UiFeatureUnit::Instance()->GetLayout("serverlist");
	const Ptr<Attr::AttributeTable>& servers = this->GetMasterList();
	int rows = Math::n_min(servers->GetNumRows(), 10);
	Ptr<UI::UiElement> basediv = layout->GetElement("serverlist1");

	Util::String format;
	for (int i = 0; i < rows; i++)
	{
		format.Format("scr.connectserver(%d)", i);
		Util::Array<UI::ElementAttributeEntry> dic;
		dic.Append(UI::ElementAttributeEntry("class", "button"));
		dic.Append(UI::ElementAttributeEntry("id", "menusmall"));
		dic.Append(UI::ElementAttributeEntry("onclick", format));		
				
		Ptr<UI::UiElement> but = basediv->AddElement("button", "button", dic).cast<UI::UiElement>();
		int count = servers->GetInt(Attr::CurrentPlayers, i);
		format.Format("%s - (%d/2)", servers->GetString(Attr::RoomName, i).AsCharPtr(), count);
		but->SetText(format);
		this->serverButtons.Append(but);
	}
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::JoinServerEntry(int entry)
{
	const Ptr<Attr::AttributeTable>& servers = this->GetMasterList();
	n_assert(entry < servers->GetNumRows());
	this->JoinRoom(servers->GetString(Attr::Id, entry));

}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::UpdatePlayerName(const Util::String & name)
{
// 	MultiplayerFeature::MultiplayerFeatureUnit::Instance()->GetPlayer()->SetPlayerName(name);
// 	Util::String gamename;
// 	gamename.Format("%s's game", name.AsCharPtr());
// 	this->SetGameName(gamename);
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::CancelSession()
{
	NetworkGame::CancelRoom();
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::UpdateChat(const Util::String & msg)
{
// 	Ptr<UI::UiLayout> lay = UI::UiFeatureUnit::Instance()->GetLayout("chatbox");
// 	Ptr<UI::UiElement> el = lay->GetElement("chat");	
// 	el->SetInputText("");
// 	const Util::String & plr = MultiplayerFeature::MultiplayerFeatureUnit::Instance()->GetPlayer()->GetPlayerName();
// 	this->AddChatMessage(plr, msg);
// 	Ptr<NotA::ChatMessage> chatMsg = NotA::ChatMessage::Create();
// 	chatMsg->SetSender(plr);
// 	chatMsg->SetMessage(msg);
// 	this->Send(chatMsg.cast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::AddChatMessage(const Util::String & sender, const Util::String & body)
{
// 	Ptr<UI::UiLayout> lay = UI::UiFeatureUnit::Instance()->GetLayout("chatbox");
// 	Ptr<UI::UiElement> el = lay->GetElement("chatbox");
// 
// 	Util::String format;
// 	
// 	format.Format("<p>%s: %s</p>", sender.AsCharPtr(), body.AsCharPtr());
// 	this->chatBuffer.Add(format);
// 	Util::String chat;
// 	for (int i = 0; i < this->chatBuffer.Size(); i++)
// 	{
// 		chat.Append(this->chatBuffer[i]);
// 	}
// 	el->SetText(chat);
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::UpdatePlayerReady(int id, bool val)
{
// 	Ptr<NotA::NotANetworkPlayer> player = MultiplayerFeature::MultiplayerFeatureUnit::Instance()->GetPlayer().cast<NotA::NotANetworkPlayer>();
// 	if (player->GetId() == id)
// 	{ 
// 		player->SetReady(val);
// 	}	
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::OnAllReady(bool dummy)
{
// 	Ptr<UI::UiElement> elem = UI::UiFeatureUnit::GetLayoutElement("gamelobby", "startbutton");
// 	elem->SetEnabled(true);
// 	elem->SetClasses("startbutton");
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::ConnectToIP(const Util::String & ip)
{
//     // go to right mode
// 	if (MultiplayerFeature::NetworkServer::Instance()->GetState() == MultiplayerFeature::NetworkServer::IDLE)
// 	{
// 		MultiplayerFeature::NetworkServer::Instance()->SearchForGames();
// 	}
//     this->JoinRoom(ip, true);
}

//------------------------------------------------------------------------------
/**
*/
void
CoreNetworkGame::SetWinner(int team)
{
/*	this->winner = team;*/
}

//------------------------------------------------------------------------------
/**
*/
bool
CoreNetworkGame::HasWon()
{
/*	return NotACastleApplication::Instance()->GetPlayer()->GetId() == this->winner;*/
	return 0;
}

}