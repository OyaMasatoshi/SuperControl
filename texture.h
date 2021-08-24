#pragma once

#include <d3dx9.h>
#include "main.h"

#define TEXTURE_FILENAME_MAX (512) //テクスチャファイル名最大文字数
#define TEXTURE_INVALID_ID   (-1)  //無効なテクスチャ管理番号
#define TEXTURE_MAX (1024)         //テクスチャ管理最大数

//テクスチャ管理テーブル構造体
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

	//テクスチャファイルの読み込み予約
    //引数:pFileName ファイル名
    //戻ち値:テクスチャ管理番号　最大管理数を超えている場合はINVALID_TEXTURE_ID(無効)
	static int SetLoadFile(const char* pFileName);

	static ID3D11ShaderResourceView* Rewrite(const char* pFileName, HRESULT *hr);

	//テクスチャの部分解放
    //引数:textureIds[] 解放したいtextureIdが入ったint型配列の先頭アドレス
	//引数:count 解放するテクスチャの数
	static void Release(int textureIds[], int count);//int textureIds[] => int* pTextureIds

	//テクスチャの全解放
	static void AllRelease(void);

	//テクスチャインターフェイスポインタの取得
    //引数:テクスチャ管理番号
    //戻り値:テクスチャインターフェースポインター
	static ID3D11ShaderResourceView* Texture_GetTexture(int textureID);

};