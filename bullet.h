#pragma once

#include "gameObject.h"

class Bullet : public GameObject
{
private:

	static Model* m_Model;

	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

public:

	static void Load();
	static void Unload();

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

};
