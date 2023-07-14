#include "StdAfx.h"
#include "EngineView.h"

namespace editor
{
    EngineView::EngineView()
    {
        setSize(800, 600);
        m_renderer.BeginRendering();
    }

    EngineView::~EngineView()
    {
        m_renderer.EndRendering();
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
        std::stop_token stopToken = m_stopSource.get_token();
        m_renderer.BeginFrame(stopToken);
        //m_renderClient.Render(i_stopToken, context);
        glEnable(GL_BLEND);
        m_renderer.EndFrame(stopToken);

    }

    void EngineView::paint(juce::Graphics& g)
    {

    }

}
