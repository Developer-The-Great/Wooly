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
	struct Mat4;
	
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

		static Vector3 GetOrthographicCameraWorldPosition(float i, float j, float screenWidth, float screenHeight, std::shared_ptr<GameObject> object);

	private:

		static void RayToMeshIntersection(Vector3 position, Vector3 direction, HitInfo& hitInfo,std::shared_ptr<Mesh> mesh, Mat4 objectTransform,std::shared_ptr<GameObject> owner);

		static void RecursiveGetMeshComponents(std::vector<std::shared_ptr<MeshComponent>>& MeshComponentList, std::shared_ptr<GameObject> gameObject);

		static void RayTriangleIntersection(Vector3 v1,Vector3 v2,Vector3 v3 , Vector3 rayPosition, Vector3 rayDirection, Mat4 objectTransform, HitInfo& hitInfo, std::shared_ptr<GameObject> owner);

		const float tickTime = 0.02f;

		float tickTimeRemaining;

		std::shared_ptr<World> world;


	};

}


