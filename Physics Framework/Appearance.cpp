#include "Appearance.h"

Appearance::Appearance(Geometry geo, Material mat, ID3D11ShaderResourceView* texRV)
{
	_geometry = geo;
	_material = mat;
	_textureRV = texRV;
}