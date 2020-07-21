#include "../../public/LunarEngine.h"

namespace LunarEngine {
    LunarEngine::LunarEngine() {
    }

    LunarEngine::~LunarEngine() {

    }

    void LunarEngine::InitResources() {
        renderer.InitResources();
        LunarRenderer::EngineRelinquish rel = renderer.RelinquishEngine();
        this->device = rel.device;
        this->window = rel.window;

        std::vector<LunarRenderer::Vertex> testQuad = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
        };

        std::vector<uint32_t> testIndicies = {
            0, 1, 2, 2, 3, 0
        };

        // attach
        manager = LunarRenderer::LayerManager(&renderer.globConstruct());
        _guiManager.PassResources(&renderer.globConstruct());

        LunarGUI::HTMLLoader indexPage("index.html");

        _guiManager.PassWindow(window);
        _guiManager.ConstructView(indexPage.HTMLContents(), glm::vec2(1280, 720), glm::vec2(640, 360), "index");
        _guiManager.ConstructLayout("epic");
        _guiManager.SetLayout("epic");

        _guiManager.RenderViews(0);
        _guiManager.UpdateUBOS(0);

        _guiManager.RenderViews(1);
        _guiManager.UpdateUBOS(1);

        manager.AttachEditor(&_guiManager);
        _input.AttachGUIManager(&_guiManager);

        renderer.AttachManager(&manager);

        LunarRenderer::ProjectionMatricies matricies = {};
        matricies.model = glm::mat4(1.0);
        matricies.proj = glm::perspective(glm::radians(45.0f), manager.swapChainExtent.width / (float) manager.swapChainExtent.height, 0.1f, 10.0f);

        manager.UpdateObjectUniform(matricies, "epic");

        renderer.CreateCommandBuffers();

        glfwSetCursorPosCallback(window, _input.CursorPositionCallback);
        glfwSetMouseButtonCallback(window, _input.MouseButtonCallback);
        glfwSetKeyCallback(window, LunarInput::WindowKeyCallback);
        glfwSetCharCallback(window, LunarInput::WindowCharCallback);
    }

    void LunarEngine::MainLoop() {
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        while(!glfwWindowShouldClose(window)) {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            glfwPollEvents();

            int currFrame = renderer.drawFrame();

            _guiManager.RenderDT(deltaTime, currFrame);
            manager.updateUniformBuffer();
        }

        vkDeviceWaitIdle(device);
        renderer.cleanup();
    }
}