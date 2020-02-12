#pragma once
#include <memory>
#include <vector>



namespace PXG
{
	struct Vector3;
	struct HitInfo;
	class GameObject;
	class PhysicsComponent;
	class MeshComponent;
	class World;
	class Mesh;
	
	class PhysicsEngine
	{
	public:

		void AddPhysicsComponent(std::shared_ptr<PhysicsComponent> physicsComponent);

		void SetWorld(std::shared_ptr<World> world);

		float GetTickRate();

		float GetCurrentTickRemaining();

		void SetTickRemaining(float tick);

		bool IsTicking();

		float DecreaseRemainingTickTime();

		//cast a ray from a position to a direction. Uses the meshes of the mesh Component for intersection
		static bool DetailedRaycast(Vector3 position,Vector3 direction,HitInfo& hitInfo,std::shared_ptr<World> world);

	private:

		static bool RayToMeshIntersection(Vector3 position, Vector3 direction, HitInfo& hitInfo,std::shared_ptr<Mesh> mesh);

		static void RecursiveGetMeshComponents(std::vector<std::shared_ptr<MeshComponent>>& MeshComponentList, std::shared_ptr<GameObject> gameObject);

		//static bool RayTriangleIntersection();

		const float tickTime = 0.02f;

		float tickTimeRemaining;

		std::shared_ptr<World> world;


	};

}


