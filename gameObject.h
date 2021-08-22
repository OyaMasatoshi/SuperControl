#pragma once

class GameObject
{
protected:	// 継承先のクラスからアクセスできる
	bool m_Destroy = false;

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Scale;
	D3DXQUATERNION m_Rotation;

public:
	GameObject() {} // コンストラクタ
	virtual ~GameObject() {} // デストラクタ（仮想関数）
	
	// 純粋仮想関数
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	D3DXVECTOR3 GetPosition() const { return m_Position; }
	void SetPosition(D3DXVECTOR3 Position) { m_Position = Position; }
	void SetRotation(D3DXQUATERNION Rotation) { m_Rotation = Rotation; }

	D3DXVECTOR3 GetForward() // 前方向ベクトル取得
	{
		//D3DXMATRIX rot;
		//D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
		//D3DXVECTOR3 forward;
		//forward.x = rot._31;
		//forward.y = rot._32;
		//forward.z = rot._33;
		//// _11_12_13はx方向,_21_22_23はy方向
		//// _41_42_43は位置
		//return forward;

		D3DXMATRIX r;
		D3DXVECTOR3 forward;
		D3DXMatrixRotationQuaternion(&r, &m_Rotation);

		forward.x = r._31;
		forward.y = r._32;
		forward.z = r._33;

		return forward;
	}

	D3DXVECTOR3 GetRight() // 右方向ベクトル取得
	{
		D3DXMATRIX r;
		D3DXVECTOR3 right;
		D3DXMatrixRotationQuaternion(&r, &m_Rotation);

		right.x = r._11;
		right.y = r._12;
		right.z = r._13;

		return right;
	}

	D3DXVECTOR3 GetUp() // 上方向ベクトル取得
	{
		D3DXMATRIX r;
		D3DXVECTOR3 up;
		D3DXMatrixRotationQuaternion(&r, &m_Rotation);

		up.x = r._21;
		up.y = r._22;
		up.z = r._23;

		return up;
	}


	void SetDestroy() { m_Destroy = true; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			Finalize();
			delete this;
			return true;
		}
		else 
		{
			return false;
		}
	}

};
