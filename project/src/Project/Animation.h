#pragma once

#include "../Transform.h"
#include "../Utility/Exception.h"
#include <glm/glm.hpp>
#include <vector>

DEFINE_EXCEPTION(InvalidKeyFrameException);

class Animation
{
public:
	enum Axis
	{
		kAxis_Forward,
		kAxis_Backward,
		kAxis_Up,
		kAxis_Down,
		kAxis_Left,
		kAxis_Right
	};

	struct KeyFrame
	{
		float Translation;
		Axis TranslationAxis;
		float Rotation; // radians
		Axis RotationAxis;
		float Duration; // ms

		KeyFrame(float t, Axis tAxis, float r, Axis rAxis, float duration);
	};

private:
	Transform *m_Transform;
	bool m_Started;
	bool m_Animating;
	
	std::vector<KeyFrame> m_Frames;
	unsigned int m_CurrentFrame;
	float m_LastFrameTime; // ms
	float m_LastTime;

public:
	Animation(Transform *transform);

	Transform *GetTransform() const;
	bool IsStarted() const;
	bool IsAnimating() const;

	void AddKeyFrame(const KeyFrame &frame);
	void Reset(float time);

	void Update(float time);
};
