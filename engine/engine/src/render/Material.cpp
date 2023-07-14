#include "StdAfx.h"
#include "render/Material.h"

namespace engine
{
	//std::future<MaterialGeneric::LoadResult> MaterialGeneric::Load(const MaterialGeneric& i_material, const Context& i_engineContext)
	//{
	//	
	//}
	//
	//std::future<MaterialGeneric::LoadResult> MaterialGeneric::LoadData(const MaterialGeneric& i_material, const Context& i_engineContext)
	//{
	//	//i_engineContext.GetAssetProvider().L
	//}

	//LoadVisual es marca, únicament un flag de que es vol carregar, i al RENDER, ya si eso, es comença

	MaterialGeneric::MaterialGeneric(std::shared_ptr<Asset<ShaderAssetVertex>> i_vertexShader, std::shared_ptr<Asset<ShaderAssetFragment>> i_fragmentShader)
		: m_vertexShader(std::move(i_vertexShader))
		, m_fragmentShader(std::move(i_fragmentShader))
	{

	}

	MaterialGeneric::~MaterialGeneric()
	{

	}

	const std::optional<std::string> MaterialGeneric::GetVertexShaderCode() const
	{
		return std::nullopt;
	}

	const std::optional<std::string> MaterialGeneric::GetFragmentShaderCode() const
	{
		return std::nullopt;
	}
}
