#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "audio.h"
#include "scene.h"
#include "game.h"
#include "title.h"
#include "result.h"

Scene* Manager::m_Scene = nullptr;


void Manager::Initialize()
{
	Renderer::Initialize();
	Input::Initialize();
	Audio::InitMaster();

	//m_Scene = new Game();
	//m_Scene->Init();

	SetScene<Title>();

}


void Manager::Finalize()
{
	m_Scene->Finalize();
	delete m_Scene;


	Audio::UninitMaster();
	Input::Finalize();
	Renderer::Finalize();

}

void Manager::Update()
{
	Input::Update();

	m_Scene->Update();

}

void Manager::Draw()
{
	// •`‰æ‚ÌŠJn
	Renderer::Begin();

	m_Scene->Draw();

	// •`‰æ‚ÌI—¹
	Renderer::End();

}
