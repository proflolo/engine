#include "StdAfx.h"
#include "render/Material.h"

namespace engine
{
	MaterialGeneric::MaterialGeneric(std::shared_ptr<Asset<ShaderAssetVertex>> i_vertexShader, std::shared_ptr<Asset<ShaderAssetFragment>> i_fragmentShader)
		: m_vertexShader(std::move(i_vertexShader))
		, m_fragmentShader(std::move(i_fragmentShader))
	{

	}

	MaterialGeneric::~MaterialGeneric()
	{

	}
	MaterialLoadState MaterialGeneric::GetMaterialLoadState() const
	{
		AssetState fragmentState = m_fragmentShader->GetState();
		AssetState vertexState = m_vertexShader->GetState();
		if (fragmentState == AssetState::Unloaded || vertexState == AssetState::Unloaded)
		{
			return MaterialLoadState::Unloaded;
		}
		else if (fragmentState == AssetState::Loading || vertexState == AssetState::Loading)
		{
			return MaterialLoadState::Loading;
		}
		else
		{
			return MaterialLoadState::Ready;
		}
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
