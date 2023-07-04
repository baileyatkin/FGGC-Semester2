#pragma once

#include <DirectXMath.h>
#include <d3d11.h>
#include "Structures.h"


using namespace DirectX;

class Appearance
{
public:
	Appearance(Geometry geo, Material mat, ID3D11ShaderResourceView* texRV);

	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView* GetTextureRV() const { return _textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }

	Geometry GetGeometryData() const { return _geometry; }

	Material GetMaterial() const { return _material; }

	Geometry _geometry;
	Material _material;
	ID3D11ShaderResourceView* _textureRV;
};

