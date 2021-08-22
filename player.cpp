// ※インクルードの順番は非常に重要メインで必要な物から順に書く
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "scene.h"
#include "model.h"	// player.hより前に必要
#include "player.h"
#include "bullet.h"
#include "audio.h"

#include "gameObject.h"

void Player::Initialize()
{
	m_Model = new Model();
	m_Model->Load( "asset\\model\\sphere.obj" );

	m_Position = D3DXVECTOR3( 0.0f, 1.0f, -3.0f );
	m_Scale    = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	D3DXQuaternionIdentity(&m_Rotation);

	Renderer::CreateVertexShader( &m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso" );

	Renderer::CreatePixelShader( &m_PixelShader, "vertexLightingPS.cso" );

	Scene* scene = Manager::GetScene();
	m_se = scene->AddGameObject<Audio>(Layer::Sound);
	m_se->Load("asset/sound/bgm/07.wav");

}

void Player::Finalize()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Player::Update()
{
	D3DXVECTOR3 forward = GetForward();

	if (Input::GetKeyPress('A'))
	{
		//m_Position.x -= 0.1f;
		//m_Rotation.y -= 0.05f;
		D3DXQUATERNION q;
		D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0.0f,1.0f,0.0f), D3DXToRadian(-3));
		D3DXQuaternionMultiply(&m_Rotation, &m_Rotation, &q);

	}
	
	if (Input::GetKeyPress('D')) 
	{
		//m_Position.x += 0.1f;
		//m_Rotation.y += 0.05f;
	}

	//if (Input::GetKeyPress('W')) 
	//{
	//	m_Position.z += 0.1f;
	//}

	//if (Input::GetKeyPress('S')) 
	//{
	//	m_Position.z -= 0.1f;
	//}

	if (Input::GetKeyPress('W'))
	{
		m_Position += forward * 0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Position -= forward * 0.1f;
	}


	if (Input::GetKeyTrigger(VK_SPACE)) 
	{
		Scene* scene = Manager::GetScene();
		Bullet* bullet = scene->AddGameObject<Bullet>(Layer::Object);
		bullet->SetPosition(m_Position);
		bullet->SetRotation(m_Rotation);
		m_se->Play();
	}

}

void Player::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout( m_VertexLayout );

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader( m_VertexShader, NULL, 0 );
	Renderer::GetDeviceContext()->PSSetShader( m_PixelShader, NULL, 0 );

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling( &scale, m_Scale.x, m_Scale.y, m_Scale.z );
	//D3DXMatrixRotationYawPitchRoll( &rot, m_Rotation.y, m_Rotation.x, m_Rotation.z );
	D3DXMatrixRotationQuaternion(&rot, &m_Rotation);
	D3DXMatrixTranslation( &trans, m_Position.x, m_Position.y, m_Position.z );
	world = scale * rot * trans;
	Renderer::SetWorldMatrix( &world );

	m_Model->Draw();

}

