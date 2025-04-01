#pragma comment(lib, "rpcrt4.lib")

#include "HierarchyScreen.h"

#include "imgui.h"
#include "UIManager.h"
#include "GameObjectManager.h"

namespace GDEngine
{
HierarchyScreen::HierarchyScreen() : UIScreen("HierarchyScreen")
{
	Logger::log(this, "Initialized");
}

HierarchyScreen::~HierarchyScreen()
{
}

void HierarchyScreen::draw()
{
	ImGui::SetNextWindowSize(ImVec2(UIManager::WINDOW_WIDTH / 6, UIManager::WINDOW_HEIGHT), ImGuiCond_Once);
	ImGui::Begin("Hierarchy", &isActive);
	
    std::string query = "";
    static char searchQuery[128] = "";
    ImGui::InputTextWithHint("Search", "Enter Game Object name.", searchQuery, IM_ARRAYSIZE(searchQuery));
    query = searchQuery;

    if (GameObjectManager::getInstance()->getMultiselectMode())
        ImGui::Text("Multiselect mode On");
    else ImGui::Text("Multiselect mode Off");

    std::vector<AGameObject*> objList = GameObjectManager::getInstance()->getAllObjects();
    std::vector<AGameObject*> viewables;

    for (AGameObject* obj : objList)
    {
        std::string name = obj->getName();
        bool searchFound = name.find(query) != std::string::npos;
        searchFound = this->contains(name, query);

        if (searchFound && obj->getParent() == nullptr)
        {
            displayNode(obj);
            viewables.push_back(obj);
        }
    }

    GameObjectManager::getInstance()->setViewableObjects(viewables);

	ImGui::End();
}

//void HierarchyScreen::displayNode(AGameObject* gameObject)
//{
//    std::string guidString = gameObject->getGuidString();
//    std::string label = gameObject->getName() + "###" + guidString;
//
//    bool hasChildren = !gameObject->getChildren().empty();
//    bool isExpanded = m_expandedState[guidString];
//
//    static bool hasValidDropTarget = false;
//
//    AGameObject* selectedObject = GameObjectManager::getInstance()->getSelectedObject();
//
//    if (hasChildren)
//    {
//        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
//        if (selectedObject == gameObject)
//            flags |= ImGuiTreeNodeFlags_Selected;
//        if (isExpanded)
//            flags |= ImGuiTreeNodeFlags_DefaultOpen;
//
//        if (ImGui::TreeNodeEx(label.c_str(), flags))
//        {
//            if (ImGui::IsItemClicked())
//            {
//                GameObjectManager::getInstance()->setSelectedObject(gameObject);
//            }
//
//            // Start Drag-and-Drop Source
//            if (ImGui::BeginDragDropSource())
//            {
//                GameObjectManager::getInstance()->setSelectedObject(gameObject);
//                ImGui::SetDragDropPayload("PARENTING", &gameObject, sizeof(AGameObject*));
//
//                hasValidDropTarget = false;
//
//                ImGui::EndDragDropSource();
//            }
//
//            // Handle Drop Target
//            if (ImGui::BeginDragDropTarget())
//            {
//                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENTING"))
//                {
//                    AGameObject* draggedObject = *(AGameObject**)payload->Data;
//
//                    if (draggedObject && draggedObject != gameObject && !gameObject->isDescendant(draggedObject))
//                    {
//                        draggedObject->setParent(gameObject);
//                        m_expandedState[guidString] = true;
//                        hasValidDropTarget = true;
//                    }
//                }
//
//                ImGui::EndDragDropTarget();
//            }
//
//            // Recursively display child nodes
//            for (AGameObject* child : gameObject->getChildren())
//            {
//                displayNode(child);
//            }
//
//            ImGui::TreePop();
//        }
//
//        if (ImGui::IsItemHovered())
//        {
//            m_expandedState[guidString] = true;
//        }
//    }
//    else
//    {
//        if (ImGui::Selectable(label.c_str(), GameObjectManager::getInstance()->getSelectedObject() == gameObject))
//        {
//            GameObjectManager::getInstance()->setSelectedObject(gameObject);
//        }
//
//        // Start Drag-and-Drop Source
//        if (ImGui::BeginDragDropSource())
//        {
//            GameObjectManager::getInstance()->setSelectedObject(gameObject);
//            ImGui::SetDragDropPayload("PARENTING", &gameObject, sizeof(AGameObject*));
//            hasValidDropTarget = false;
//            ImGui::EndDragDropSource();
//        }
//
//        // Handle Drop Target for leaf nodes
//        if (ImGui::BeginDragDropTarget())
//        {
//            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENTING"))
//            {
//                AGameObject* draggedObject = *(AGameObject**)payload->Data;
//
//                if (draggedObject && draggedObject != gameObject && !gameObject->isDescendant(draggedObject))
//                {
//                    draggedObject->setParent(gameObject);
//                    m_expandedState[guidString] = true;
//                    hasValidDropTarget = true;
//                }
//            }
//
//            ImGui::EndDragDropTarget();
//        }
//    }
//
//    // Dropped to hierarchy screen
//    if (ImGui::IsWindowHovered() && ImGui::IsMouseReleased(0) && !ImGui::IsAnyItemHovered() && !hasValidDropTarget)
//    {
//        AGameObject* selectedObject = GameObjectManager::getInstance()->getSelectedObject();
//
//        if (selectedObject)
//        {
//            Logger::log("Detached: " + selectedObject->getName() + " from its parent.");
//
//            selectedObject->setParent(nullptr);
//
//            selectedObject->setPosition(selectedObject->getWorldPosition());
//            selectedObject->setRotation(selectedObject->getWorldRotation());
//            selectedObject->setScale(selectedObject->getWorldScale());
//
//        }
//    }
//}

void HierarchyScreen::displayNode(AGameObject* gameObject)
{
    std::string guidString = gameObject->getGuidString();
    std::string label = gameObject->getName() + "###" + guidString;

    bool hasChildren = !gameObject->getChildren().empty();
    bool isExpanded = m_expandedState[guidString];

    static bool hasValidDropTarget = false;

    AGameObject* selectedObject = GameObjectManager::getInstance()->getSelectedObject();

    if (hasChildren)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (GameObjectManager::getInstance()->isSelected(gameObject))
            flags |= ImGuiTreeNodeFlags_Selected;
        if (isExpanded)
            flags |= ImGuiTreeNodeFlags_DefaultOpen;

        if (ImGui::TreeNodeEx(label.c_str(), flags))
        {
            if (ImGui::IsItemClicked())
            {
                GameObjectManager::getInstance()->setSelectedObject(gameObject);
            }

            // Start Drag-and-Drop Source
            if (ImGui::BeginDragDropSource())
            {
                GameObjectManager::getInstance()->setSelectedObject(gameObject);
                ImGui::SetDragDropPayload("PARENTING", &gameObject, sizeof(AGameObject*));

                hasValidDropTarget = false;

                ImGui::EndDragDropSource();
            }

            // Handle Drop Target
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENTING"))
                {
                    AGameObject* draggedObject = *(AGameObject**)payload->Data;

                    if (draggedObject && draggedObject != gameObject && !gameObject->isDescendant(draggedObject))
                    {
                        draggedObject->setParent(gameObject);
                        m_expandedState[guidString] = true;
                        hasValidDropTarget = true;
                    }
                }

                ImGui::EndDragDropTarget();
            }

            // Recursively display child nodes
            for (AGameObject* child : gameObject->getChildren())
            {
                displayNode(child);
            }

            ImGui::TreePop();
        }

        if (ImGui::IsItemHovered())
        {
            m_expandedState[guidString] = true;
        }
    }
    else
    {
        if (ImGui::Selectable(label.c_str(), GameObjectManager::getInstance()->isSelected(gameObject)))
        {
            GameObjectManager::getInstance()->setSelectedObject(gameObject);
        }

        // Start Drag-and-Drop Source
        if (ImGui::BeginDragDropSource())
        {
            GameObjectManager::getInstance()->setSelectedObject(gameObject);
            ImGui::SetDragDropPayload("PARENTING", &gameObject, sizeof(AGameObject*));
            hasValidDropTarget = false;
            ImGui::EndDragDropSource();
        }

        // Handle Drop Target for leaf nodes
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PARENTING"))
            {
                AGameObject* draggedObject = *(AGameObject**)payload->Data;

                if (draggedObject && draggedObject != gameObject && !gameObject->isDescendant(draggedObject))
                {
                    draggedObject->setParent(gameObject);
                    m_expandedState[guidString] = true;
                    hasValidDropTarget = true;
                }
            }

            ImGui::EndDragDropTarget();
        }
    }

    // Dropped to hierarchy screen
    if (ImGui::IsWindowHovered() && ImGui::IsMouseReleased(0) && !ImGui::IsAnyItemHovered() && !hasValidDropTarget)
    {
        AGameObject* selectedObject = GameObjectManager::getInstance()->getSelectedObject();

        if (selectedObject)
        {
            Logger::log("Detached: " + selectedObject->getName() + " from its parent.");

            selectedObject->setParent(nullptr);

            selectedObject->setPosition(selectedObject->getWorldPosition());
            selectedObject->setRotation(selectedObject->getWorldRotation());
            selectedObject->setScale(selectedObject->getWorldScale());

        }
    }
}

bool HierarchyScreen::contains(std::string str, std::string substr)
{
    std::string lowerStr;
    for (char c : str) {
        lowerStr += tolower(c);
    }

    std::string lowerSubstr;
    for (char c : substr) {
        lowerSubstr += tolower(c);
    }


    if (lowerStr.find(lowerSubstr) != std::string::npos) {
        return true;
    }

    return false;
}

}

