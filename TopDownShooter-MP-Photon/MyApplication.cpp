#include "MyApplication.h"
#include <iostream>
#include <sstream>
#include "DataPacker.h"
#include "DataSplitter.h"
#include "ResDefinition.h"

MyApplication::MyApplication() : Application()
{

}

MyApplication::~MyApplication()
{

}

void MyApplication::Start()
{
	Application::Start();

	Sprite* sp;
	std::stringstream ss;

	statusText = new Text("Initializing...");
	m_GameObjects.GetList().push_back(statusText);
	statusText->Start();
	statusText->GetTransform().position = Vector2(RESOLUTION_X / 2.0f, 64.0f);

	player = new PhysicalGameObject();
	m_GameObjects.GetList().push_back(player);
	m_PhysObjects.GetList().push_back(player);
	player->radius = 30.0f;
	player->Start();
	player->GetTransform().position = Vector2(20.0f, RESOLUTION_Y / 2.0f);
	player->velocity.x = 1.0f;
	sp = &(player->GetSprite());
	sp->LoadFromFile("Ball.png");
	sp->SetDimension(40, 40);
	sp->SetBlendingMode(BM_Alpha);
	sp->SetColor(Color(255, 0, 0));

	player->SetActive(false);

	opponent = new PhysicalGameObject();
	m_GameObjects.GetList().push_back(opponent);
	m_PhysObjects.GetList().push_back(opponent);
	opponent->radius = 20.0f;
	opponent->Start();
	opponent->GetTransform().position = Vector2(RESOLUTION_X - 20.0f, RESOLUTION_Y / 2.0f);
	opponent->velocity.x = -1.0f;
	sp = &(opponent->GetSprite());
	sp->LoadFromFile("Ball.png");
	sp->SetDimension(40, 40);
	sp->SetBlendingMode(BM_Alpha);
	sp->SetColor(Color(0, 255, 255));

	opponent->SetActive(false);

	rocket = new PhysicalGameObject();
	m_GameObjects.GetList().push_back(rocket);
	m_PhysObjects.GetList().push_back(rocket);
	rocket->radius = 10.0f;
	rocket->Start();
	rocket->GetTransform().position = Vector2(RESOLUTION_X / 2.0f, RESOLUTION_Y / 2.0f);
	sp = &(rocket->GetSprite());
	sp->LoadFromFile("Ball.png");
	sp->SetDimension(20, 20);
	sp->SetBlendingMode(BM_Alpha);
	sp->SetColor(Color(255, 255, 255));

	rocket->SetActive(false);
}

void MyApplication::Update(float deltaTime)
{
	Application::Update(deltaTime);

	for
	(
		std::list<PhysicalDestructible*>::iterator it = m_Bullets.GetList().begin();
		it != m_Bullets.GetList().end();
	)
	{
		if ((*it)->isToBeDestroyed)
		{
			GameObject* temp = *it;
			m_PhysObjects.GetList().remove(*it);
			it = m_Bullets.GetList().erase(it);
			Destroy(temp);
		}
		else
		{
			++it;
		}
	}

	if (!m_gameStarted)
		return;

	//Update player
	player->acceleration += keyAxis * deltaTime * 100.0f / player->mass;
	if (player->GetTransform().scale.x > 1.0f)
		player->GetTransform().scale -= deltaTime;
	else
		player->GetTransform().scale = Vector2(1.0f);

	//Network Update
	static double prevTime = glfwGetTime();

	double time = glfwGetTime();
	if (time - prevTime >= NetworkListener::gNetworkFrameTime)
	{
		SendMove();

		prevTime = time;
	}

	//Update Opponent's data
	//very slowly interpolate from on-going predicting pos to lastest received pos. Without this interpolation, the offset of opponent position will keep being accumulated. 
	opponent->GetTransform().position = (opponent->GetTransform().position * 0.995f) + (m_lastReceivedPos * 0.005f);
	if (opponent->GetTransform().scale.x > 1.0f)
		opponent->GetTransform().scale -= deltaTime;
	else
		opponent->GetTransform().scale = Vector2(1.0f);

	//Update Rocket's data
	rocket->GetTransform().position = (rocket->GetTransform().position * 0.995f) + (m_lastReceivedPos_Rocket * 0.005f);
	if (rocket->GetTransform().scale.GetSquaredMagnitude() > 1.0f)
		rocket->GetTransform().scale -= deltaTime;
	else
		rocket->GetTransform().scale = Vector2(1.0f);
}

void MyApplication::GenerateBullet(Vector2 cursorPosition, Vector2 playerPosition, Color playerColor)
{
	Vector2 dir = cursorPosition - playerPosition;
	dir.Normalize();
	//dir /= 10000.0f;

	PhysicalDestructible* bullet = new PhysicalDestructible();
	m_GameObjects.GetList().push_back(bullet);
	m_PhysObjects.GetList().push_back(bullet);
	m_Bullets.GetList().push_back(bullet);
	bullet->radius = 5.0f;
	bullet->Start();
	bullet->GetTransform().position = playerPosition + (dir * bullet->radius * 5.0f);
	bullet->velocity = dir * 10.0f;

	Sprite* sp = &(bullet->GetSprite());
	sp->LoadFromFile("Ball.png");
	sp->SetDimension(10, 10);
	sp->SetBlendingMode(BM_Alpha);
	sp->SetColor(playerColor);
}

void MyApplication::Attack()
{
	if (!m_gameStarted)
		return;

	GenerateBullet(cursorPos, player->GetTransform().position, player->GetSprite().GetColor());

	SendAttack();
}

void MyApplication::UpdateStatusText()
{
	std::stringstream ss;
	std::stringstream ssBTN;
	//ss.str(""); //Clears the stream
	switch (board.status)
	{
	case EStatus::EStatus_Waiting:
		ss << "Waiting for opponent...";
		break;
	case EStatus::EStatus_Playing:
		ss << "Fight!";
		break;
	case EStatus::EStatus_End:
		switch (board.endStatus)
		{
		case EEndStatus::EEndStatus_Victory:
			ss << char(1) << " Victorious " << char(1);
			break;
		case EEndStatus::EEndStatus_Defeat:
			ss << char(2) << "  Defeated  " << char(2);
			break;
		case EEndStatus::EEndStatus_Draw:
			ss << char(247) << "    Draw    " << char(247);
			break;
		case EEndStatus::EEndStatus_SVictory:
			ss << char(1) << " Opponent Surrendered " << char(1);
			break;
		case EEndStatus::EEndStatus_SDefeat:
			ss << char(2) << "     Surrendered     " << char(2);
			break;
		}
		break;
	}

	statusText->SetText(ss.str());
}

void MyApplication::OnMouseCursorMove(float x, float y)
{
	cursorPos = Vector2(x, y);
}

void MyApplication::OnMousePress(int button)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		Attack();
	}
}

void MyApplication::OnKeyPress(int key)
{
	switch (key)
	{
	case GLFW_KEY_A:
		keyAxis.x -= 1.0f;
		break;
	case GLFW_KEY_D:
		keyAxis.x += 1.0f;
		break;
	case GLFW_KEY_W:
		keyAxis.y -= 1.0f;
		break;
	case GLFW_KEY_S:
		keyAxis.y += 1.0f;
		break;
	}
}

void MyApplication::OnKeyRelease(int key)
{
	switch (key)
	{
	case GLFW_KEY_A:
		keyAxis.x += 1.0f;
		break;
	case GLFW_KEY_D:
		keyAxis.x -= 1.0f;
		break;
	case GLFW_KEY_W:
		keyAxis.y += 1.0f;
		break;
	case GLFW_KEY_S:
		keyAxis.y -= 1.0f;
		break;
	}
}

void MyApplication::SendMove()
{
	float data[12] =
	{
		player->GetTransform().position.x,
		player->GetTransform().position.y,
		player->velocity.x,
		player->velocity.y,
		player->acceleration.x,
		player->acceleration.y,
		rocket->GetTransform().position.x,
		rocket->GetTransform().position.y,
		rocket->velocity.x,
		rocket->velocity.y,
		rocket->acceleration.x,
		rocket->acceleration.y
	};

	SendNetworkEvent(data, 12);
}

void MyApplication::SendAttack()
{
	float data[2] =
	{
		cursorPos.x,
		cursorPos.y
	};

	SendNetworkEvent(data, 2);
}

void MyApplication::OnReceiveNetworkEvent(byte* packedData, uint size)
{
	if (size == sizeof(uint))
	{
		DataSplitter<uint> splitter;
		splitter.SetArray(packedData);

		DataPacker<uint> packer;
		packer.SetData(splitter.Merge());
		uint ex, ey;
		packer.Extract(ey, sizeof(ushort) * 8);
		packer.Extract(ex, sizeof(ushort) * 8);
		otherCursorPos.x = (float)ex;
		otherCursorPos.y = (float)ey;
	}
	else if (size == sizeof(byte))
	{
		DataSplitter<byte> splitter;
		splitter.SetArray(packedData);

		DataPacker<byte> packer;
		packer.SetData(splitter.Merge());

		byte bCommand;
		packer.Extract(bCommand, 8);

		uint command = (uint)bCommand;
		std::cout << "Received: " << command << std::endl;
	}
}

void MyApplication::OnReceiveNetworkEvent(float* packedData, uint size)
{
	if (m_gameStarted == false)
	{
		m_gameStarted = true;
		opponent->GetTransform().position = Vector2(packedData[0], packedData[1]);

		m_lastReceivedPos = opponent->GetTransform().position;
		m_lastReceivedPos_Rocket = rocket->GetTransform().position;
		m_prevReceivedTime = glfwGetTime();
		return;
	}

	//m_lastReceivedPos = Vector2(packedData[0], packedData[1]);
	

	//double currentTime = glfwGetTime();
	//double timeSinceLastReceive = currentTime - m_prevReceivedTime;

	//if (timeSinceLastReceive >= NetworkListener::gNetworkFrameTime * 0.5f) //filter the noise
	//{
	//	Vector2 targetPos = Vector2(packedData[0], packedData[1]);

	//	Vector2 dir = m_lastReceivedPos - targetPos;
	//	float magnitude = sqrt(dir.GetSquaredMagnitude()) / timeSinceLastReceive;
	//	Vector2 goVec = targetPos - opponent->GetTransform().position / 100.0f;
	//	goVec.Normalize();
	//	goVec *= magnitude;

	//	Vector2 finalVel = (opponent->velocity * 0.6f) + (goVec * 0.4f);
	//	opponent->velocity = finalVel;

	//	m_lastReceivedPos = targetPos;
	//	m_prevReceivedTime = currentTime;
	//}

	if (size == 12) //Movement
	{
		m_lastReceivedPos = Vector2(packedData[0], packedData[1]);
		opponent->velocity = Vector2(packedData[2], packedData[3]);
		opponent->acceleration = Vector2(packedData[4], packedData[5]);

		m_lastReceivedPos_Rocket = Vector2(packedData[6], packedData[7]);
		rocket->velocity = Vector2(packedData[8], packedData[9]);
		rocket->acceleration = Vector2(packedData[10], packedData[11]);
	}
	else if (size == 2) //Attack
	{
		GenerateBullet(Vector2(packedData[0], packedData[1]), opponent->GetTransform().position, opponent->GetSprite().GetColor());
	}
}

void MyApplication::OnCreateRoomEvent(int playerID)
{
	if (localPlayerID < 0)
	{
		localPlayerID = playerID;
		isRoomOwner = true;
		std::cout << localPlayerID << std::endl;
	}
}

void MyApplication::OnJoinRoomEvent(int playerID)
{
	if (localPlayerID < 0)
	{
		localPlayerID = playerID;
		std::cout << localPlayerID << std::endl;
	}

	if (playerID > 1)
	{
		if (!isRoomOwner)
		{
			PhysicalGameObject* temp = player;
			player = opponent;
			opponent = temp;
		}
		player->SetActive(true);
		opponent->SetActive(true);
		rocket->SetActive(true);

		//SendMove(myCursor->GetTransform().position.x, myCursor->GetTransform().position.y);
		//UpdateBoard();

		SendMove();

		board.status = EStatus::EStatus_Playing;
	}

	UpdateStatusText();
}

void MyApplication::OnLeaveRoomEvent(int playerID)
{
	if (localPlayerID != playerID)
	{
		opponent->SetActive(false);
		rocket->SetActive(false);
		m_gameStarted = false;
	}
}

void MyApplication::SendNetworkEvent(byte* packedData, uint size)
{
	if (m_networkListener != NULL)
	{
		m_networkListener->sendEvent(packedData, size);
	}
}

void MyApplication::SendNetworkEvent(float* packedData, uint size)
{
	if (m_networkListener != NULL)
	{
		m_networkListener->sendEvent(packedData, size);
	}
}
