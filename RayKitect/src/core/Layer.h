#pragma once
#include <string>
#include "events/Event.h"

class Layer {
public:
	Layer(std::string name);
	virtual ~Layer();

	virtual void OnAttach() {};
	virtual void OnDetach() {};
	virtual void OnUpdate(float dt) {};
	virtual void OnImGuiUpdate() {};
	virtual void OnEvent(Event& e) {};

	const std::string& GetName() const { return m_Name; }
private:
	std::string m_Name;
};
