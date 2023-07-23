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

	std::span<std::shared_ptr<Asset<void>>> MaterialGeneric::GetAssets() const
	{
		return std::span<std::shared_ptr<Asset<void>>>((std::shared_ptr<Asset<void>>*) & m_vertexShader, 2);
	}


	const std::optional<std::span<const char>> MaterialGeneric::GetVertexShaderCode() const
	{
		if (m_vertexShader)
		{
			const std::optional<std::vector<char>>& data = m_vertexShader->GetData();
			if (data)
			{
				std::span<const char> view (data->begin(), data->size());
				return view;
			}
		}
		return std::nullopt;
	}

	const std::optional<std::span<const char>> MaterialGeneric::GetFragmentShaderCode() const
	{
		if (m_fragmentShader)
		{
			const std::optional<std::vector<char>>& data = m_fragmentShader->GetData();
			if (data)
			{
				std::span<const char> view(data->begin(), data->size());
				return view;
			}
		}
		return std::nullopt;
	}
}
