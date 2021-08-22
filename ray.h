#pragma once

#include "main.h"

class Ray
{
private:

	float _determinant(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c);

	bool _rayintersects(D3DXVECTOR3 origin, D3DXVECTOR3 ray, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c);

public:

};