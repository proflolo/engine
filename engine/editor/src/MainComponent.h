#pragma once
#include "EngineView.h"
#include "assetwidgets/AssetNavigator.h"

namespace editor
{
    class MainComponent : public juce::Component
    {
    public:
        MainComponent(const ProjectContext& i_context);
        ~MainComponent();

        void resized() override;

    private:
        void UpdateLayout();
        EngineView m_view;
        AssetNavigator m_assetNavigator;
    };
}