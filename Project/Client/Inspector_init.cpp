#include "pch.h"
#include "Inspector.h"

void Inspector::CreateChildUI()
{
	CreateComponentUI();

	CreateAssetUI();
}


#include "TransformUI.h"
#include "MeshRenderUI.h"
#include "Collider2DUI.h"
#include "Light2DUI.h"
#include "Animator2DUI.h"

void Inspector::CreateComponentUI()
{
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]->Deactivate();
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER] = new MeshRenderUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]->Deactivate();
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::MESHRENDER]);

	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D] = new Collider2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]->Deactivate();
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::COLLIDER2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D] = new Light2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]->Deactivate();
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::LIGHT2D]);

	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DUI;
	m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]->Deactivate();
	AddChildUI(m_arrComUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);
}

#include "TextureUI.h"
#include "MeshDataUI.h"
#include "MeshUI.h"
#include "MaterialUI.h"
#include "GraphicsShaderUI.h"
#include "ComputeShaderUI.h"
#include "SoundUI.h"
#include "PrefabUI.h"
#include "FSMUI.h"


void Inspector::CreateAssetUI()
{
	m_arrAssetUI[(UINT)ASSET_TYPE::MESH] = new MeshUI;
	AddChildUI(m_arrAssetUI[(UINT)ASSET_TYPE::MESH]);

	m_arrAssetUI[(UINT)ASSET_TYPE::MESHDATA] = new MeshDataUI;
	AddChildUI(m_arrAssetUI[(UINT)ASSET_TYPE::MESHDATA]);

	m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE] = new TextureUI;
	AddChildUI(m_arrAssetUI[(UINT)ASSET_TYPE::TEXTURE]);

	m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHICS_SHADER] = new GraphicsShader;
	AddChildUI(m_arrAssetUI[(UINT)ASSET_TYPE::GRAPHICS_SHADER]);

	m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER] = new ComputeShaderUI;
	AddChildUI(m_arrAssetUI[(UINT)ASSET_TYPE::COMPUTE_SHADER]);

	m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL] = new MaterialUI;
	AddChildUI(m_arrAssetUI[(UINT)ASSET_TYPE::MATERIAL]);

	m_arrAssetUI[(UINT)ASSET_TYPE::SOUND] = new SoundUI;
	AddChildUI(m_arrAssetUI[(UINT)ASSET_TYPE::SOUND]);

	m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB] = new PrefabUI;
	AddChildUI(m_arrAssetUI[(UINT)ASSET_TYPE::PREFAB]);

	m_arrAssetUI[(UINT)ASSET_TYPE::FSM] = new FSMUI;
	AddChildUI(m_arrAssetUI[(UINT)ASSET_TYPE::FSM]);
}