#include "GameObject.h"

// Not needed
class Sphere : public GameObject
{
public:
	Sphere(Game *game, const std::string &name)
		: GameObject(game, name)
	{
	}

	void Update(float deltaTime) override
	{
		// Update children
		GameObject::Update(deltaTime);

		// ...
	}

	void Render(float deltaTime) override
	{
		// Render children
		GameObject::Render(deltaTime);

		// ...
	}

	void RenderAudio(float deltaTime) override
	{
		// Render children
		GameObject::RenderAudio(deltaTime);

		// ...
	}

	void Shutdown() override
	{
		// Shutdown children
		GameObject::Shutdown();

		// ...
	}
};