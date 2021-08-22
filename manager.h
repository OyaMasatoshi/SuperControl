#pragma once



class Manager
{
private:
	static class Scene* m_Scene;

public:
	static void Initialize();
	static void Finalize();
	static void Update();
	static void Draw();

	static class Scene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene()
	{
		if (m_Scene)// NULLチェック
		{
			m_Scene->Finalize();
			delete m_Scene;
		}

		T* scnen = new T();
		m_Scene = scnen;
		scnen->Initialize();
	}
};