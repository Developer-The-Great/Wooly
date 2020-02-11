#pragma once
#include "Component.h"
#include "Mat4.h"

namespace PXG
{


	class CameraComponent : public Component, public std::enable_shared_from_this<CameraComponent>
	{
	public:

		CameraComponent();

		~CameraComponent() override;
		virtual void Start() override;

		virtual void FixedUpdate(float tick) override;

		Mat4 GetView();
		Mat4 GetProjection();

		virtual void SetOwner(std::shared_ptr<GameObject> owner) override;

		int GetUseCount();


	private:

		Mat4 projection;

	};

}

