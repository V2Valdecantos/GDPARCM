#include "MenuScreen.h"

#include "ActionHistory.h"
#include "imgui.h"
#include "UIManager.h"
#include "GraphicsEngine.h"
#include "GameObjectManager.h"
#include "SceneReader.h"
#include "SceneWriter.h"
#include "ViewportManager.h"
#include "StreamingManager.h"

namespace GDEngine {
    MenuScreen::MenuScreen() : UIScreen("MenuScreen")
    {
        m_openSceneDialog = new ImGui::FileBrowser();
        m_openSceneDialog->SetTitle("Open Scene");
        m_openSceneDialog->SetTypeFilters({".txt" });

        m_saveSceneDialog = new ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename);
        m_saveSceneDialog->SetTitle("Save Scene");
        m_saveSceneDialog->SetTypeFilters({".txt" });
        Logger::log(this, "Initialized");
    }

    MenuScreen::~MenuScreen()
    {
        delete m_openSceneDialog;
        delete m_saveSceneDialog;
    }

    void MenuScreen::draw()
    {

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Create")) {
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                    m_openSceneDialog->Open();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    m_saveSceneDialog->Open();
                }
                if (ImGui::MenuItem("Save as..")) {
                    m_saveSceneDialog->Open();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
                    if (ActionHistory::getInstance()->hasRemainingUndoActions())
                    {
                        GameObjectManager::getInstance()->applyAction(ActionHistory::getInstance()->undoAction());
                    }
                    else Logger::log("Empty Undo");
                }
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) {
                    if (ActionHistory::getInstance()->hasRemainingRedoActions())
                    {
                        GameObjectManager::getInstance()->applyAction(ActionHistory::getInstance()->redoAction());
                    }
                    else Logger::log("Empty Redo");
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Game Object"))
            {
                if (ImGui::BeginMenu("3D Object"))
                {
                    RenderSystem* renderSystem = GraphicsEngine::getInstance()->getRenderSystem();

                    // Primitives menu
                    if (ImGui::BeginMenu("Primitives"))
                    {
                        if (ImGui::MenuItem("Cube"))
                        {
                            GameObjectManager::getInstance()->createCube();
                        }
                        if (ImGui::MenuItem("Plane"))
                        {
                            GameObjectManager::getInstance()->createPlane();
                        }
                        if (ImGui::MenuItem("Sphere"))
                        {
                            GameObjectManager::getInstance()->createTexturedSphere();
                        }
                        if (ImGui::MenuItem("Capsule"))
                        {
                            GameObjectManager::getInstance()->createTexturedCapsule();
                        }
                        if (ImGui::MenuItem("Cylinder"))
                        {
                            GameObjectManager::getInstance()->createTexturedCylinder();
                        }
                        if (ImGui::MenuItem("Quad"))
                        {
                            GameObjectManager::getInstance()->createQuad();
                        }
                        ImGui::EndMenu();  // Close Primitives menu
                    }

                    // Models menu
                    if (ImGui::BeginMenu("Models"))
                    {
                        if (ImGui::MenuItem("Teapot")) {
                            GameObjectManager::getInstance()->createTeapot();
                        }
                        if (ImGui::MenuItem("Bunny")) {
                            GameObjectManager::getInstance()->createBunny();
                        }
                        if (ImGui::MenuItem("Armadillo")) {
                            GameObjectManager::getInstance()->createArmadillo();
                        }
                        if (ImGui::MenuItem("Lucy")) {
                            GameObjectManager::getInstance()->createLucy();
                        }
                        ImGui::EndMenu();  // Close Models menu
                    }
                    if (ImGui::BeginMenu("Physics Objects"))
                    {
                        if (ImGui::MenuItem("100 Physics Cube")) {
                            for (int i = 0; i < 100; i++)
                            {
                                GameObjectManager::getInstance()->createPhysicsCube();
                            }
                        }
                        if (ImGui::MenuItem("Physics Plane")) {
                            GameObjectManager::getInstance()->createPhysicsPlane();
                        }
                        ImGui::EndMenu();
                    }
                    
                    if (ImGui::BeginMenu("Object Streaming"))
                    {
                        if (ImGui::MenuItem("Scene 1")) 
                        {
                            if (!StreamingManager::getInstance()->sceneFlags[0])
                                StreamingManager::getInstance()->startStreamingScene(0);
                            else
                                StreamingManager::getInstance()->sceneFlags[0] = false;
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();  // Close 3D Object menu
                }
                ImGui::EndMenu();  // Close Game Object menu
            }

            if (ImGui::BeginMenu("Window")) {
                if (ImGui::MenuItem("Inspector")) {
                    UIManager::getInstance()->setActive("INSPECTOR_SCREEN");
                }
                if (ImGui::MenuItem("Hierarchy")) {
                    UIManager::getInstance()->setActive("HIERARCHY_SCREEN");
                }
                if (ImGui::MenuItem("Profiler")) {
                    UIManager::getInstance()->setActive("PROFILER_SCREEN");
                }
                if (ImGui::MenuItem("Color Picker")) {
                    UIManager::getInstance()->setActive("COLOR_PICKER_SCREEN");
                }
                if (ImGui::BeginMenu("Viewport"))
                {
                    if (ImGui::MenuItem("Create Viewport"))
                    {
                        ViewportManager::getInstance()->createViewport();
                    }
                    if (ImGui::MenuItem("Single Viewport"))
                    {
                        ViewportManager::getInstance()->setNumViewports(1);
                    }
                    if (ImGui::MenuItem("2 Viewports"))
                    {
                        ViewportManager::getInstance()->setNumViewports(2);
                    }
                    if (ImGui::MenuItem("3 Viewports"))
                    {
                        ViewportManager::getInstance()->setNumViewports(3);
                    }
                    if (ImGui::MenuItem("4 Viewports"))
                    {
                        ViewportManager::getInstance()->setNumViewports(4);
                    }
                    if (ImGui::MenuItem("Delete All Viewports"))
                    {
                        ViewportManager::getInstance()->deleteAllViewports();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                if (ImGui::BeginMenu("About")) {
                    if (ImGui::MenuItem("Credits")) {
                        UIManager::getInstance()->setActive("CREDITS_SCREEN");
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        m_openSceneDialog->Display();
        m_saveSceneDialog->Display();

        if (m_saveSceneDialog->HasSelected())
        {
            SceneWriter writer = SceneWriter(m_saveSceneDialog->GetSelected().string());
            writer.writeToFile();

            m_saveSceneDialog->ClearSelected();
            m_saveSceneDialog->Close();
        }

        else if (m_openSceneDialog->HasSelected()) {
            SceneReader reader = SceneReader(m_openSceneDialog->GetSelected().string());
            reader.readFromFile();

            m_openSceneDialog->ClearSelected();
            m_openSceneDialog->Close();
        }
    }
}