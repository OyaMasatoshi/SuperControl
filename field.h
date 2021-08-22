#pragma once

#include "gameObject.h"

class Field3D : public GameObject
{
private:

	// NULLチェックするプログラムがない場合はNULLを入れる。
	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

};
