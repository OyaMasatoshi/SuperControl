#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "polygon2D.h"

#include "title.h"

void Title::Initialize()
{
	AddGameObject<Polygon2D>(Layer::Polygon2D);
	Scene* scene = Manager::GetScene();
	Polygon2D* poly = scene->GetGameObject<Polygon2D>(Layer::Object);
	poly->SetTexture(0);
}

void Title::Finalize()
{
	Scene::Finalize();// 継承元クラスのメソッドの呼び出し
}

void Title::Update()
{
	Scene::Update();// 継承元クラスのメソッドの呼び出し

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Game>();
	}
}
