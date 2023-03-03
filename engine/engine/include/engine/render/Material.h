#pragma once
#include "Resource.h"
#include "RenderResource.h"

namespace engine
{
	class ShaderAssetVertex;
	class ShaderAssetFragment;

	enum class MaterialLoadState
	{
		Ready,
		Loading,
		Unloaded
	};

	class MaterialGeneric: public Resource, public RenderResource<MaterialGeneric>
	{
	public:
		~MaterialGeneric();

		MaterialLoadState GetMaterialLoadState() const;
		void StartLoad(const Context& i_context);
		
		const std::optional<std::string&> GetVertexShaderCode() const;
		const std::optional<std::string&> GetFragmentShaderCode() const;

	protected:
		MaterialGeneric(std::shared_ptr<Asset<ShaderAssetVertex>> i_vertexShader, std::shared_ptr<Asset<ShaderAssetFragment>> i_fragmentShader);

	private:
		std::shared_ptr<Asset<ShaderAssetVertex>> m_vertexShader; 
		std::shared_ptr<Asset<ShaderAssetFragment>> m_fragmentShader;
	};

	template<typename Layout>
	class Material : public MaterialGeneric
	{
	public:
		inline Material(std::shared_ptr<Asset<ShaderAssetVertex>> i_vertexShader, std::shared_ptr<Asset<ShaderAssetFragment>> i_fragmentShader)
			: MaterialGeneric(std::move(i_vertexShader), std::move(i_fragmentShader))
		{

		}
	};
}