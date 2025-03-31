#include "InspectorScreen.h"

#include "ActionHistory.h"
#include "EngineBackend.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "UIManager.h"
#include "GameObjectManager.h"
#include "AComponent.h"
#include "MeshRenderer.h"
#include "PhysicsComponent.h"
#include "TextureComponent.h"
#include "TextureLibrary.h"

namespace GDEngine {
	InspectorScreen::InspectorScreen() : UIScreen("InspectorScreen")
	{
		m_meshDialog = new ImGui::FileBrowser();
		m_meshDialog->SetTitle("Select Mesh");
		m_meshDialog->SetTypeFilters({ ".obj" });
		Logger::log(this, "Initialized");

		this->m_position[0] = 0;
		this->m_position[1] = 0;
		this->m_position[2] = 0;
		this->m_rotation[0] = 0;
		this->m_rotation[1] = 0;
		this->m_rotation[2] = 0;
		this->m_scale[0] = 1;
		this->m_scale[1] = 1;
		this->m_scale[2] = 1;
	}

	InspectorScreen::~InspectorScreen()
	{
	}

	void InspectorScreen::draw()
	{
		ImGui::SetNextWindowSize(ImVec2(UIManager::WINDOW_WIDTH / 6, UIManager::WINDOW_HEIGHT), ImGuiCond_Once);
		ImGui::Begin("Inspector", &isActive);

		std::vector<AGameObject*> selectedObjects = GameObjectManager::getInstance()->getSelectedObjects();
		if (selectedObjects.size() > 1)
			this->drawSharedInspector(selectedObjects);
		else if (selectedObjects.size() == 1)
		{
			// reset positions
			this->m_position[0] = 0;
			this->m_position[1] = 0;
			this->m_position[2] = 0;
			this->m_rotation[0] = 0;
			this->m_rotation[1] = 0;
			this->m_rotation[2] = 0;
			this->m_scale[0] = 1;
			this->m_scale[1] = 1;
			this->m_scale[2] = 1;

			this->m_selectedObject = selectedObjects[0];
			this->drawInspector();
		}
		else {
			// reset positions
			this->m_position[0] = 0;
			this->m_position[1] = 0;
			this->m_position[2] = 0;
			this->m_rotation[0] = 0;
			this->m_rotation[1] = 0;
			this->m_rotation[2] = 0;
			this->m_scale[0] = 1;
			this->m_scale[1] = 1;
			this->m_scale[2] = 1;
		}

		/*m_selectedObject = GameObjectManager::getInstance()->getSelectedObject();
		if (m_selectedObject != nullptr)
		{
			this->drawInspector();
		}*/

		ImGui::End();
	}

	void InspectorScreen::drawInspector()
	{
		std::string name = m_selectedObject->getName();
		bool isActive = m_selectedObject->isActive();

		if (ImGui::Checkbox("##Active", &isActive))
		{
			m_selectedObject->setActive(isActive);
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::InputText("##Name", &name))
		{
			if (ImGui::IsItemDeactivatedAfterEdit())
				m_selectedObject->setName(name);
		}

		this->drawTransformTable(m_selectedObject);
		this->drawComponentList(m_selectedObject);

		if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowSize().x - 15, 20)))
		{
			ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
			ImGui::OpenPopup("SelectComponent");
		}
		ImVec2 buttonSize = ImGui::GetItemRectSize();
		buttonSize.x -= 17;
		buttonSize.y = 0;
		if (ImGui::BeginPopup("SelectComponent"))
		{
			if (ImGui::Selectable("Rigidbody", false, 0, buttonSize))
			{
				// ADD RIGIDBODY TO OBJECT
				m_selectedObject->setPhysics(true);
				m_selectedObject->attachComponent(new PhysicsComponent("PhysicsComponent " + m_selectedObject->getName(), m_selectedObject));
			}
			if (ImGui::Selectable("Texture", false, 0, buttonSize))
			{
				// ADD TEXTURE COMPONENT TO OBJECT
				m_selectedObject->attachComponent(new TextureComponent("TextureComponent " + m_selectedObject->getName(), m_selectedObject));
			}
			if (ImGui::Selectable("Mesh Renderer", false, 0, buttonSize))
			{
				// ADD MESH COMPONENT TO OBJECT
				m_selectedObject->attachComponent(new MeshRenderer("MeshRenderer " + m_selectedObject->getName(), m_selectedObject));
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x - 15, 20)))
		{
			GameObjectManager::getInstance()->setSelectedObject(nullptr);
			GameObjectManager::getInstance()->deleteObject(m_selectedObject);
		}
	}

	void InspectorScreen::drawSharedInspector(std::vector<AGameObject*> objects)
	{
		std::string name = " ";
		int commonActive = 0; int commonInactive = 0;
		for (AGameObject* obj : objects) {
			if (obj->isActive()) commonActive++;
			if (!obj->isActive()) commonInactive++;
		}
		bool isActive = ((commonActive > 0 && commonInactive == 0) || (commonActive == 0 && commonInactive > 0)) ? true : false;

		if (ImGui::Checkbox("##Active", &isActive))
		{
			for (AGameObject* obj : objects) {
				obj->setActive(isActive);
			}
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(-1);
		if (ImGui::InputText("##Name", &name))
		{
			if (ImGui::IsItemDeactivatedAfterEdit()) {
				for (AGameObject* obj : objects) {
					obj->setName(name);
				}
			}
		}

		this->drawSharedTransformTable(objects);
		//this->drawComponentList(m_selectedObject);

		//if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowSize().x - 15, 20)))
		//{
		//	ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
		//	ImGui::OpenPopup("SelectComponent");
		//}
		//ImVec2 buttonSize = ImGui::GetItemRectSize();
		//buttonSize.x -= 17;
		//buttonSize.y = 0;
		//if (ImGui::BeginPopup("SelectComponent"))
		//{
		//	if (ImGui::Selectable("Rigidbody", false, 0, buttonSize))
		//	{
		//		// ADD RIGIDBODY TO OBJECT
		//		m_selectedObject->setPhysics(true);
		//		m_selectedObject->attachComponent(new PhysicsComponent("PhysicsComponent " + m_selectedObject->getName(), m_selectedObject));
		//	}
		//	if (ImGui::Selectable("Texture", false, 0, buttonSize))
		//	{
		//		// ADD TEXTURE COMPONENT TO OBJECT
		//		m_selectedObject->attachComponent(new TextureComponent("TextureComponent " + m_selectedObject->getName(), m_selectedObject));
		//	}
		//	if (ImGui::Selectable("Mesh Renderer", false, 0, buttonSize))
		//	{
		//		// ADD MESH COMPONENT TO OBJECT
		//		m_selectedObject->attachComponent(new MeshRenderer("MeshRenderer " + m_selectedObject->getName(), m_selectedObject));
		//	}
		//	ImGui::EndPopup();
		//}

		if (ImGui::Button("Delete", ImVec2(ImGui::GetWindowSize().x - 15, 20)))
		{
			GameObjectManager::getInstance()->setSelectedObject(nullptr);
			GameObjectManager::getInstance()->deleteObject(m_selectedObject);
		}
	}

	void InspectorScreen::drawTransformTable(AGameObject* gameObject)
	{
		int rows = 3;

		std::string labels[] = { "Position", "Rotation", "Scale" };

		Vector3D vectorValues[3];
		vectorValues[0] = gameObject->getLocalPosition();
		vectorValues[1] = gameObject->getLocalRotation();
		vectorValues[2] = gameObject->getLocalScale();

		if (ImGui::BeginTable("Transform", 2, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthStretch);

			for (int i = 0; i < rows; i++)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text(labels[i].c_str());
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1);
				float values[3] = { vectorValues[i].x, vectorValues[i].y, vectorValues[i].z };
				std::string dragLabel = "##" + labels[i];

				if (ImGui::DragFloat3(dragLabel.c_str(), values, 0.01f))
				{
					if (!m_hasChanged)
					{
						ActionHistory::getInstance()->recordAction(gameObject);
					}

					m_hasChanged = true;

					switch (i)
					{
					case 0:
						gameObject->setPosition(Vector3D(values[0], values[1], values[2]));
						break;
					case 1:
						gameObject->setRotation(values[0], values[1], values[2]);
						break;
					case 2:
						gameObject->setScale(Vector3D(values[0], values[1], values[2]));
						break;
					}
				}

				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					m_isLeftDown = true;
				}
				else
				{
					m_isLeftDown = false;
				}

				if (m_hasChanged && !m_isLeftDown)
				{
					m_hasChanged = false;
				}
			}

			ImGui::EndTable();
		}
	}

	void InspectorScreen::drawSharedTransformTable(std::vector<AGameObject*> objects)
	{
		int rows = 3;

		std::string labels[] = { "Position", "Rotation", "Scale" };

		bool commonPos = true; bool commonRot = true; bool commonScale = true;
		Vector3D vectorValues[3];
		vectorValues[0] = objects[0]->getLocalPosition();
		vectorValues[1] = objects[0]->getLocalRotation();
		vectorValues[2] = objects[0]->getLocalScale();

		for (AGameObject* gameObject : objects) {
			if (vectorValues[0].x == gameObject->getLocalPosition().x &&
				vectorValues[0].y == gameObject->getLocalPosition().y &&
				vectorValues[0].z == gameObject->getLocalPosition().z &&
				commonPos)
				commonPos = true;
			else
				commonPos = false;

			if (vectorValues[1].x == gameObject->getLocalRotation().x &&
				vectorValues[1].y == gameObject->getLocalRotation().y &&
				vectorValues[1].z == gameObject->getLocalRotation().z &&
				commonRot)
				commonRot = true;
			else
				commonRot = false;

			if (vectorValues[2].x == gameObject->getLocalScale().x &&
				vectorValues[2].y == gameObject->getLocalScale().y &&
				vectorValues[2].z == gameObject->getLocalScale().z &&
				commonScale)
				commonScale = true;
			else
				commonScale = false;
		}

		if (ImGui::BeginTable("Transform", 2, ImGuiTableFlags_SizingFixedFit))
		{
			ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Values", ImGuiTableColumnFlags_WidthStretch);

			for (int i = 0; i < rows; i++)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text(labels[i].c_str());
				ImGui::TableNextColumn();
				ImGui::SetNextItemWidth(-1);
				float values[3] = { vectorValues[i].x, vectorValues[i].y, vectorValues[i].z };
				if (i == 0 && !commonPos)
				{
					values[0] = this->m_position[0];
					values[1] = this->m_position[1];
					values[2] = this->m_position[2];
				}if (i == 1 && !commonRot)
				{
					values[0] = this->m_rotation[0];
					values[1] = this->m_rotation[1];
					values[2] = this->m_rotation[2];
				}if (i == 2 && !commonScale)
				{
					values[0] = this->m_scale[0];
					values[1] = this->m_scale[1];
					values[2] = this->m_scale[2];
				}

				std::string dragLabel = "##" + labels[i];

				if (ImGui::DragFloat3(dragLabel.c_str(), values, 0.01f))
				{
					if (!m_hasChanged)
					{
						for (AGameObject* gameObject : objects) {
							ActionHistory::getInstance()->recordAction(gameObject);
						}
					}

					m_hasChanged = true;

					switch (i)
					{
					case 0:
						for (AGameObject* gameObject : objects) {
							gameObject->setPosition(Vector3D(values[0], values[1], values[2]));
						}
						break;
					case 1:
						for (AGameObject* gameObject : objects) {
							gameObject->setRotation(values[0], values[1], values[2]);
						}
						break;
					case 2:
						for (AGameObject* gameObject : objects) {
							gameObject->setScale(Vector3D(values[0], values[1], values[2]));
						}
						break;
					}
				}

				if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
				{
					m_isLeftDown = true;
				}
				else
				{
					m_isLeftDown = false;
				}

				if (m_hasChanged && !m_isLeftDown)
				{
					m_hasChanged = false;
				}
			}

			ImGui::EndTable();
		}
		
	}

	void InspectorScreen::drawComponentList(AGameObject* gameObject)
	{
		AGameObject::ComponentList componentList = gameObject->getComponents();
		for (AComponent* component : componentList)
		{
			if (component->getType() == AComponent::ComponentType::Physics)
			{
				PhysicsComponent* physicsComponent = dynamic_cast<PhysicsComponent*>(component);

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
				if (ImGui::CollapsingHeader("Rigidbody", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.800000190734863f);

					float mass = physicsComponent->getMass();
					float linearDrag = physicsComponent->getLinearDrag();
					float angularDrag = physicsComponent->getAngularDrag();
					bool gravity = physicsComponent->getUseGravity();

					ImGui::DragFloat("Mass", &mass);
					ImGui::DragFloat("Linear Drag", &linearDrag);
					ImGui::DragFloat("Angular Drag", &angularDrag);
					ImGui::Checkbox("Gravity", &gravity);

					BodyType bodyType = physicsComponent->getBodyType();
					const char* items[] = { "Static", "Kinematic", "Dynamic" };
					int item_selected_idx = static_cast<int>(bodyType); // Here we store our selection data as an index.
					// Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
					const char* combo_preview_value = items[item_selected_idx];

					if (ImGui::BeginCombo("Body Type", combo_preview_value))
					{
						for (int n = 0; n < IM_ARRAYSIZE(items); n++)
						{
							const bool is_selected = (item_selected_idx == n);
							if (ImGui::Selectable(items[n], is_selected))
								item_selected_idx = n;

							// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
							if (is_selected)
								ImGui::SetItemDefaultFocus();
						}
						ImGui::EndCombo();
					}

					if (ImGui::TreeNode("Constraints"))
					{
						bool freezePosX = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezePositionX);
						bool freezePosY = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezePositionY);
						bool freezePosZ = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezePositionZ);
						bool freezeRotX = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezeRotationX);
						bool freezeRotY = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezeRotationY);
						bool freezeRotZ = physicsComponent->getConstraint(PhysicsComponent::EConstraints::FreezeRotationZ);

						if (ImGui::BeginTable("ConstraintsTable", 4, ImGuiTableFlags_SizingFixedFit))
						{
							ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthStretch);
							ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthFixed);
							ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthFixed);
							ImGui::TableSetupColumn("Z", ImGuiTableColumnFlags_WidthFixed);

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::Text("Freeze Position");
							ImGui::TableNextColumn();
							ImGui::Checkbox("X##Pos", &freezePosX);
							ImGui::TableNextColumn();
							ImGui::Checkbox("Y##Pos", &freezePosY);
							ImGui::TableNextColumn();
							ImGui::Checkbox("Z##Pos", &freezePosZ);

							ImGui::TableNextColumn();
							ImGui::Text("Freeze Rotation");
							ImGui::TableNextColumn();
							ImGui::Checkbox("X##Rot", &freezeRotX);
							ImGui::TableNextColumn();
							ImGui::Checkbox("Y##Rot", &freezeRotY);
							ImGui::TableNextColumn();
							ImGui::Checkbox("Z##Rot", &freezeRotZ);

							ImGui::EndTable();
						}
						ImGui::TreePop();

						physicsComponent->setConstraints(
							static_cast<PhysicsComponent::EConstraints>(
								freezePosX << 0
								| freezePosY << 1
								| freezePosZ << 2
								| freezeRotX << 3
								| freezeRotY << 4
								| freezeRotZ << 5));
					}

					physicsComponent->setMass(mass);
					physicsComponent->setLinearDrag(linearDrag);
					physicsComponent->setAngularDrag(angularDrag);
					physicsComponent->setUseGravity(gravity);
					physicsComponent->setBodyType(static_cast<BodyType>(item_selected_idx));

					std::string buttonName = "Delete##" + component->getName();
					if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowSize().x - 15, 20)))
					{
						gameObject->detachComponent(component);
						gameObject->setPhysics(false);
					}
					ImGui::PopStyleVar();
				}
				ImGui::PopStyleVar();
				ImGui::Separator();
			}


			if (component->getType() == AComponent::ComponentType::Tex)
			{
				TextureComponent* textureComponent = dynamic_cast<TextureComponent*>(component);

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
				if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.800000190734863f);

					//DRAW TEXTURE COMPONENT UI
					if (textureComponent->getTexture() != nullptr)
						ImGui::Image((ImTextureID)textureComponent->getTexture()->m_shaderResourceView, ImVec2(128, 128));
					else
						ImGui::TextColored(ImVec4(1, 0, 0, 1), "Invalid File Path!");

					/*ImGui::InputText("File Path", &textureComponent->filePath);
					if (ImGui::Button("Set Texture"))
						textureComponent->setTexture(textureComponent->filePath);*/
					int texIndex = textureComponent->textureIndex;
					std::string displayName;
					std::string dropdownName = "Texture " + std::to_string(texIndex);
					if (ImGui::BeginCombo("##SelectTexture", dropdownName.c_str()))
					{
						for (size_t i = 0; i < TextureLibrary::getInstance()->getTextureLibrary().size(); i++)
						{
							bool isSelected = (texIndex == static_cast<int>(i));

							displayName = "Texture " + std::to_string(i + 1);
							if (ImGui::ImageButton(displayName.c_str(), (ImTextureID)TextureLibrary::getInstance()->getTextureLibrary()[(TextureName)i]->m_shaderResourceView, ImVec2(64, 64)))
							{
								texIndex = static_cast<int>(i);
								textureComponent->setTexture((TextureName)i);
								textureComponent->textureIndex = i;
								dropdownName = displayName;
							}
							
							if (isSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					std::string buttonName = "Delete##" + component->getName();
					if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowSize().x - 15, 20)))
					{
						textureComponent->setTexture(TextureName::DEFAULT);
						gameObject->detachComponent(component);
						gameObject->setHasTexture(false);
					}

					ImGui::PopStyleVar();
				}
				ImGui::PopStyleVar();
				ImGui::Separator();
			}

			if (component->getType() == AComponent::ComponentType::Renderer)
			{
				MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(component);

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0);
				if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 7.800000190734863f);

					std::string buttonName = "Set Mesh##" + component->getName();
					if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowSize().x - 15, 20)))
					{
						m_meshDialog->Open();
					}
					buttonName = "Delete##" + component->getName();
					if (ImGui::Button(buttonName.c_str(), ImVec2(ImGui::GetWindowSize().x - 15, 20)))
					{
						gameObject->detachComponent(component);
						gameObject->setHasTexture(false);
					}

					ImGui::PopStyleVar();
				}

				if (m_meshDialog->HasSelected())
				{
					std::string selected = m_meshDialog->GetSelected().string();
					std::wstring wide = std::wstring(selected.begin(), selected.end());

					meshRenderer->setMeshFromFile(wide.c_str());

					m_meshDialog->ClearSelected();
					m_meshDialog->Close();
				}

				ImGui::PopStyleVar();
				ImGui::Separator();
			}
		}
		m_meshDialog->Display();

		
	}
}