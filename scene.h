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
	std::list<GameObject*> m_GameObject[static_cast<int>(Layer::LayerAll)]; // STLのリスト構造

	void SetDebug(bool status) { m_Debug = status; }

public:
	Scene() {}
	virtual ~Scene(){}

	virtual void Initialize() = 0; // 純粋仮想関数

	template <typename T> // テンプレート関数
	T* AddGameObject(Layer layer)
	{
		T* gameObject = new T();
		m_GameObject[static_cast<int>(layer)].push_back(gameObject);
		gameObject->Initialize();
		
		return gameObject;
	}

	template <typename T> // テンプレート関数
	T* GetGameObject(Layer layer)
	{
		for (GameObject* object : m_GameObject[static_cast<int>(layer)])
		{
			// 型を調べる(RTTI動的型情報)
			if (typeid(*object) == typeid(T))
			{
				return (T*)object;
			}
		}
		return NULL;
	}

	template <typename T> // テンプレート関数
	std::vector<T*> GetGameObjects(Layer layer)
	{
		std::vector<T*> objects; // STLの配列

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

			m_GameObject[i].clear(); // リストのクリア
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

			// ラムダ式
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