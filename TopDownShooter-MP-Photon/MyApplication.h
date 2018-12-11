#pragma once

#include "Application.h"
#include "Spritesheet.h"
#include "Text.h"
#include "PhysicalGameObject.h"
#include "PhysicalDesturctible.h"
#include "BoardState.h"

class MyApplication : public Application
{
private:
	Container<PhysicalDestructible> m_Bullets;
	Container<PhysicalDestructible> m_Rockets;

	Vector2 m_lastReceivedPos;
	double m_prevReceivedTime;

	Vector2 m_lastReceivedPos_Rocket;
	double m_prevReceivedTime_Rocket;
	bool m_gameStarted;
public:
	//Temporary names
	Vector2 otherCursorPos;

	int localPlayerID = -1;
	int activePlayer = -1;
	bool isRoomOwner = false;

	Text* statusText;
	BoardState board;

	PhysicalGameObject* player;
	PhysicalGameObject* opponent;

	PhysicalGameObject* rocket;
	
	Vector2 keyAxis;
	Vector2 cursorPos;

	int lastChoiceIndex = -1;
	EPlayerType lastChoicePlayerType = EPlayerType::EPlayerType_Empty;

	MyApplication();
	virtual ~MyApplication();

	static MyApplication* GetInstance()
	{
		static MyApplication app;
		return &app;
	}

	void Start() override;
	void Update(float deltaTime) override;

	void GenerateBullet(Vector2 cursorPosition, Vector2 playerPosition, Color playerColor);
	void Attack();
	void UpdateStatusText();

	void OnMouseCursorMove(float x, float y);
	void OnMousePress(int button);
	void OnKeyPress(int key);
	void OnKeyRelease(int key);
	void SendMove();
	void SendAttack();
	virtual void OnReceiveNetworkEvent(byte* packedData, uint size);
	virtual void OnReceiveNetworkEvent(float * packedData, uint size);
	virtual void OnCreateRoomEvent(int playerID);
	virtual void OnJoinRoomEvent(int playerID);
	virtual void OnLeaveRoomEvent(int playerID);
	virtual void SendNetworkEvent(byte* packedData, uint size);
	virtual void SendNetworkEvent(float* packedData, uint size);
};