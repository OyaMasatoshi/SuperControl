#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "missile.h"
#include "enemy.h"
#include "player.h"
#include "explosion.h"

/*------------------------------------------------------
				 定数
------------------------------------------------------*/
static const int MISSILE_LIFE_MAX = 180;
static const float MISSILE_ROTATION_MAX = 10.0f;


Model* Missile::m_Model;




void Missile::Load()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sphere.obj");
}

void Missile::Unload()
{
	m_Model->Unload();
	delete m_Model;
}

void Missile::Initialize()
{
	m_Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_Scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	D3DXQuaternionIdentity(&m_Rotation);
	D3DXQuaternionRotationAxis(&m_Rotation, &D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXToRadian(90));

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");
}

void Missile::Finalize()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Missile::Update()
{
	if (m_Life >= 30)
	{
		if (m_Life > MISSILE_LIFE_MAX)
		{
			SetDestroy();
			Scene* scene = Manager::GetScene();
			scene->AddGameObject<Explosion>(Layer::Object)->SetPosition(m_Position);
			return;
		}

		Player* player = Manager::GetScene()->GetGameObject<Player>(Layer::Object);
		D3DXVECTOR3 target_pos = player->GetPosition();

		D3DXVECTOR3 target_dir = m_Position - target_pos;
		D3DXVec3Normalize(&target_dir, &target_dir);

		// 回転軸を計算
		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &GetForward(), &target_dir);

		if (D3DXVec3Length(&normal) > 0.01f) // 平行チェック
		{
			D3DXVec3Normalize(&normal, &normal);

			// 回転角度を計算
			float x = D3DXVec3Dot(&GetForward(), &target_dir);
			float angle = acos(x);

			// 振動防止
			if (angle < 0.01f)
			{
				angle = 0.0f;
			}
			else if (angle > MISSILE_ROTATION_MAX)
			{
				angle = MISSILE_ROTATION_MAX;
			}

			D3DXQUATERNION q, result;
			D3DXQuaternionRotationAxis(&q, &normal, angle);
			D3DXQuaternionMultiply(&result, &m_Rotation, &q);
			SetRotation(result);
		}
		
	}

	m_Position -= GetForward() * 0.1f;

	m_Life++;
}

void Missile::Draw()
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
