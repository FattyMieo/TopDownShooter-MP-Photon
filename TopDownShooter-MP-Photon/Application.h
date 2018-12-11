//#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include <GLFW/glfw3.h>
#include "FMODExtension.h"
#include "AudioManager.h"
#include "NetworkListener.h"
#include "IRunnable.h"
#include "IDrawable.h"
#include "Container.h"
#include "GameObject.h"
#include "PhysicalGameObject.h"
#include "TypeDefinition.h"

class Application : public IRunnable, public IDrawable
{
protected:
	AudioManager* m_audioManager;
	NetworkListener* m_networkListener;
	Container<GameObject> m_GameObjects;
	Container<PhysicalGameObject> m_PhysObjects;

public:
	Application();
	virtual ~Application();

	void InitDependencies(AudioManager* audioManager, NetworkListener* networkListener);

	virtual void Start();
	virtual void Update(float deltaTime);
	void Draw();

	virtual void OnMouseCursorMove(float x, float y);
	virtual void OnMousePress(int button);
	virtual void OnMouseRelease(int button);
	virtual void OnKeyPress(int key);
	virtual void OnKeyRelease(int key);

	GameObject* Instantiate();
	GameObject* Instantiate(GameObject* copy);
	GameObject* Instantiate(Transform2D transform);
	GameObject* Instantiate(Vector2 position, float rotation);
	GameObject* Instantiate(Vector2 position, float rotation, Vector2 scale);

	GameObject& FindGameObject(int index);

	void Destroy(GameObject* gameObject);
};

#endif