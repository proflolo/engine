#pragma once

namespace editor
{
    class EngineView : public juce::OpenGLAppComponent
    {
    public:
        EngineView();
        ~EngineView();
        void initialise() override;

        void shutdown() override;

        void render() override;

        void paint(juce::Graphics& g) override;

    private:
        engine::RendererOpenGL m_renderer;
        std::stop_source m_stopSource;
    };
}