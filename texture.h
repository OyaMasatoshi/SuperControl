#pragma once

#include <d3dx9.h>
#include "main.h"

#define TEXTURE_FILENAME_MAX (512) //�e�N�X�`���t�@�C�����ő啶����
#define TEXTURE_INVALID_ID   (-1)  //�����ȃe�N�X�`���Ǘ��ԍ�
#define TEXTURE_MAX (1024)         //�e�N�X�`���Ǘ��ő吔

//�e�N�X�`���Ǘ��e�[�u���\����
typedef struct Texture_tag
{
	ID3D11ShaderResourceView* pTexture;
	unsigned long width;
	unsigned long height;
	char filename[TEXTURE_FILENAME_MAX];

}TEXTURE;

class Texture
{
private:

	static TEXTURE m_Texture[TEXTURE_MAX];

public:

	static void Initialize();
	static void Finalize();
	static int  Load();

	//�e�N�X�`���t�@�C���̓ǂݍ��ݗ\��
    //����:pFileName �t�@�C����
    //�߂��l:�e�N�X�`���Ǘ��ԍ��@�ő�Ǘ����𒴂��Ă���ꍇ��INVALID_TEXTURE_ID(����)
	static int SetLoadFile(const char* pFileName);

	static ID3D11ShaderResourceView* Rewrite(const char* pFileName, HRESULT *hr);

	//�e�N�X�`���̕������
    //����:textureIds[] ���������textureId��������int�^�z��̐擪�A�h���X
	//����:count �������e�N�X�`���̐�
	static void Release(int textureIds[], int count);//int textureIds[] => int* pTextureIds

	//�e�N�X�`���̑S���
	static void AllRelease(void);

	//�e�N�X�`���C���^�[�t�F�C�X�|�C���^�̎擾
    //����:�e�N�X�`���Ǘ��ԍ�
    //�߂�l:�e�N�X�`���C���^�[�t�F�[�X�|�C���^�[
	static ID3D11ShaderResourceView* Texture_GetTexture(int textureID);

};