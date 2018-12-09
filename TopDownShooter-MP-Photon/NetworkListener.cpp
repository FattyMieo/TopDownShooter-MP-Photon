#include "NetworkListener.h"
#include <iostream>
#include "MyApplication.h"
#include "PhotonKey.h"

static const ExitGames::Common::JString appId = PhotonKey::GenerateKey(); // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";
static const ExitGames::Common::JString PLAYER_NAME = L"Windows";

// Functions
NetworkListener::NetworkListener() : mLoadBalancingClient(*this, appId, appVersion, ExitGames::Photon::ConnectionProtocol::TCP)
{

}

void NetworkListener::run(void)
{
	mLoadBalancingClient.service();
}

void NetworkListener::connect(void)
{
	std::wcout<<"Connecting..."<<std::endl;
	mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues(), PLAYER_NAME);
}

void NetworkListener::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

/*
void PhotonListener::opCreateRoom(void)
{
	ExitGames::Common::JString name;
	name += GETTIMEMS();
	mLoadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(4).setPlayerTtl(INT_MAX/2).setEmptyRoomTtl(10000));
	//mStateAccessor.setState(STATE_JOINING);
	//mpOutputListener->writeLine(ExitGames::Common::JString(L"creating room ") + name + L"...");
}

void PhotonListener::opJoinRandomRoom(void)
{
	mLoadBalancingClient.opJoinRandomRoom();
}

void PhotonListener::opJoinOrCreateRoom(void)
{
	ExitGames::Common::JString name("DemoLoadBalancing");
	mLoadBalancingClient.opJoinOrCreateRoom(name);
	//mStateAccessor.setState(STATE_JOINING);
	//mpOutputListener->writeLine(ExitGames::Common::JString(L"joining or creating room ") + name + L"...");
}
*/

void NetworkListener::sendEvent(unsigned char* data, int size)
{
	//static int64 count = 0;
	//mLoadBalancingClient.opRaiseEvent(false, ++count, 0);

	if (mLoadBalancingClient.getIsInRoom() || mLoadBalancingClient.getIsInGameRoom())
	{
		mLoadBalancingClient.opRaiseEvent(true, data, size, 0);
	}
}

// protocol implementations

void NetworkListener::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
}

void NetworkListener::connectionErrorReturn(int errorCode)
{
	std::wcout<<"connectionErrorReturn : "<<errorCode<<std::endl;
}

void NetworkListener::clientErrorReturn(int errorCode)
{
	std::wcout<<"clientErrorReturn : "<<errorCode<<std::endl;
}

void NetworkListener::warningReturn(int warningCode)
{
	std::wcout<<"warningReturn : "<<warningCode<<std::endl;
}

void NetworkListener::serverErrorReturn(int errorCode)
{
	std::wcout<<"serverErrorReturn : "<<errorCode<<std::endl;
}

void NetworkListener::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& playernrs, const ExitGames::LoadBalancing::Player& player)
{
	std::wcout<<"joinRoomEventAction"<<std::endl;
	MyApplication::GetInstance()->OnJoinRoomEvent(playerNr);
}

void NetworkListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
	std::wcout<<"leaveRoomEventAction"<<std::endl;
	MyApplication::GetInstance()->OnLeaveRoomEvent(playerNr);
}

void NetworkListener::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
	// you do not receive your own events, unless you specify yourself as one of the receivers explicitly, so you must start 2 clients, to receive the events, which you have sent, as sendEvent() uses the default receivers of opRaiseEvent() (all players in same room like the sender, except the sender itself)
//	EGLOG(ExitGames::Common::DebugLevel::ALL, L"");
//	mpOutputListener->write(ExitGames::Common::JString(L"r") + ExitGames::Common::ValueObject<long long>(eventContent).getDataCopy() + L" ");

	unsigned char* data = ExitGames::Common::ValueObject<unsigned char*>(eventContent).getDataCopy();
	const int size = *ExitGames::Common::ValueObject<unsigned char*>(eventContent).getSizes();
	if (data)
	{
		for (int i = 0; i < size; i++)
		{
			std::cout << (unsigned int)data[i] << " ";
		}
		std::cout << "received " << size << " bytes" << std::endl;
		MyApplication::GetInstance()->OnReceiveNetworkEvent(data, (unsigned int)size);
	}
	else
	{
		std::cout << "Invalid data!" << std::endl;
	}
}

void NetworkListener::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster)
{
	if(errorCode)
		std::wcout<<"Failed to connect : "<<errorString.cstr()<<std::endl;
	else
	{
		std::wcout<<"Connected to Photon Server."<<std::endl;
		
		std::wcout<<"Trying to join a random room..."<<std::endl;
		mLoadBalancingClient.opJoinRandomRoom();
	}
}

void NetworkListener::disconnectReturn(void)
{
	std::wcout<<"disconnected"<<std::endl;
}

void NetworkListener::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"createRoomReturn"<<std::endl;
	MyApplication::GetInstance()->OnCreateRoomEvent(localPlayerNr);
}

void NetworkListener::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"joinOrCreateRoomReturn"<<std::endl;
}

void NetworkListener::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"joinRoomReturn"<<std::endl;
}

void NetworkListener::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"joinRandomRoomReturn"<<std::endl;

	if(errorCode)
	{
		std::wcout<<"Failed to join a random room : "<<errorCode<<", "<<errorString.cstr()<<std::endl;

		if(errorCode == 32760) //no match found error code
		{
			std::wcout<<"Creating a room..."<<std::endl;
			//try to create a room and wait;
			ExitGames::Common::JString name;
			name += GETTIMEMS();
			mLoadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(2));
		}

		return;
	}
}

void NetworkListener::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"leaveRoomReturn"<<std::endl;
}

void NetworkListener::joinLobbyReturn(void)
{
	std::wcout<<"joinLobbyReturn"<<std::endl;
}

void NetworkListener::leaveLobbyReturn(void)
{
	std::wcout<<"leaveLobbyReturn"<<std::endl;
}

void NetworkListener::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout<<"onLobbyStatsResponse"<<std::endl;
}

void NetworkListener::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout<<"onLobbyStatsUpdate"<<std::endl;
}

void NetworkListener::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
//	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls / %ls", availableRegions.toString().cstr(), availableRegionServers.toString().cstr());
//	mpOutputListener->writeLine(L"onAvailableRegions: " + availableRegions.toString() + L" / " + availableRegionServers.toString());
	// select first region from list
	//mpOutputListener->writeLine(L"selecting region: " + availableRegions[0]);
	//mLoadBalancingClient.selectRegion(availableRegions[0]);
	mLoadBalancingClient.selectRegion("asia");
}
