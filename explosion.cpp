#include "main.h"
#include "renderer.h"
#include "explosion.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"

void Explosion::Initialize()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(-0.5f, 1.0f, 0.0f);
	vertex[0].Normal   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);				// テクスチャの座標

	vertex[1].Position = D3DXVECTOR3( 0.5f, 1.0f, 0.0f);
	vertex[1].Normal   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
	vertex[2].Normal   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
	vertex[3].Normal   = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC; // ←頂点データを書き換えられるように設定
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // ←頂点データを書き換えられるように設定

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/Explosion.png",
		NULL,
		NULL,
		&m_Texture,
		NULL);
	assert(m_Texture);

	// unlit(ライティング無し）
	// シェーダーの作成
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXQuaternionIdentity(&m_Rotation);

}

void Explosion::Finalize()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Explosion::Update()
{
	m_Count++;

	if (m_Count >= 16) 
	{
		SetDestroy();
		return;
	}
}

void Explosion::Draw()
{
	// テクスチャ座標算出
	float x = m_Count % 4 * (1.0f / 4);
	float y = m_Count / 4 * (1.0f / 4);

	// 頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].Position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	vertex[0].TexCoord = D3DXVECTOR2(x, y);				// テクスチャの座標

	vertex[1].Position = D3DXVECTOR3(1.0f, 1.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(x + 0.25f, y);

	vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(x, y + 0.25f);

	vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(x + 0.25f, y + 0.25f);

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);


	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// カメラのビューマトリクス取得
	Scene* scene = Manager::GetScene();
	Camera* camera = scene->GetGameObject<Camera>(Layer::Camera);
	D3DXMATRIX view = camera->GetViewMatrix();

	// ビューの逆行列
	D3DXMATRIX invView;

	view._41 = 0.0f;
	view._42 = 0.0f;
	view._43 = 0.0f;

	D3DXMatrixTranspose(&invView, &view);

	//D3DXMatrixInverse(&invView, NULL, &view);
	//invView._41 = 0.0f;
	//invView._42 = 0.0f;
	//invView._43 = 0.0f;


	// ワールドマトリクス設定
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);

	world = scale * invView * trans;
	Renderer::SetWorldMatrix(&world);

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));	// マテリアルの初期化
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 反射光
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 環境光
	Renderer::SetMaterial(material);

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// プリミティブトポロジ設定（ポリゴンをどうやって書くか）
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw(4, 0);	// 第一引数：頂点数

}