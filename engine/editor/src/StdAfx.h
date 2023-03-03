#pragma once
#include <string>
#include "EditorStdAfx.h"

#include "juce_gui_basics/juce_gui_basics.h"
#include "juce_events/juce_events.h"
#include "juce_opengl/juce_opengl.h"

#include "EngineDefs.h"
#include "engine/EngineStdAfx.h"

#include "render/opengl/RendererOpenGL.h"

#include "definition/DefinitionDef.hpp"

namespace editor
{
	struct ProjectContext
	{
		std::vector<std::reference_wrapper<const asset_def>> assetTypes;
	};
}
