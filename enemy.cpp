// ���C���N���[�h�̏��Ԃ͔��ɏd�v���C���ŕK�v�ȕ����珇�ɏ���
#include "main.h"
#include "renderer.h"
#include "model.h"	// player.h���O�ɕK�v
#include "enemy.h"

void Enemy::Initialize()
{
	m_Model = new Model();
	m_Model->Load("asset\\model\\sphere.obj");

	m_Position = D3DXVECTOR3(0.0f, 1.0f, 5.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXQuaternionIdentity(&m_Rotation);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "vertexLightingPS.cso");

}

void Enemy::Finalize()
{
	m_Model->Unload();
	delete m_Model;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Enemy::Update()
{

}

void Enemy::Draw()
{
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// �}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixRotationQuaternion(&rot, &m_Rotation);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	m_Model->Draw();

}
