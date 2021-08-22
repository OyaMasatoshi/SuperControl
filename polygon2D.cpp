// ※インクルードの順番は非常に重要メインで必要な物から順に書く
#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "scene.h"
#include "manager.h"

int g_num = 0;

void Polygon2D::Initialize()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3( 0.0f, 540.0f, 0.0f );
	vertex[0].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[0].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );	// 色
	vertex[0].TexCoord = D3DXVECTOR2( 0.0f, 0.0f );				// テクスチャの座標

	vertex[1].Position = D3DXVECTOR3( 960.0f, 540.0f, 0.0f );
	vertex[1].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[1].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[1].TexCoord = D3DXVECTOR2( 1.0f, 0.0f );

	vertex[2].Position = D3DXVECTOR3( 0.0f, 1080.0f, 0.0f );
	vertex[2].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[2].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[2].TexCoord = D3DXVECTOR2( 0.0f, 1.0f );

	vertex[3].Position = D3DXVECTOR3( 960.0f, 1080.0f, 0.0f );
	vertex[3].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[3].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[3].TexCoord = D3DXVECTOR2( 1.0f, 1.0f );

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
	D3DX11CreateShaderResourceViewFromFile( Renderer::GetDevice(),
											"asset/texture/Title.png",
											NULL,
											NULL,
											&m_Texture,
											NULL );
	assert(m_Texture);

	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		                                    "asset/texture/Result_Good.png",
		                                    NULL,
		                                    NULL,
		                                    &m_Texture1,
		                                    NULL);
	assert(m_Texture1);



	// unlit(ライティング無し）
	// シェーダーの作成
	Renderer::CreateVertexShader( &m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso" );

	Renderer::CreatePixelShader( &m_PixelShader, "unlitTexturePS.cso" );


}


void Polygon2D::Finalize()
{
	m_VertexBuffer->Release();
	m_Texture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Polygon2D::Update()
{

}

void Polygon2D::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout( m_VertexLayout );

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader( m_VertexShader, NULL, 0 );
	Renderer::GetDeviceContext()->PSSetShader( m_PixelShader, NULL, 0 );

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	// 頂点バッファ設定
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// テクスチャ設定
	if (g_num == 0)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	}
	else
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture1);
	}

	// プリミティブトポロジ設定（ポリゴンをどうやって書くか）
	Renderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// ポリゴン描画
	Renderer::GetDeviceContext()->Draw( 4, 0 );	// 第一引数：頂点数

}

void Polygon2D::SetTexture(int num)
{
	g_num = num;
}
