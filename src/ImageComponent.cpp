#include "ImageComponent.h"
#include "Utils.h"

StarBangBang::ImageComponent::ImageComponent(GameObject* gameObject, AEGfxVertexList* mesh, AEGfxTexture* texture)
{
	this->mesh = mesh;
	this->texture = texture;
	this->gameObject = gameObject;
}

void StarBangBang::ImageComponent::Draw()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// No tint
	AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
	// Set texture
	AEGfxTextureSet(texture, 0, 0);
	// Set Position
	AEVec2 pos = GetGameObjectPos(gameObject);
	AEGfxSetPosition(pos.x, pos.y);
	// Set Transparency
	AEGfxSetTransparency(1.0f);
	// Drawing the mesh (list of triangles)
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
}
