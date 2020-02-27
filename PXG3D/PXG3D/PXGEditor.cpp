#include "PXGEditor.h"
#include "World.h"
#include "Input.h"
#include "KeyCode.h"
#include "CameraComponent.h"
#include "RayCastShooter.h"
#include "FileConfig.h"
#include "TextureMaterial.h"
#include "Canvas.h"
#include <filesystem>
#include "ColorMaterial.h"
#include "TextComponent.h"
#include "ButtonComponent.h"
#include "imgui/imgui.h"
#include "CollisionCubeParams.h"
#include "PhysicsComponent.h"
#include <nlohmann/json.hpp>

namespace PXG {


    bool show_newBlockWindow = false;
    HitInfo info;

	ImVec2 mousePosAtClick;

    std::string current_model = "";
	std::string current_texture = "";


	class TileData : public Component
	{
	public:
        std::string model;
        std::string texture;
	};
	

	class window_enabler : public subscriber_base
	{
	public:
		void onNotify(subject_base* subject_base, subject_base::event_t event) override
		{
			if(show_newBlockWindow == false)
				if(event == RayCastShooter::ON_RAYCAST_HIT)
                {
                    mousePosAtClick = ImVec2(Input::GetMouseX(), Input::GetMouseY());
					show_newBlockWindow = true;
                    Debug::Log("Entered window_enabler::onNotify");

                    const auto shooter = static_cast<RayCastShooter*>(subject_base);
                    info = shooter->GetLastHit();
                }
		}
	} enabler;

    void PXGEditor::Initialize()
    {
        Input::AddKeysToTrack(
            KeyCode::LeftMouse, KeyCode::RightMouse);

        SetupCamera();


        map = MakeChildObj("map");
        Player = MakeChildObj("Player");


        Player->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "character.obj");
        Player->GetMeshComponent()->AddTextureToMeshAt({ config::PXG_INDEPENDENT_TEXTURES_PATH + "texture.png",TextureType::DIFFUSE }, 0);
        Player->GetTransform()->SetLocalPosition({ 0,100,0 });
        Player->GetTransform()->Scale(glm::vec3{ 100 });
        Player->GetMeshComponent()->SetMaterial(std::make_shared<TextureMaterial>());

    }

    void PXGEditor::Start()
    {
        world->Start();
        canvas->Start();
    }

    void PXGEditor::CreateEditorBlock(const std::string& model, const std::string& texture)
    {
        static int indexer = 0;

        indexer++;

        auto block = Instantiate();
    	block->name = model + texture + std::to_string(indexer);
        map->AddToChildren(block);


        auto tile_data = std::make_shared<TileData>();


        tile_data->texture = std::filesystem::path(current_texture).filename().string();
        tile_data->model = std::filesystem::path(current_model).filename().string();

        block->AddComponent(tile_data);
        block->GetMeshComponent()->Load3DModel(current_model);
        block->GetMeshComponent()->SetMaterial(std::make_shared<TextureMaterial>());
        block->GetMeshComponent()->AddTextureToMeshAt({ current_texture,TextureType::DIFFUSE }, 0);
        block->GetTransform()->SetLocalPosition(info.GameObjectHit->GetTransform()->GetPosition() + info.Normal.Normalize() * -100);

        Debug::Log("GetPosition() {}", info.GameObjectHit->GetTransform()->GetPosition().ToString());
        Debug::Log("Normal {}", info.Normal.ToString());

        block->GetTransform()->Scale(glm::vec3{ 100 });

        CollisionCubeParams cubeParams;
        cubeParams.heightFromMin = 1;
        block->GetPhysicsComponent()->ConstructCollisionCube(cubeParams);


        Debug::Log("hi {}",(info.GameObjectHit->GetTransform()->GetPosition() + info.Normal * 100).ToString());
    }

    void PXGEditor::SaveMap()
    {
        using json = nlohmann::json;

        json j;
    	
        auto tiles_array =json::array();

    	
        for(auto & child: map->GetChildren())
        {
            auto o = json::object();
            auto data = child->GetComponent<TileData>();

            auto pos = json::array();

        	for(size_t i = 0; i<3;++i)
        	{
                pos.push_back(std::round(child->GetTransform()->GetPosition()[i] / 100.0f));
        	}
            o["position"] = pos;
            o["model"] = data->model;
            o["texture"] = data->texture;
            o["meta-data"] = json::object({
                {"node",{}}
            });

        	

            tiles_array.push_back(o);
        	
        }

        j["tiles"] = tiles_array;


        Debug::Log("{}", j.dump());

    }


    void PXGEditor::SetupCamera()
    {
        auto mainCameraObject = MakeChildObj("main_camera");
        mainCameraObject->AddComponent(std::make_shared<CameraComponent>());

        auto shooter = std::make_shared<RayCastShooter>();
        mainCameraObject->AddComponent(shooter);
        shooter->attach(&enabler);




    	
        mainCameraObject->GetTransform()->SetLocalPosition(Vector3(600, 300, 600));
        mainCameraObject->GetTransform()->rotate(Vector3(1, 0, 0), -20.0f);
        mainCameraObject->GetTransform()->rotate(Vector3(0, 1, 0), 45);


    	auto uiCameraObject = MakeCanvasObj("ui_camera");
        uiCameraObject->AddComponent(std::make_shared<CameraComponent>());

        uiCameraObject->GetTransform()->SetLocalPosition(Vector3(300, 200, 0));

    }



    void PXGEditor::Update()
    {
        namespace f = std::filesystem;


    	//ImGui::ShowDemoWindow();
        ImGui::Begin("tile-selector");

        const  f::path model_directory_path(config::PXG_MODEL_PATH);

    	for(auto& file : f::directory_iterator(model_directory_path))
    	{
    		if(ImGui::Button(file.path().filename().string().c_str()))
    		{

                Player->GetMeshComponent()->Clear();
                Player->GetMeshComponent()->Load3DModel(file.path().string());

    			CollisionCubeParams cubeParams;
                cubeParams.heightFromMin = 1;
                Player->GetPhysicsComponent()->ConstructCollisionCube(cubeParams);

    			current_model = file.path().string();

    			Debug::Log("hi");
    		}
    	}
        ImGui::End();

        ImGui::Begin("texture-selector");


        const  f::path textures_directory_path(config::PXG_INDEPENDENT_TEXTURES_PATH);

        for (auto& file : f::directory_iterator(textures_directory_path))
        {
            if (ImGui::Button(file.path().filename().string().c_str()))
            {

                Player->GetMeshComponent()->ClearTextures(0);
                Player->GetMeshComponent()->AddTextureToMeshAt({ file.path().string(),TextureType::DIFFUSE }, 0);

                current_texture = file.path().string();

                Debug::Log("hi");
            }
        }
        ImGui::End();

    	if(show_newBlockWindow)
    	{
            ImGui::SetNextWindowPos(mousePosAtClick);
            ImGui::Begin("New Block",nullptr,ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoDecoration);
    		{
    			if(ImGui::Button("Make"))
    			{
                    Debug::Log("Stuff");
                    show_newBlockWindow = false;
                    CreateEditorBlock(current_model, current_texture);


    			}
            	if(ImGui::Button("Abort"))
            	{
                    show_newBlockWindow = false;
            	}
    		}
            ImGui::End();
    	}

        ImGui::Begin("file-dialog");
			if(ImGui::Button("Save"))
			{
                Debug::Log("Should save");
                SaveMap();
			}
        ImGui::End();


    }

    void PXGEditor::FixedUpdate(float tick)
    {
       // frender->add_queue(&persistent_queue);
        world->FixedUpdate(tick);
        canvas->FixedUpdate(tick);
    }

    GameObj PXGEditor::MakeChildObj(const std::string& name)
    {
        GameObj go = Instantiate();
        go->name = name;
        world->AddToChildren(go);
        return go;
    }

	GameObj PXGEditor::MakeCanvasObj(const std::string& name)
    {
        GameObj go = InstantiateUIObject();
        go->name = name;
        canvas->AddToChildren(go);
        return go;
    }
}
