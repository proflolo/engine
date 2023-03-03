#pragma once
#include "engine/definition/Definitionbase.hpp"
#include "engine/definition/Definition.hpp"
#include "TextureAsset.h"
#include "ShaderAsset.h"

namespace engine
{
	class EngineDb : public engine::definitionbase<EngineDb>
	{
	public:
		template<typename Func>
		static void visit_assets(const Func& i_func)
		{
			visit_type<TextureAssetPng>(i_func);
			visit_type<TextureAssetJpg>(i_func);
			visit_type<ShaderAssetVertex>(i_func);
			visit_type<ShaderAssetFragment>(i_func);
		}

		template<typename Func>
		static void visit_types(const Func& i_func)
		{

		}
	};
}