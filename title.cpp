#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "texture.h"
#include "polygon2D.h"
#include "button.h"

#include "title.h"

void Title::Initialize()
{
	Scene* scene = Manager::GetScene();

	

	int title_texture = Texture::SetLoadFile("asset/texture/Title.png");
	int button_texture = Texture::SetLoadFile("asset/texture/(TBD)button.png");
	Texture::Load();

	Polygon2D* title_bg = scene->AddGameObject<Polygon2D>(Layer::Polygon2D);
	title_bg->SetTexture(title_texture, 480, 270, 960, 540, 0.0f, 0.0f, 1.0f, 1.0f);

	Button* title_button = scene->AddGameObject<Button>(Layer::Polygon2D);
	title_button->SetTexture(button_texture, 480, 770, 250, 250, 0.0f, 0.0f, 1.0f, 1.0f);
}

void Title::Finalize()
{
	Texture::AllRelease();
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
