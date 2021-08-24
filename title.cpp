#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "texture.h"
#include "polygon2D.h"

#include "title.h"

void Title::Initialize()
{
	Scene* scene = Manager::GetScene();

	

	int test = Texture::SetLoadFile("asset/texture/Title.png");
	Texture::Load();

	Polygon2D* title_texture = scene->AddGameObject<Polygon2D>(Layer::Polygon2D);
	title_texture->SetTexture(test, 480, 270, 960, 540, 0.0f, 0.0f, 1.0f, 1.0f);
}

void Title::Finalize()
{
	Texture::AllRelease();
	Scene::Finalize();// �p�����N���X�̃��\�b�h�̌Ăяo��
}

void Title::Update()
{
	Scene::Update();// �p�����N���X�̃��\�b�h�̌Ăяo��

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Game>();
	}
}
