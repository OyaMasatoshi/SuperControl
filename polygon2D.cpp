// ���C���N���[�h�̏��Ԃ͔��ɏd�v���C���ŕK�v�ȕ����珇�ɏ���
#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "scene.h"
#include "manager.h"
#include "texture.h"

void Polygon2D::Initialize()
{
	m_Coordinate = D3DXVECTOR2(0.0f, 0.0f);
	m_Size       = D3DXVECTOR2(1.0f, 1.0f);
	m_UV_Origin  = D3DXVECTOR2(0.0f, 0.0f);
	m_UV_Size    = D3DXVECTOR2(1.0f, 1.0f);

	float defx = m_Coordinate.x - m_Size.x / 2;
	float defy = m_Coordinate.y - m_Size.y / 2;

	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(defx, defy, 0.0f );
	vertex[0].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[0].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );	   // �F
	vertex[0].TexCoord = D3DXVECTOR2(m_UV_Origin.x, m_UV_Origin.y);// �e�N�X�`���̍��W

	vertex[1].Position = D3DXVECTOR3(defx + m_Size.x, defy, 0.0f );
	vertex[1].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[1].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[1].TexCoord = D3DXVECTOR2(m_UV_Origin.x + m_UV_Size.x, 0.0f );

	vertex[2].Position = D3DXVECTOR3(defx, defy + m_Size.y, 0.0f );
	vertex[2].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[2].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[2].TexCoord = D3DXVECTOR2( 0.0f, m_UV_Origin.y + m_UV_Size.y);

	vertex[3].Position = D3DXVECTOR3(defx + m_Size.x, defy + m_Size.y, 0.0f );
	vertex[3].Normal   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	vertex[3].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[3].TexCoord = D3DXVECTOR2(m_UV_Origin.x + m_UV_Size.x, m_UV_Origin.y + m_UV_Size.y);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX_3D ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );

	// �e�N�X�`���ǂݍ���(��)
	D3DX11CreateShaderResourceViewFromFile( Renderer::GetDevice(),
											"asset/texture/Error_Texture.png",
											NULL,
											NULL,
											&m_Texture,
											NULL );
	assert(m_Texture);

	// �e�N�X�`���ǂݍ���
	//int textureid = Texture::SetLoadFile("asset/texture/Error_Texture.png");
	//Texture::Texture_GetTexture(textureid, m_Texture);

	// unlit(���C�e�B���O�����j
	// �V�F�[�_�[�̍쐬
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
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout( m_VertexLayout );

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader( m_VertexShader, NULL, 0 );
	Renderer::GetDeviceContext()->PSSetShader( m_PixelShader, NULL, 0 );

	// �}�g���N�X�ݒ�
	Renderer::SetWorldViewProjection2D();

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �v���~�e�B�u�g�|���W�ݒ�i�|���S�����ǂ�����ď������j
	Renderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw( 4, 0 );	// �������F���_��

}

void Polygon2D::SetTexture(int textureid, float x, float y, float size_width, float size_height,float start_u, float start_v, float texture_u,float texture_v)
{
	m_Coordinate = D3DXVECTOR2(x, y);
	m_Size       = D3DXVECTOR2(size_width, size_height);
	m_UV_Origin  = D3DXVECTOR2(start_u, start_v);
	m_UV_Size    = D3DXVECTOR2(texture_u, texture_v);

	// ���_�f�[�^��������
	VERTEX_3D vertex[4];

	float defx = m_Coordinate.x - m_Size.x / 2;
	float defy = m_Coordinate.y - m_Size.y / 2;

	vertex[0].Position = D3DXVECTOR3(defx, defy, 0.0f);
	vertex[0].Normal   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);	   // �F
	vertex[0].TexCoord = D3DXVECTOR2(m_UV_Origin.x, m_UV_Origin.y);// �e�N�X�`���̍��W

	vertex[1].Position = D3DXVECTOR3(defx + m_Size.x, defy, 0.0f);
	vertex[1].Normal   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(m_UV_Origin.x + m_UV_Size.x, 0.0f);

	vertex[2].Position = D3DXVECTOR3(defx, defy + m_Size.y, 0.0f);
	vertex[2].Normal   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, m_UV_Origin.y + m_UV_Size.y);

	vertex[3].Position = D3DXVECTOR3(defx + m_Size.x, defy + m_Size.y, 0.0f);
	vertex[3].Normal   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse  = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(m_UV_Origin.x + m_UV_Size.x, m_UV_Origin.y + m_UV_Size.y);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// �e�N�X�`���ǂݍ���
	m_Texture = Texture::Texture_GetTexture(textureid);

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

}

void Polygon2D::SetCoordinate(float x, float y)
{
}
