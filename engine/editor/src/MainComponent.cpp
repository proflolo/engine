#include "StdAfx.h"
#include "MainComponent.h"

namespace editor
{
    MainComponent::MainComponent(const ProjectContext& i_context)
        : m_assetNavigator(i_context)
    {
        setSize(800, 600);
        addAndMakeVisible(m_view);
        addAndMakeVisible(m_assetNavigator);
    }


    MainComponent::~MainComponent()
    {

    }

    void MainComponent::resized()
    {
        UpdateLayout();
    }
    
    void MainComponent::UpdateLayout()
    {
        juce::Rectangle<int> bound = getBounds();
        m_view.setBounds(0, 0, bound.getWidth() - 300, bound.getHeight());
        m_assetNavigator.setBounds(bound.getWidth() - 300, 0, 300, bound.getHeight());
    }
}