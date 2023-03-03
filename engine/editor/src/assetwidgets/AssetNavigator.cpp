#include "StdAfx.h"
#include "AssetNavigator.h"

namespace editor
{
    juce::String CreateExtensionList(const ProjectContext& i_projectContext)
    {
        juce::String result;
        for (const std::reference_wrapper<const asset_def>& assetDef : i_projectContext.assetTypes)
        {
            if (result.isEmpty())
            {
                result = "*.";
                result += assetDef.get().GetExtension().c_str();
            }
            else
            {
                result += ";*.";
                result += assetDef.get().GetExtension().c_str();
            }
        }
        return result;
    }

    AssetNavigator::AssetNavigator(const ProjectContext& i_projectContext)
        : imagesWildcardFilter(CreateExtensionList(i_projectContext), "*", "xxxxx")
    {
        imageList.setDirectory(juce::File::File::getCurrentWorkingDirectory(), true, true);
        directoryThread.startThread(juce::Thread::Priority::background);

        fileTree.setTitle("Files");
        fileTree.addListener(this);
        
        addAndMakeVisible(fileTree);
        UpdateLayout();
    }

    AssetNavigator::~AssetNavigator()
    {
        fileTree.removeListener(this);

    }
    
    void AssetNavigator::resized()
    {
        UpdateLayout();
    }
    
    void AssetNavigator::UpdateLayout()
    {
        juce::Rectangle<int> bounds = getBounds();
        fileTree.setBounds(0, 0, bounds.getWidth(), bounds.getHeight());
    }
}