#pragma once

#include <memory>

class Agent
{
public:
	Agent();

	bool Initialize(Behaviour::Tree::shared_ptr behaviour);
	
	void Start(float time);
	void Update(float time);

private:
	Behaviour::Tree::shared_ptr mBehaviour;
	std::unique_ptr<Bag> mBehaviourState;
};
