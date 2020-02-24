#include "PhysicsEngine.h"
#include "PhysicsComponent.h"
#include "World.h"
#include "Mathf.h"
#include "MeshComponent.h"
#include "Vector3.h"
#include "HitInfo.h"
#include "Mat4.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Math.h"
#include "CameraComponent.h"
namespace PXG
{
	void PhysicsEngine::AddPhysicsComponent(std::shared_ptr<PhysicsComponent> physicsComponent)
	{
	}

	void PhysicsEngine::SetWorld(std::shared_ptr<World> world)
	{
		this->world = world;
	}

	float PhysicsEngine::GetTickRate() const
	{
		return tickTime;
	}

	float PhysicsEngine::GetCurrentTickRemaining() const
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
		//
		hitInfo.RayHit = false;
		hitInfo.DistanceFromRayPosition = FLT_MAX;
		hitInfo.T = FLT_MAX;


		//get all mesh Component af all objects
		std::vector<std::shared_ptr<MeshComponent>> meshComponents;
		RecursiveGetMeshComponents(meshComponents,world);

		for (auto const& meshComponent : meshComponents)
		{
			auto meshes = meshComponent->GetMeshes();
			auto gameObject = meshComponent->GetOwner();

			Mat4 worldTransform = gameObject->GetTransform()->GetWorldTransform();

			for (auto const& mesh : meshes)
			{
				RayToMeshIntersection(position, direction, hitInfo,mesh, worldTransform,gameObject);
			}
		}

		return hitInfo.RayHit;
	}

	void PhysicsEngine::RayToMeshIntersection(Vector3 position, Vector3 direction, HitInfo & hitInfo, std::shared_ptr<Mesh> mesh, Mat4 objectTransform, std::shared_ptr<GameObject> owner)
	{

		for (int index = 0; index < mesh->Indices.size(); index+=3)
		{

			int v0Index = mesh->Indices[index];
			int v1Index = mesh->Indices[index+1];
			int v2Index = mesh->Indices[index+2];

			HitInfo triangleHitInfo;
			triangleHitInfo.RayHit = false;

			RayTriangleIntersection(
				mesh->Vertices.at(v0Index).position,
				mesh->Vertices.at(v1Index).position,
				mesh->Vertices.at(v2Index).position,
				position,
				direction,
				objectTransform,
				triangleHitInfo,
				owner);

			if (triangleHitInfo.RayHit && triangleHitInfo.T < hitInfo.T)
			{
				hitInfo = triangleHitInfo;
			}


		}

	}

	void PhysicsEngine::RayTriangleIntersection(Vector3 vec1, Vector3 vec2, Vector3 vec3, Vector3 rayPosition, Vector3 rayDirection, Mat4 objectTransform, HitInfo& hitInfo, std::shared_ptr<GameObject> owner)
	{
		//-----find a point where the ray intersects the plane where the triangle lies-------//
		HitInfo Result;

		glm::vec4 rayOrigin(rayPosition.ToGLMVec3(), 1);
		glm::vec4 rayDir(-rayDirection.Normalized().ToGLMVec3(), 0);

		//transform ray into the triangle's model space
		rayOrigin =  glm::inverse(objectTransform.ToGLM()) * rayOrigin;
		rayDir = glm::normalize(glm::inverse(objectTransform.ToGLM()) * rayDir);

		Vector3 a(vec1.ToGLMVec3());
		Vector3 b(vec2.ToGLMVec3());
		Vector3 c(vec3.ToGLMVec3());

		Vector3 ObjectSpaceNormal =  Mathf::Cross(a - b, c - b).Normalized()*-1;


		auto transformedNormal = glm::transpose(glm::inverse(objectTransform.ToGLM())) * glm::vec4(ObjectSpaceNormal.ToGLMVec3(), 0);
		transformedNormal = glm::normalize(transformedNormal);

		Result.Normal = glm::vec3(transformedNormal.x, transformedNormal.y, transformedNormal.z);

		float normalDotDirection = Mathf::Dot(ObjectSpaceNormal, Vector3(rayDir.x, rayDir.y, rayDir.z));

		float normalDotPoint = Mathf::Dot(a, ObjectSpaceNormal);

		float OriginDotNormal = Mathf::Dot(Vector3(rayOrigin.x, rayOrigin.y, rayOrigin.z), ObjectSpaceNormal);

		float t = (normalDotPoint - OriginDotNormal) / normalDotDirection;

		//triangle is behind ray
		if (t < 0)
		{
			Result.RayHit = false;
			Result.T = t;
			hitInfo = Result;
			return;
		}


		glm::vec3 P = rayOrigin + rayDir * t;
		//---------check if that point is inside the the triangle using barycentric coordinates--------//

		Vector3 p(P.x, P.y, P.z);

		Vector3 v0 = b - a;
		Vector3 v1 = c - a;
		Vector3 v2 = p - a;

		float d00 = Mathf::Dot(v0, v0);
		float d10 = Mathf::Dot(v1, v0);
		float d01 = Mathf::Dot(v0, v1);
		float d11 = Mathf::Dot(v1, v1);
		float d20 = Mathf::Dot(v2, v0);
		float d21 = Mathf::Dot(v2, v1);

		float denom = d00 * d11 - d01 * d10;

		// apply cramers rule
		float v = (d20* d11 - d21 * d10) / denom;

		//---- P is in triangle if u + v + w = 1 ------
		if (v < 0 || v > 1)
		{
			Result.RayHit = false;
			Result.T = t;
			hitInfo = Result;
			return;
		}

		float w = (d00 * d21 - d01 * d20) / denom;

		if (w < 0 || w > 1)
		{
			Result.RayHit = false;
			Result.T = t;
			hitInfo = Result;
			return;
		}

		float u = 1 - w - v;

		if (u < 0 || u > 1 && (u + v +w) <= 1.0f)
		{
			Result.RayHit = false;
			Result.T = t;
			hitInfo = Result;
			return;
		}

		//ray hits triangle
		Result.Position = p;
		Result.GameObjectHit = owner;
		Result.RayHit = true;
		Result.T = t;

		hitInfo = Result;

	}


	void PhysicsEngine::RecursiveGetMeshComponents(std::vector<std::shared_ptr<MeshComponent>>& MeshComponentList, std::shared_ptr<GameObject> gameObject)
	{
		MeshComponentList.push_back(gameObject->GetMeshComponent());

		for (auto const& child : gameObject->GetChildren())
		{
			RecursiveGetMeshComponents(MeshComponentList, child);
		}
	}

	Vector3 PhysicsEngine::GetOrthographicCameraWorldPosition(float x, float y, float screenWidth, float screenHeight, std::shared_ptr<GameObject> object)
	{
		auto  transform = object->GetTransform();

		Vector3 position = transform->GetLocalPosition();
		Vector3 up = transform->GetUp();
		Vector3 right = transform->GetRight();

		Vector3 topLeftPosition = position - (right * float(screenWidth / 2.0f)) + (up * screenHeight / 2.0f);

		topLeftPosition = topLeftPosition + right * (x / screenWidth)*screenWidth - up * (y / screenHeight)*screenHeight;

		return topLeftPosition;
	}
}

