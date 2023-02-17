#pragma once

namespace engine
{
	struct RenderLayout
	{
		struct Field
		{
			unsigned int shaderLayout;
			unsigned int size;
			unsigned int stride;
			unsigned int start;
		};

		std::vector<Field> fields;


		template<typename T>
		static RenderLayout Create()
		{
			RenderLayout result;
			T sample;
			unsigned int layout = 0;
			unsigned int stride = sizeof(T);
			unsigned int start = 0;
			for_each_member<T>(sample, [&result, &layout, stride, &start](const auto& i_field)
			{
				using F = std::decay_t<decltype(i_field)>;
				RenderLayout::Field f = CreateField<F>(layout);
				f.stride = stride - sizeof(i_field);
				f.start = start;
				result.fields.emplace_back(f);
				layout++;
				start += sizeof(i_field);
			});
			return result;
		}

	private:
		template<typename F>
		static Field CreateField(unsigned int i_layout)
		{
			//Do not compile
		}

		template<>
		static Field CreateField<float>(unsigned int i_layout)
		{
			return Field{ i_layout, 1 };
		}

		template<>
		static Field CreateField<math::vector<float, 3>>(unsigned int i_layout)
		{
			return Field{ i_layout, 3 };
		}
	};
}