#include "texture.h"
#include "renderer.h"
#include "main.h"

TEXTURE Texture::m_Texture[TEXTURE_MAX];

void Texture::Initialize()
{
	//�e�N�X�`���Ǘ��e�[�u���̏�����
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		m_Texture[i].pTexture = NULL;
		m_Texture[i].width = 0;
		m_Texture[i].height = 0;
		m_Texture[i].filename[0] = 0;
	}
}

//�e�N�X�`���Ǘ����W���[���̏I������
void Texture::Finalize(void)
{
	AllRelease();
}



int Texture::SetLoadFile(const char * pFileName)
{
	//�t�@�C��������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//�e�[�u�����󂢂Ă���X�L�b�v
		if (m_Texture[i].filename[0] == 0)
		{
			continue;//�ȉ��̏������X�L�b�v
		}
		//���łɎw��̃t�@�C�����\�񂳂�Ă��邩�H
		if (strcmp(pFileName, m_Texture[i].filename) == 0)//strcmp���d���̂ŁA���\�[�X�Ƀn�b�V����g�ݍ���
		{
			//�\�񂳂�Ă���I
			return i;
		}
	}

	//�V�K�\��
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//�e�[�u�����g�p����������X�L�b�v
		if (m_Texture[i].filename[0] != 0)
		{
			continue;
		}

		strcpy(m_Texture[i].filename, pFileName);

		return i;
	}

	return TEXTURE_INVALID_ID;
}

int Texture::Load()
{
	int err_count = 0;
	HRESULT hr;

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (m_Texture[i].filename[0] == 0)
		{
			//��̃e�[�u���Ȃ̂ŃX�L�b�v
			continue;
		}
		if (m_Texture[i].pTexture != NULL)
		{
			//���łɓǂݍ��܂�Ă���
			continue;
		}

		m_Texture[i].pTexture = Rewrite(m_Texture[i].filename, &hr);
		
		//�ǂݍ��݌��ʂ̔���
		if (FAILED(hr))
		{
			//�ǂݍ��ݎ��s������
			//DebugPrintf("�ǂݍ��߂Ȃ������e�N�X�`���t�@�C��:[%s]\n", g_Textures[i].filename);
			err_count++;
		}
		else
		{
			//�e�N�X�`���̉𑜓x(���E����)�̒���
			D3DXIMAGE_INFO info;
			D3DXGetImageInfoFromFile(m_Texture[i].filename, &info);
			m_Texture[i].width = info.Width;
			m_Texture[i].height = info.Height;
		}
	}

	return err_count;
}



ID3D11ShaderResourceView* Texture::Rewrite(const char* pFileName, HRESULT *hr)
{
	VERTEX_3D vertex[4];

	vertex[0].Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].Position = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	vertex[1].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].Position = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	vertex[2].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].Position = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
	vertex[3].Normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, NULL);

	// �e�N�X�`���ǂݍ���
	ID3D11ShaderResourceView *I;
	HRESULT rhr = D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		pFileName,
		NULL,
		NULL,
		&I,
		NULL);

	// unlit(���C�e�B���O�����j
	// �V�F�[�_�[�̍쐬
	Renderer::CreateVertexShader(NULL, NULL, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(NULL, "unlitTexturePS.cso");


	*hr = rhr;

	return I;
}

ID3D11ShaderResourceView* Texture::Texture_GetTexture(int textureID)
{
	if (textureID == TEXTURE_INVALID_ID)
	{
		return NULL;
	}
	else
	{
		return m_Texture[textureID].pTexture;
	}
}


void Texture::Release(int textureIds[], int count)
{
	if (*textureIds == TEXTURE_INVALID_ID || *textureIds == NULL) return;

	for (int i = 0; i < count; i++)
	{
		if (m_Texture[textureIds[i]].pTexture && m_Texture[textureIds[i]].pTexture != NULL)
		{
			m_Texture[textureIds[i]].pTexture->Release();
			m_Texture[textureIds[i]].pTexture = NULL;
		}
		m_Texture[textureIds[i]].filename[0] = 0; //�擪��\0�����邱�Ƃŏ�����
		m_Texture[textureIds[i]].width = 0;
		m_Texture[textureIds[i]].height = 0;
	}

}

void Texture::AllRelease(void)
{
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (m_Texture[i].filename[0] == 0) continue;

		if (m_Texture[i].pTexture)
		{
			m_Texture[i].pTexture->Release();
			m_Texture[i].pTexture = NULL;
		}
		m_Texture[i].filename[0] = 0; //�擪��\0�����邱�Ƃŏ�����
		m_Texture[i].width = 0;
		m_Texture[i].height = 0;
	}
}
