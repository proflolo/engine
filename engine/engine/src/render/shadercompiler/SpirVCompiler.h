#pragma once

//https://lxjk.github.io/2020/03/10/Translate-GLSL-to-SPIRV-for-Vulkan-at-Runtime.html
namespace engine
{
	class SpirVCompiler
	{
	public:

		SpirVCompiler();

		~SpirVCompiler();


		enum class ShaderStage
		{
			Vertex,
			TessellationControl,
			TessellationEvaluation,
			Geometry,
			Fragment,
			Compute
		};

		std::vector<unsigned int> GLSLtoSPV(const ShaderStage shader_type, const std::string& pshader) const;
		std::pair<std::vector<unsigned int>, std::vector<unsigned int>> GLSLtoSPV(const std::string& vertexShader, const std::string& fragmentShader) const;
		std::string SPVtoGLSL(const std::vector<unsigned int>& i_spv) const;

	private:
		
	};

	
}