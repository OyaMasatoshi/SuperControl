#pragma once

#include "gameObject.h"
#include "texture.h"

class Polygon2D : public GameObject
{

private:
	// NULLチェックするプログラムがない場合はNULLを入れる。
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

public:
	//Polygon2D();
	// ~Polygon2D();

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	void SetTexture(int textureid, float x, float y, float size_width, float size_height, float start_u, float start_v, float texture_u, float texture_v);

};

