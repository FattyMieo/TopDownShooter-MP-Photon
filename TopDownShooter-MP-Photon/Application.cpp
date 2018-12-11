#include "Application.h"

Application::Application()
{

}

Application::~Application()
{
	std::list<GameObject*>::iterator it = m_GameObjects.GetList().begin();

	while (it != m_GameObjects.GetList().end())
	{
		delete *it;
		++it;
	}

	m_GameObjects.GetList().clear();
	m_PhysObjects.GetList().clear();
}

void Application::InitDependencies(AudioManager* audioManager, NetworkListener* networkListener)
{
	m_audioManager = audioManager;
	m_networkListener = networkListener;
}

void Application::Start() { }

void Application::Update(float deltaTime)
{
	for
	(
		std::list<GameObject*>::iterator it = m_GameObjects.GetList().begin();
		it != m_GameObjects.GetList().end();
		++it
	)
	{
		GameObject* go = *it;
		if (go->IsActive())
			go->Update(deltaTime);
	}

	std::vector<PhysicalGameObject*> physObjVec;
	for
	(
		std::list<PhysicalGameObject*>::iterator it = m_PhysObjects.GetList().begin();
		it != m_PhysObjects.GetList().end();
		++it
	)
	{
		PhysicalGameObject* go = *it;
		if (go->IsActive())
		{
			for
			(
				std::vector<PhysicalGameObject*>::iterator other_it = physObjVec.begin();
				other_it != physObjVec.end();
				++other_it
			)
			{
				PhysicalGameObject* other = *other_it;

				go->UpdateCollision(deltaTime, *other);
			}

			physObjVec.push_back(go);
		}
	}
}

void Application::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::list<GameObject*>::iterator it = m_GameObjects.GetList().begin();

	while (it != m_GameObjects.GetList().end())
	{
		GameObject* go = *it;
		if(go->IsActive())
			go->Draw();
		++it;
	}
}

void Application::OnMouseCursorMove(float x, float y) { }
void Application::OnMousePress(int button) { }
void Application::OnMouseRelease(int button) { }
void Application::OnKeyPress(int key) { }
void Application::OnKeyRelease(int key) { }

GameObject* Application::Instantiate()
{
	return Instantiate(new GameObject());
}

GameObject* Application::Instantiate(GameObject* copy)
{
	m_GameObjects.GetList().push_back(copy);
	copy->Start();
	return copy;
}

GameObject* Application::Instantiate(Transform2D transform)
{
	GameObject* go = new GameObject();
	go->SetTransform(transform);

	return Instantiate(go);
}

GameObject* Application::Instantiate(Vector2 position, float rotation)
{
	return Instantiate(position, rotation, Vector2(1.0f, 1.0f));
}

GameObject* Application::Instantiate(Vector2 position, float rotation, Vector2 scale)
{
	Transform2D transform;
	transform.position = position;
	transform.rotation = rotation;
	transform.scale = scale;

	return Instantiate(transform);
}

GameObject& Application::FindGameObject(int index)
{
	return m_GameObjects.GetItem(index);
}

void Application::Destroy(GameObject* gameObject)
{
	GameObject* cachedGO = gameObject;
	m_GameObjects.GetList().remove(gameObject);
	delete gameObject;
}
