#pragma once
#include "Resource.h"
#include "RenderResource.h"
#include "RenderLayout.h"

namespace engine
{
	struct RenderLayout;

	class MeshGeneric: public Resource, public RenderResource<MeshGeneric>
	{
	public:
		~MeshGeneric();
		
		inline const std::basic_string_view<float>& GetData() const
		{
			return m_dataView;
		}

		inline const std::vector<unsigned int>& GetIndices() const
		{
			return m_indices;
		}


	protected:
		MeshGeneric(std::vector<unsigned int> i_indices);
		std::basic_string_view<float> m_dataView;
		std::vector<unsigned int> m_indices;

	};

	template<typename Layout>
	class Mesh : public MeshGeneric
	{
	public:
		inline const RenderLayout& GetLayout() const
		{
			return m_layout;
		}

	protected:
		Mesh(std::vector<unsigned int> i_indices)
			: MeshGeneric(std::move(i_indices))
			, m_layout(RenderLayout::Create<Layout>())
		{

		}

	private:
		RenderLayout m_layout;
	};

	template<typename Layout>
	class StaticMesh : public Mesh<Layout>
	{
	public:
		StaticMesh(std::vector<Layout> i_data, std::vector<unsigned int> i_indices)
			: Mesh<Layout>(std::move(i_indices))
			, m_buffer(std::move(i_data))
		{
			UpdateView();
		}

		StaticMesh(const StaticMesh<Layout>& i_other) = delete;

	private:
		void UpdateView()
		{
			this->m_dataView = std::basic_string_view<float>((const float*)&m_buffer[0], m_buffer.size() * sizeof(Layout) / sizeof(float));
		}

		std::vector<Layout> m_buffer;
	};
}