#include "texture.h"
#include "renderer.h"
#include "main.h"

TEXTURE Texture::m_Texture[TEXTURE_MAX];

void Texture::Initialize()
{
	//テクスチャ管理テーブルの初期化
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		m_Texture[i].pTexture = NULL;
		m_Texture[i].width = 0;
		m_Texture[i].height = 0;
		m_Texture[i].filename[0] = 0;
	}
}

//テクスチャ管理モジュールの終了処理
void Texture::Finalize(void)
{
	AllRelease();
}



int Texture::SetLoadFile(const char * pFileName)
{
	//ファイル名検索
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//テーブルが空いてたらスキップ
		if (m_Texture[i].filename[0] == 0)
		{
			continue;//以下の処理をスキップ
		}
		//すでに指定のファイルが予約されているか？
		if (strcmp(pFileName, m_Texture[i].filename) == 0)//strcmpが重いので、リソースにハッシュを組み込む
		{
			//予約されている！
			return i;
		}
	}

	//新規予約
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		//テーブルが使用中だったらスキップ
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
			//空のテーブルなのでスキップ
			continue;
		}
		if (m_Texture[i].pTexture != NULL)
		{
			//すでに読み込まれている
			continue;
		}

		m_Texture[i].pTexture = Rewrite(m_Texture[i].filename, &hr);
		
		//読み込み結果の判定
		if (FAILED(hr))
		{
			//読み込み失敗したら
			//DebugPrintf("読み込めなかったテクスチャファイル:[%s]\n", g_Textures[i].filename);
			err_count++;
		}
		else
		{
			//テクスチャの解像度(幅・高さ)の調査
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

	// 頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, NULL);

	// テクスチャ読み込み
	ID3D11ShaderResourceView *I;
	HRESULT rhr = D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		pFileName,
		NULL,
		NULL,
		&I,
		NULL);

	// unlit(ライティング無し）
	// シェーダーの作成
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
		m_Texture[textureIds[i]].filename[0] = 0; //先頭に\0を入れることで初期化
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
		m_Texture[i].filename[0] = 0; //先頭に\0を入れることで初期化
		m_Texture[i].width = 0;
		m_Texture[i].height = 0;
	}
}
