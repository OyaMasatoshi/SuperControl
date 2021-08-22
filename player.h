#pragma once

#include "gameObject.h"
#include "audio.h"

class Player : public GameObject
{
private:

	class Model* m_Model;

	ID3D11VertexShader*	m_VertexShader = NULL;
	ID3D11PixelShader*	m_PixelShader = NULL;
	ID3D11InputLayout*	m_VertexLayout = NULL;

	Audio* m_se;

public:
	
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

};
