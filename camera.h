#pragma once

#include "gameObject.h"

class Camera : public GameObject
{
private:

	D3DXVECTOR3 m_Target;
	D3DXMATRIX  m_viewMatrix;

public:
	
	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	D3DXMATRIX GetViewMatrix() const { return m_viewMatrix; };

};
