#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "player.h"
#include "scene.h"
#include "input.h"

void Camera::Initialize()
{
	m_Position = D3DXVECTOR3( 0.0f, 5.0f, -5.0f );
	m_Target = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

}

void Camera::Finalize()
{

}

void Camera::Update()
{
	bool debug = Manager::GetScene()->GetDebug();

	if (!debug) // 通常の場合
	{
		Player* player = Manager::GetScene()->GetGameObject<Player>(Layer::Object);
		m_Target = player->GetPosition();

		// トップビュー
		//m_Position = m_Target + D3DXVECTOR3(0.0f, 5.0f, -8.0f);

		// サードパーソンビュー
		D3DXVECTOR3 forward = player->GetForward();
		m_Position = m_Target - forward * 5.0f + D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	}
	else // デバッグの場合
	{

	}
	
}

void Camera::Draw()
{
	// ビューマトリクス設定(視点と注視点、カメラの位置の設定）
	D3DXMatrixLookAtLH( &m_viewMatrix, &m_Position, &m_Target, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );

	Renderer::SetViewMatrix(&m_viewMatrix);

	// プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
																							// near, far
	D3DXMatrixPerspectiveFovLH( &projectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_UP_HEIGHT, 1.0f, 1000.0f );

	Renderer::SetProjectionMatrix( &projectionMatrix );

}