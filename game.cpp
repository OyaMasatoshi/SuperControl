#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "field.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "polygon2D.h"
#include "explosion.h"
#include "input.h"
#include "missile.h"
#include "audio.h"
#include "result.h"

#include "game.h"



void Game::Initialize()
{
	Bullet::Load();
	Missile::Load();

	AddGameObject<Camera>(Layer::Camera);
	AddGameObject<Field3D>(Layer::Object);
	AddGameObject<Player>(Layer::Object);

	AddGameObject<Enemy>(Layer::Object)->SetPosition(D3DXVECTOR3(-3.0, 1.0f, 5.0f));
	AddGameObject<Enemy>(Layer::Object)->SetPosition(D3DXVECTOR3(0.0, 1.0f, 5.0f));
	AddGameObject<Enemy>(Layer::Object)->SetPosition(D3DXVECTOR3(3.0, 1.0f, 5.0f));

	Polygon2D* paint = AddGameObject<Polygon2D>(Layer::Polygon2D);
	paint->SetTexture("asset/texture/床.jpeg", 480, 770, 100, 100, 0, 0, 1, 1);

	//AddGameObject<Bullet>();

	Audio *bgm = AddGameObject<Audio>(Layer::Sound);
	bgm->Load("asset/sound/bgm/stageBGM.wav");
	bgm->Play();

}

void Game::Update()
{
	if (Input::GetKeyPress('1'))
	{
		Game::SetDebug(!Game::GetDebug());
	}

	if (Input::GetKeyTrigger('Q'))
	{
		Scene* scene = Manager::GetScene();
		Missile* missile = scene->AddGameObject<Missile>(Layer::Object);
		missile->SetPosition(D3DXVECTOR3(-3.0, 1.0f, 5.0f));
	}

	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Result>();
	}
}


void Game::Finalize()
{
	Scene::Finalize();// 継承元クラスのメソッドの呼び出し
	Bullet::Unload();
	Missile::Unload();
}
