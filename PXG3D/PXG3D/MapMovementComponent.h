#pragma once
#include "Component.h"
#include "GLMHeaders.h"
#include "Vector3.h"
#include "Subscriber.h"
#include "RayCastShooter.h"
#include "TileMetaData.h"
#include "PXGNode.h"
#include <queue>
#include "GameObject.h"
#include "AbstractEventComponent.h"
#include "JumperComponent.h"
namespace PXG
{

	class MapMovementComponent : public Component, public subscriber_base, public subject_base
	{
	public:
		enum MovementCommands
		{
			FORWARD,
			BACKWARD,
			UP,
			DOWN,
			LEFT,
			RIGHT
		};
		enum event : event_t
		{
			ON_MOVE,
			ON_MOVE_START,
			ON_MOVE_FINISHED
		};
		void interpretDelta(int delta, MovementCommands a, MovementCommands b)
		{
			auto pusher = [&](int dp, MovementCommands pos, MovementCommands neg)
			{
				if (dp > 0)
				{
					for (; dp != 0; --dp)
					{
						commandQueue.push_front(pos);
					}
				}
				if (dp < 0)
				{
					for (; dp != 0; ++dp)
					{
						commandQueue.push_front(neg);
					}
				}
			};
			pusher(delta, a, b);
		}
		void Move(std::vector<Node*>* path)
		{
			Debug::Log("moving map");
			Node* oldNode = *path->begin();
			Vector3 startPos = oldNode->getPos();
			Vector3 endPos;
			for (auto Node : *path)
			{
				Debug::Log("iterating nodes");
				endPos = Node->getPos();
				Vector3 delta = startPos - endPos;
				offset = offset - delta;
				interpretDelta(delta.x, LEFT, RIGHT);
				interpretDelta(delta.y, UP, DOWN);
				interpretDelta(delta.z, FORWARD, BACKWARD);
				startPos = endPos;

			}
			Debug::Log("new offset:");
			Debug::Log(offset.ToString());
		}

		Vector3 getOffset() const { return offset; };
		Vector3 getOldOffset() const { return oldOffset; };
		Vector3 getTempNodePos() const { return tempNodePos; }

		void Start() override;
		void FixedUpdate(float tick) override;
		void SetMap(std::shared_ptr<GameObject> newMap);
		void Reset() const;
		void AddOtherObjectToMove(std::shared_ptr<GameObject> newObject);
		void AddSheep(std::shared_ptr<GameObject> newObject);
		Vector3 currentDir = Vector3{ 0,0,0 };
	private:
		bool move(PXG::Vector3 direction, bool restart, float factor, float tick, MovementCommands command);
		std::shared_ptr<GameObject> map;

		Vector3 offset = { 0,0,0 };
		Vector3 oldOffset = { 0,0,0 };
		Vector3 tempPlayerPos = { 0,0,0 };
		Vector3 tempOffset = { 0,0,0 };
		Vector3 tempNodePos = { 0,0,0 };
		Vector3 lastSheepPos = { 0,0,0 };
		Vector3 previousDir = { 0,0,0 };
		std::vector<std::shared_ptr<GameObject>> sheeps;
		std::vector<std::shared_ptr<GameObject>> otherObjectsToMove;
		std::deque<MovementCommands> commandQueue;
		virtual void onNotify(subject_base * subject_base, subject_base::event_t event) override;
	};
}
