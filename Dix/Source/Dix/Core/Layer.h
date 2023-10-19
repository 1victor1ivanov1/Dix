#pragma once

#include "Dix/Core/Base.h"
#include "Dix/Core/Timestep.h"

#include "Dix/Events/Event.h"

namespace Dix
{
	class Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_Name; }

	protected:
		std::string m_Name;
	};
}