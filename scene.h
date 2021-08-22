#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "gameObject.h"
#include "input.h"
#include "renderer.h"

enum class Layer
{
	Camera,
	Object,
	Polygon2D,
	Sound,
	LayerAll
};


class Scene
{
private:

	bool m_Debug = false;

protected:
	std::list<GameObject*> m_GameObject[static_cast<int>(Layer::LayerAll)]; // STL�̃��X�g�\��

	void SetDebug(bool status) { m_Debug = status; }

public:
	Scene() {}
	virtual ~Scene(){}

	virtual void Initialize() = 0; // �������z�֐�

	template <typename T> // �e���v���[�g�֐�
	T* AddGameObject(Layer layer)
	{
		T* gameObject = new T();
		m_GameObject[static_cast<int>(layer)].push_back(gameObject);
		gameObject->Initialize();
		
		return gameObject;
	}

	template <typename T> // �e���v���[�g�֐�
	T* GetGameObject(Layer layer)
	{
		for (GameObject* object : m_GameObject[static_cast<int>(layer)])
		{
			// �^�𒲂ׂ�(RTTI���I�^���)
			if (typeid(*object) == typeid(T))
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	template <typename T> // �e���v���[�g�֐�
	std::vector<T*> GetGameObjects(Layer layer)
	{
		std::vector<T*> objects; // STL�̔z��

		for (GameObject* object : m_GameObject[static_cast<int>(layer)])
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}

	virtual void Finalize()
	{
		for (int i = 0; i < static_cast<int>(Layer::LayerAll); i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Finalize();
				delete object;
			}

			m_GameObject[i].clear(); // ���X�g�̃N���A
		}
		
	}

	virtual void Update()
	{

		for (int i = 0; i < static_cast<int>(Layer::LayerAll); i++)
		{
			for (GameObject* object : m_GameObject[i])
			{
				object->Update();
			}

			// �����_��
			m_GameObject[i].remove_if([](GameObject* object) { return object->Destroy(); });
		}
		
	}

	virtual void Draw()
	{
		for (int i = 0; i < static_cast<int>(Layer::LayerAll); i++)
		{
			if (i != static_cast<int>(Layer::Polygon2D))
			{
				Renderer::SetViewport(SCREEN_WIDTH, SCREEN_UP_HEIGHT);
			}
			else
			{
				Renderer::SetViewport(SCREEN_WIDTH, SCREEN_HEIGHT);
			}

			for (GameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}

	}

	bool GetDebug() const { return m_Debug; }
};