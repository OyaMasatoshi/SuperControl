#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "title.h"
#include "texture.h"
#include "polygon2D.h"

#include "result.h"

void Result::Initialize()
{
	Scene* scene = Manager::GetScene();

	int test = Texture::SetLoadFile("asset/texture/Result_Good.png");
	Texture::Load();
	Polygon2D* title_texture = scene->AddGameObject<Polygon2D>(Layer::Polygon2D);
	title_texture->SetTexture(test, 480, 270, 960, 540, 0.0f, 0.0f, 1.0f, 1.0f);

}

void Result::Finalize()
{
	Texture::AllRelease();
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
