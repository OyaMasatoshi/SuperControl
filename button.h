#pragma once

#include "main.h"
#include "polygon2D.h"

enum class ButtonType
{
	Circle,
	Square,
	ButtonAll
};


class Button : public Polygon2D
{
private:
	int m_Texture;
	bool m_IsHold;
	ButtonType m_Type;

	bool _IsCollision(float x, float y);

public:

	void Initialize();
	void Finalize();
	void Update();
	void Draw();

	bool IsPush();
	bool IsTrigerr();
};