#pragma once

namespace engine
{
	class TextureAssetPng: public engine::assetdefinition<TextureAssetPng, "Texture (png)", "png">
	{
	public:
		template<typename Func>
		static void visit_fields(const Func& i_func)
		{
			//visit_field(&Character::level2, i_func);
		}

		//field<bool, "wrap"> m_wrap;
	};

	class TextureAssetJpg : public engine::assetdefinition<TextureAssetJpg, "Texture (jpg)", "jpg">
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