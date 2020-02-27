#include "Canvas.h"

namespace PXG
{

	Canvas::Canvas(){}

	GameObj Canvas::createCanvasObject( Vector2 position, Vector2 dimensions, std::string name, std::shared_ptr<AbstractMaterial>  material)
	{
		GameObj newGameObject = std::make_shared<GameObject>();
		newGameObject->GetMeshComponent()->Load3DModel(config::PXG_MODEL_PATH + "plane.obj");
		newGameObject->GetMeshComponent()->SetMaterial(material);
		newGameObject->GetTransform()->Scale(Vector3(dimensions.x, 0, dimensions.y));
		newGameObject->GetTransform()->SetLocalPosition(Vector3(position.x, position.y, -100));
		newGameObject->GetTransform()->rotate(glm::vec3(1, 0, 0), 90);
		return newGameObject;
	}

	GameObj Canvas::createEmptyCanvasObject()
	{
		GameObj newGameObject = std::make_shared<GameObject>();
		this->AddToChildren(newGameObject);
		return newGameObject;
	}
}

