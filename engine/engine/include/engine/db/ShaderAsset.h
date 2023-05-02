#pragma once
#include "engine/definition/Definition.hpp"

namespace engine
{
	class ShaderAssetVertex: public engine::assetdefinition<ShaderAssetVertex, "Vertex Shader", "vs">
	{
	public:
		template<typename Func>
		static void visit_fields(const Func& i_func)
		{
			//visit_field(&Character::level2, i_func);
		}

		//field<bool, "wrap"> m_wrap;
	};

	class ShaderAssetFragment : public engine::assetdefinition<ShaderAssetFragment, "Fragment Shader", "fs">
	{
	public:
		template<typename Func>
		static void visit_fields(const Func& i_func)
		{
			//visit_field(&Character::level2, i_func);
		}

		//field<bool, "wrap"> m_wrap;
	};
}