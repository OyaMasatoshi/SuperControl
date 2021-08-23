#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "title.h"
#include "polygon2D.h"

#include "result.h"

void Result::Initialize()
{
	AddGameObject<Polygon2D>(Layer::Polygon2D);
	Scene* scene = Manager::GetScene();
	Polygon2D* poly = scene->GetGameObject<Polygon2D>(Layer::Object);

}

void Result::Finalize()
{
	Scene::Finalize();// �p�����N���X�̃��\�b�h�̌Ăяo��
}

void Result::Update()
{
	Scene::Update();// �p�����N���X�̃��\�b�h�̌Ăяo��

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Title>();
	}
}
