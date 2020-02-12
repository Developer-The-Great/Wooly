#include "PhysicsEngine.h"
#include "PhysicsComponent.h"
#include "World.h"
#include "Mathf.h"
#include "MeshComponent.h"
#include "Vector3.h"
#include "HitInfo.h"


namespace PXG
{
	void PhysicsEngine::AddPhysicsComponent(std::shared_ptr<PhysicsComponent> physicsComponent)
	{

	}

	void PhysicsEngine::SetWorld(std::shared_ptr<World> world)
	{
		this->world = world;
	}

	float PhysicsEngine::GetTickRate()
	{
		return tickTime;
	}

	float PhysicsEngine::GetCurrentTickRemaining()
	{
		return tickTimeRemaining;
	}

	void PhysicsEngine::SetTickRemaining(float tick)
	{
		tickTimeRemaining = tick;
	}

	bool PhysicsEngine::IsTicking()
	{
		if (tickTimeRemaining > 0.0f)
		{
			return true;
		}
		else
		{
			tickTimeRemaining = 0.0f;
			return false;
		}

	}

	float PhysicsEngine::DecreaseRemainingTickTime()
	{
		if (tickTimeRemaining > tickTime)
		{
			tickTimeRemaining -= tickTime;
			return tickTime;
		}
		else
		{
			float decreasedTime = tickTimeRemaining;

			tickTimeRemaining = 0;
			
			return  decreasedTime;
		}



	}

	bool PhysicsEngine::DetailedRaycast(Vector3 position, Vector3 direction, HitInfo& hitInfo, std::shared_ptr<World> world)
	{
		float smallestT = FLT_MAX;
		HitInfo info;
		bool somethingHit = false;

		
		//get all mesh Component af all objects
		std::vector<std::shared_ptr<MeshComponent>> meshComponents;
		RecursiveGetMeshComponents(meshComponents,world);
		//std::vector<std::shared_ptr<GameObject>
		//Debug::Log("")

		Debug::Log("meshComponents Found {0}", meshComponents.size());

		//for each MeshComponent in MeshComponents
		for (auto const& meshComponent : meshComponents)
		{

			//for(auto const& mesh : meshComponent->)


		}
			//for each mesh in MeshComponent.meshes

				//if(RayToMeshIntersection)
					//if currentTime < smallestT
						//store info



		return false;
	}

	/*bool PhysicsEngine::RayToMeshIntersection(Vector3 position, Vector3 direction, HitInfo & hitInfo, std::shared_ptr<Mesh> mesh)
	{



		return false;
	}*/

	bool PhysicsEngine::RayToMeshIntersection(Vector3 position, Vector3 direction, HitInfo & hitInfo, std::shared_ptr<Mesh> mesh)
	{
		return false;
	}

	void PhysicsEngine::RecursiveGetMeshComponents(std::vector<std::shared_ptr<MeshComponent>>& MeshComponentList, std::shared_ptr<GameObject> gameObject)
	{
		
		MeshComponentList.push_back(gameObject->GetMeshComponent());

		for (auto const& child : gameObject->GetChildren())
		{
			RecursiveGetMeshComponents(MeshComponentList, child);
		}
	}

	

}

