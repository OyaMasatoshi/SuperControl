// ���C���N���[�h�̏��Ԃ͔��ɏd�v���C���ŕK�v�ȕ����珇�ɏ���
#include "main.h"
#include "renderer.h"
#include "field.h"

void Field3D::Initialize()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3( -10.0f, 0.0f, 10.0f );
	vertex[0].Normal   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	vertex[0].Diffuse  = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );	// �F
	vertex[0].TexCoord = D3DXVECTOR2( 0.0f, 0.0f );				// �e�N�X�`���̍��W

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

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX_3D ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );

	// �e�N�X�`���ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
										   "asset/texture/��.jpeg",
										   NULL,
										   NULL,
										   &m_Texture,
										   NULL);
	assert(m_Texture);

	// �V�F�[�_�[�̍쐬
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
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader( m_VertexShader, NULL, 0 );
	Renderer::GetDeviceContext()->PSSetShader( m_PixelShader, NULL, 0 );

	// ���[���h�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling( &scale, m_Scale.x, m_Scale.y, m_Scale.z );
	//D3DXMatrixRotationYawPitchRoll( &rot, m_Rotation.y, m_Rotation.x, m_Rotation.z );
	D3DXMatrixRotationQuaternion(&rot, &m_Rotation);
	D3DXMatrixTranslation( &trans, m_Position.x, m_Position.y, m_Position.z );
	world = scale * rot * trans;
	Renderer::SetWorldMatrix( &world );

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );	// �}�e���A���̏�����
	material.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );	// ���ˌ�
	material.Ambient = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );	// ����
	Renderer::SetMaterial( material );

	// �e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources( 0, 1, &m_Texture );

	// �v���~�e�B�u�g�|���W�ݒ�i�|���S�����ǂ�����ď������j
	Renderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// �|���S���`��
	Renderer::GetDeviceContext()->Draw( 4, 0 );	// �������F���_��

}