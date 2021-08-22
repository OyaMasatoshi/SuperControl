#include "ray.h"

float Ray::_determinant(D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{
	return ((a.x * b.y * c.z) + (a.y * b.z * c.x) + (a.z * b.z * c.y) - (a.x * b.z * c.y) - (a.y * b.x * c.z) - (a.z * b.y * c.x));
}


bool Ray::_rayintersects(D3DXVECTOR3 origin, D3DXVECTOR3 ray, D3DXVECTOR3 a, D3DXVECTOR3 b, D3DXVECTOR3 c)
{


	return false;
}
