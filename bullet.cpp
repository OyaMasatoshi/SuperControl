// ※インクルードの順番は非常に重要メインで必要な物から順に書く
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"


Model* Bullet::m_Model;//スタティックメンバ変数はCPP側でも宣言する必要がある


void Bullet::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sphere.obj");
}


void Bullet::Unload()
{
	m_Model->Unload();
	delete m_Model;
}



void Bullet::Initialize()
{

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	D3DXQuaternionIdentity(&m_Rotation);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

}

void Bullet::Finalize()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Bullet::Update()
{
	D3DXVECTOR3 front = Bullet::GetForward();

	m_Position += front;

	if (m_Position.z > 6.0f) 
	{
		SetDestroy();
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Explosion>(Layer::Object)->SetPosition(m_Position);
		return;
	}

	Scene* scene = Manager::GetScene();
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(Layer::Object);

	for (Enemy* enemy : enemyList)
	{
		D3DXVECTOR3 enemyPosition = enemy->GetPosition();

		D3DXVECTOR3 direction = m_Position - enemyPosition;
		float length = D3DXVec3Length(&direction);

		if (length < 2.0f)
		{
			enemy->SetDestroy();
			SetDestroy();
			Scene* scene = Manager::GetScene();
			scene->AddGameObject<Explosion>(Layer::Object)->SetPosition(m_Position);
			return;
		}
	}

}

void Bullet::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixRotationQuaternion(&rot, &m_Rotation);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

}

