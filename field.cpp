// ※インクルードの順番は非常に重要メインで必要な物から順に書く
#include "main.h"
#include "renderer.h"
#include "field.h"

void Field3D::Initialize()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3( -10.0f, 0.0f, 10.0f );
	vertex[0].Normal   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	vertex[0].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );	// 色
	vertex[0].TexCoord = D3DXVECTOR2( 0.0f, 0.0f );				// テクスチャの座標

	vertex[1].Position = D3DXVECTOR3( 10.0f, 0.0f, 10.0f );
	vertex[1].Normal   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	vertex[1].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[1].TexCoord = D3DXVECTOR2( 10.0f, 0.0f );

	vertex[2].Position = D3DXVECTOR3( -10.0f, 0.0f, -10.0f );
	vertex[2].Normal   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	vertex[2].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[2].TexCoord = D3DXVECTOR2( 0.0f, 10.0f );

	vertex[3].Position = D3DXVECTOR3( 10.0f, 0.0f, -10.0f );
	vertex[3].Normal   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	vertex[3].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[3].TexCoord = D3DXVECTOR2( 10.0f, 10.0f );

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX_3D ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );

	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
										   "asset/texture/床.jpeg",
										   NULL,
										   NULL,
										   &m_Texture,
										   NULL);
	assert(m_Texture);

	// シェーダーの作成
	Renderer::CreateVertexShader( &m_VertexShader, &m_VertexLayout, "vertexLightingVS.cso" );

	Renderer::CreatePixelShader( &m_PixelShader, "vertexLightingPS.cso" );

	m_Position = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_Scale =    D3DXVECTOR3( 1.0f, 1.0f, 1.0f );
	D3DXQuaternionIdentity(&m_Rotation);

}

void Field3D::Finalize()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Field3D::Update()
{

}

void Field3D::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader( m_VertexShader, NULL, 0 );
	Renderer::GetDeviceContext()->PSSetShader( m_PixelShader, NULL, 0 );

	// ワールドマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling( &scale, m_Scale.x, m_Scale.y, m_Scale.z );
	//D3DXMatrixRotationYawPitchRoll( &rot, m_Rotation.y, m_Rotation.x, m_Rotation.z );
	D3DXMatrixRotationQuaternion(&rot, &m_Rotation);
	D3DXMatrixTranslation( &trans, m_Position.x, m_Position.y, m_Position.z );
	world = scale * rot * trans;
	Renderer::SetWorldMatrix( &world );

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// マテリアル設定
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );	// マテリアルの初期化
	material.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );	// 反射光
	material.Ambient = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );	// 環境光
	Renderer::SetMaterial( material );

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources( 0, 1, &m_Texture );

	// プリミティブトポロジ設定（ポリゴンをどうやって書くか）
	Renderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw( 4, 0 );	// 第一引数：頂点数

}
