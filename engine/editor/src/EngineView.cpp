#include "StdAfx.h"
#include "EngineView.h"

namespace editor
{
    EngineView::EngineView()
    {
        setSize(800, 600);
    }

    EngineView::~EngineView()
    {
        shutdownOpenGL();
    }

    void EngineView::initialise()
    {

    }

    void EngineView::shutdown()
    {
        bool stopped = m_stopSource.stop_requested();
    }

    void EngineView::render()
    {
        m_renderer.BeginRender(m_stopSource.get_token());
        glEnable(GL_BLEND);

        m_renderer.EndRender(m_stopSource.get_token());

    }

    void EngineView::paint(juce::Graphics& g)
    {

    }

}
