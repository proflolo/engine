#include "StdAfx.h"
#include "SpirVCompiler.h"
#include "glslang/SPIRV/GlslangToSpv.h"

#include "spirv_cross/spirv_glsl.hpp"
namespace engine
{
	SpirVCompiler::SpirVCompiler()
	{
		glslang::InitializeProcess();
	}
	
	SpirVCompiler::~SpirVCompiler()
	{
		glslang::FinalizeProcess();
	}
	
	void InitResources(TBuiltInResource& Resources) {
		Resources.maxLights = 32;
		Resources.maxClipPlanes = 6;
		Resources.maxTextureUnits = 32;
		Resources.maxTextureCoords = 32;
		Resources.maxVertexAttribs = 64;
		Resources.maxVertexUniformComponents = 4096;
		Resources.maxVaryingFloats = 64;
		Resources.maxVertexTextureImageUnits = 32;
		Resources.maxCombinedTextureImageUnits = 80;
		Resources.maxTextureImageUnits = 32;
		Resources.maxFragmentUniformComponents = 4096;
		Resources.maxDrawBuffers = 32;
		Resources.maxVertexUniformVectors = 128;
		Resources.maxVaryingVectors = 8;
		Resources.maxFragmentUniformVectors = 16;
		Resources.maxVertexOutputVectors = 16;
		Resources.maxFragmentInputVectors = 15;
		Resources.minProgramTexelOffset = -8;
		Resources.maxProgramTexelOffset = 7;
		Resources.maxClipDistances = 8;
		Resources.maxComputeWorkGroupCountX = 65535;
		Resources.maxComputeWorkGroupCountY = 65535;
		Resources.maxComputeWorkGroupCountZ = 65535;
		Resources.maxComputeWorkGroupSizeX = 1024;
		Resources.maxComputeWorkGroupSizeY = 1024;
		Resources.maxComputeWorkGroupSizeZ = 64;
		Resources.maxComputeUniformComponents = 1024;
		Resources.maxComputeTextureImageUnits = 16;
		Resources.maxComputeImageUniforms = 8;
		Resources.maxComputeAtomicCounters = 8;
		Resources.maxComputeAtomicCounterBuffers = 1;
		Resources.maxVaryingComponents = 60;
		Resources.maxVertexOutputComponents = 64;
		Resources.maxGeometryInputComponents = 64;
		Resources.maxGeometryOutputComponents = 128;
		Resources.maxFragmentInputComponents = 128;
		Resources.maxImageUnits = 8;
		Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		Resources.maxCombinedShaderOutputResources = 8;
		Resources.maxImageSamples = 0;
		Resources.maxVertexImageUniforms = 0;
		Resources.maxTessControlImageUniforms = 0;
		Resources.maxTessEvaluationImageUniforms = 0;
		Resources.maxGeometryImageUniforms = 0;
		Resources.maxFragmentImageUniforms = 8;
		Resources.maxCombinedImageUniforms = 8;
		Resources.maxGeometryTextureImageUnits = 16;
		Resources.maxGeometryOutputVertices = 256;
		Resources.maxGeometryTotalOutputComponents = 1024;
		Resources.maxGeometryUniformComponents = 1024;
		Resources.maxGeometryVaryingComponents = 64;
		Resources.maxTessControlInputComponents = 128;
		Resources.maxTessControlOutputComponents = 128;
		Resources.maxTessControlTextureImageUnits = 16;
		Resources.maxTessControlUniformComponents = 1024;
		Resources.maxTessControlTotalOutputComponents = 4096;
		Resources.maxTessEvaluationInputComponents = 128;
		Resources.maxTessEvaluationOutputComponents = 128;
		Resources.maxTessEvaluationTextureImageUnits = 16;
		Resources.maxTessEvaluationUniformComponents = 1024;
		Resources.maxTessPatchComponents = 120;
		Resources.maxPatchVertices = 32;
		Resources.maxTessGenLevel = 64;
		Resources.maxViewports = 16;
		Resources.maxVertexAtomicCounters = 0;
		Resources.maxTessControlAtomicCounters = 0;
		Resources.maxTessEvaluationAtomicCounters = 0;
		Resources.maxGeometryAtomicCounters = 0;
		Resources.maxFragmentAtomicCounters = 8;
		Resources.maxCombinedAtomicCounters = 8;
		Resources.maxAtomicCounterBindings = 1;
		Resources.maxVertexAtomicCounterBuffers = 0;
		Resources.maxTessControlAtomicCounterBuffers = 0;
		Resources.maxTessEvaluationAtomicCounterBuffers = 0;
		Resources.maxGeometryAtomicCounterBuffers = 0;
		Resources.maxFragmentAtomicCounterBuffers = 1;
		Resources.maxCombinedAtomicCounterBuffers = 1;
		Resources.maxAtomicCounterBufferSize = 16384;
		Resources.maxTransformFeedbackBuffers = 4;
		Resources.maxTransformFeedbackInterleavedComponents = 64;
		Resources.maxCullDistances = 8;
		Resources.maxCombinedClipAndCullDistances = 8;
		Resources.maxSamples = 4;
		Resources.maxMeshOutputVerticesNV = 256;
		Resources.maxMeshOutputPrimitivesNV = 512;
		Resources.maxMeshWorkGroupSizeX_NV = 32;
		Resources.maxMeshWorkGroupSizeY_NV = 1;
		Resources.maxMeshWorkGroupSizeZ_NV = 1;
		Resources.maxTaskWorkGroupSizeX_NV = 32;
		Resources.maxTaskWorkGroupSizeY_NV = 1;
		Resources.maxTaskWorkGroupSizeZ_NV = 1;
		Resources.maxMeshViewCountNV = 4;
		Resources.limits.nonInductiveForLoops = 1;
		Resources.limits.whileLoops = 1;
		Resources.limits.doWhileLoops = 1;
		Resources.limits.generalUniformIndexing = 1;
		Resources.limits.generalAttributeMatrixVectorIndexing = 1;
		Resources.limits.generalVaryingIndexing = 1;
		Resources.limits.generalSamplerIndexing = 1;
		Resources.limits.generalVariableIndexing = 1;
		Resources.limits.generalConstantMatrixVectorIndexing = 1;
	}

	EShLanguage FindLanguage(const SpirVCompiler::ShaderStage shader_type) {
		switch (shader_type) {
		case SpirVCompiler::ShaderStage::Vertex:
			return EShLangVertex;
		case SpirVCompiler::ShaderStage::TessellationControl:
			return EShLangTessControl;
		case SpirVCompiler::ShaderStage::TessellationEvaluation:
			return EShLangTessEvaluation;
		case SpirVCompiler::ShaderStage::Geometry:
			return EShLangGeometry;
		case SpirVCompiler::ShaderStage::Fragment:
			return EShLangFragment;
		case SpirVCompiler::ShaderStage::Compute:
			return EShLangCompute;
		default:
			return EShLangVertex;
		}
	}
	
	std::vector<unsigned int> SpirVCompiler::GLSLtoSPV(const ShaderStage shader_type, const std::string& pshader) const{
		
		std::vector<unsigned int> spirv;
		EShLanguage stage = FindLanguage(shader_type);
		glslang::TShader shader(stage);
		glslang::TProgram program;
		const char* shaderStrings[1];
		TBuiltInResource Resources = {};
		InitResources(Resources);

		// Enable SPIR-V and Vulkan rules when parsing GLSL
		EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

		shaderStrings[0] = pshader.c_str();
		shader.setStrings(shaderStrings, 1);

		if (!shader.parse(&Resources, 100, false, messages)) {
			auto infoLog = shader.getInfoLog();
			auto debugLog = shader.getInfoDebugLog();
			throw std::runtime_error("Could not parse shader");
		}

		program.addShader(&shader);

		//
		// Program-level processing...
		//

		if (!program.link(messages)) {
			puts(shader.getInfoLog());
			puts(shader.getInfoDebugLog());
			fflush(stdout);
			throw std::runtime_error("Could not link shader");
		}

		glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
		return spirv;
	}

	std::pair<std::vector<unsigned int>, std::vector<unsigned int>> SpirVCompiler::GLSLtoSPV(const std::string& vertexShader, const std::string& fragmentShader) const
	{
		glslang::TShader vShader(EShLangVertex);
		glslang::TShader fShader(EShLangFragment);
		glslang::TProgram program;

		TBuiltInResource Resources = {};
		InitResources(Resources);

		EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);
		{
			const char* shaderStrings[1];
			shaderStrings[0] = vertexShader.c_str();
			vShader.setStrings(shaderStrings, 1);

			if (!vShader.parse(&Resources, 100, false, messages)) {
				auto infoLog = vShader.getInfoLog();
				auto debugLog = vShader.getInfoDebugLog();
				throw std::runtime_error("Could not parse shader");
			}

			program.addShader(&vShader);
		}

		{
			const char* shaderStrings[1];
			shaderStrings[0] = fragmentShader.c_str();
			fShader.setStrings(shaderStrings, 1);

			if (!fShader.parse(&Resources, 100, false, messages)) {
				auto infoLog = fShader.getInfoLog();
				auto debugLog = fShader.getInfoDebugLog();
				throw std::runtime_error("Could not parse shader");
			}

			program.addShader(&fShader);
		}

		if (!program.link(messages)) {
			throw std::runtime_error("Could not link shader");
		}

		std::vector<unsigned int> vert, frag;
		glslang::GlslangToSpv(*program.getIntermediate(EShLangVertex), vert);
		glslang::GlslangToSpv(*program.getIntermediate(EShLangFragment), frag);
		return { vert, frag };

	}

	std::string SpirVCompiler::SPVtoGLSL(const std::vector<unsigned int>& spirv_binary) const
	{
		

		spirv_cross::CompilerGLSL glsl(std::move(spirv_binary));

		// The SPIR-V is now parsed, and we can perform reflection on it.
		spirv_cross::ShaderResources resources = glsl.get_shader_resources();

		// Get all sampled images in the shader.
		for (auto& resource : resources.sampled_images)
		{
			unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
			unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
			printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

			// Modify the decoration to prepare it for GLSL.
			glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

			// Some arbitrary remapping if we want.
			glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
		}

		// Set some options.
		spirv_cross::CompilerGLSL::Options options;
		//options.version = 310;
		//options.es = true;
		glsl.set_common_options(options);

		// Compile to GLSL, ready to give to GL driver.
		return glsl.compile();
	}


}