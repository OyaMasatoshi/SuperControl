#include "Button.h"
#include "texture.h"
#include "polygon2D.h"

bool Button::_IsCollision(float x, float y)
{
	switch(static_cast<int>(m_Type))
	{
	case (static_cast<int>(ButtonType::Circle)):

		if (m_Size.x * m_Size.x <=
			(m_Coordinate.x - x) * (m_Coordinate.x - x) +
			(m_Coordinate.y - y) * (m_Coordinate.y - y))
		{
			return true;
		}
		break;

	case (static_cast<int>(ButtonType::Square)):

		if (x <= m_Coordinate.x + m_Size.x &&
			x >= m_Coordinate.x - m_Size.x &&
			y <= m_Coordinate.y + m_Size.y &&
			y >= m_Coordinate.y - m_Size.y)
		{
			return true;
		}
		break;
	}

	return false;
}

void Button::Initialize()
{
	Polygon2D::Initialize();
}

void Button::Finalize()
{
	Polygon2D::Finalize();
}

void Button::Update()
{

}

void Button::Draw()
{
	Polygon2D::Draw();
}

bool Button::IsPush()
{
	return false;
}

bool Button::IsTrigerr()
{
	return false;
}
