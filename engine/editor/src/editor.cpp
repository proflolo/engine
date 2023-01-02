// editor.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//
#include "StdAfx.h"

#include "engine/GameModule.h"
#include "engine/definition/Definitionbase.hpp"
#include "render/opengl/RendererOpenGL.h"


class EngineView : public juce::OpenGLAppComponent
{
public:
    EngineView()
    {
        setSize(800, 600);
    }
    ~EngineView()
    {
        shutdownOpenGL();
    }
    void initialise() override
    {

    }

    void shutdown() override
    {
        bool stopped = m_stopSource.stop_requested();
    }

    void render() override
    {
        m_renderer.render(m_stopSource.get_token());
        glEnable(GL_BLEND);
    }

    void paint(juce::Graphics& g) override
    {
    
    }

private:
    engine::RendererOpenGL m_renderer;
    std::stop_source m_stopSource;
};

class MainComponent : public juce::Component
{
public:
    MainComponent()
    {
        setSize(800, 600);
        addAndMakeVisible(m_view);
        m_view.setBounds(50, 100, 200, 200);
    }
    ~MainComponent()
    {
       
    }
   

private:
    EngineView m_view;
};

class Editor : public juce::JUCEApplication
{
public:
    Editor() {}

    ~Editor() override
    {
        //sharedAudioDeviceManager.reset();
    }

    const juce::String getApplicationName() override { return "Editor"; }
    const juce::String getApplicationVersion() override { return "0.0.0"; }
    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise(const juce::String& commandLine) override
    {
        //juce::registerAllDemos();

        mainWindow.reset(new MainAppWindow(getApplicationName()));
    }

    //bool backButtonPressed() override { mainWindow->getMainComponent().getSidePanel().showOrHide(false); return true; }
    void shutdown() override { mainWindow = nullptr; }

    //==============================================================================
    void systemRequestedQuit() override { quit(); }
    void anotherInstanceStarted(const juce::String&) override {}

    juce::ApplicationCommandManager& getGlobalCommandManager() { return commandManager; }

private:
    class MainAppWindow : public juce::DocumentWindow
    {
    public:
        MainAppWindow(const juce::String& name)
            : DocumentWindow(name, juce::Desktop::getInstance().getDefaultLookAndFeel()
                .findColour(ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setResizable(true, false);
            setResizeLimits(400, 400, 10000, 10000);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);

            auto& desktop = Desktop::getInstance();

            desktop.setOrientationsEnabled(Desktop::allOrientations);
            desktop.setKioskModeComponent(this);
#else
            setBounds((int)(0.1f * (float)getParentWidth()),
                (int)(0.1f * (float)getParentHeight()),
                juce::jmax(850, (int)(0.5f * (float)getParentWidth())),
                juce::jmax(600, (int)(0.7f * (float)getParentHeight())));
#endif

            setContentOwned(new MainComponent(), false);
            setVisible(true);

            std::unique_ptr<engine::GameModule> gameModule = engine::CreateGame();
            ProcessDefinitions(gameModule->GetDefinitions());
            //for (const std::unique_ptr<engine::Module> subModules : gameModule->())
            //{
            //
            //}

#if JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX || JUCE_BSD
            //taskbarIcon.reset(new DemoTaskbarComponent());
#endif
        }

        void ProcessDefinitions(const engine::definitions& i_defs)
        {
            ;
            for (const editor::definition_def& def : i_defs.get_definition().list_defs())
            {
                std::string name = def.GetName();
            }
        }

        void closeButtonPressed() override { JUCEApplication::getInstance()->systemRequestedQuit(); }

#if JUCE_IOS || JUCE_ANDROID
        void parentSizeChanged() override
        {
            getMainComponent().resized();
        }
#endif

        //==============================================================================
        MainComponent& getMainComponent() { return *dynamic_cast<MainComponent*> (getContentComponent()); }

    private:
        std::unique_ptr<Component> taskbarIcon;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainAppWindow)
    };

    std::unique_ptr<MainAppWindow> mainWindow;
    juce::ApplicationCommandManager commandManager;
};

START_JUCE_APPLICATION(Editor)

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
