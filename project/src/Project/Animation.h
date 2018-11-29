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
	Transform m_Original;
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
	
	Transform *GetOriginalTransform();
	void SetOriginalTransform(const Transform *t);

	bool IsStarted() const;
	bool IsAnimating() const;

	void AddKeyFrame(const KeyFrame &frame);
	void Reset(float time);

	void Update(float time);
};

class AnimationContainer
{
	bool m_Simultaneous;
	std::vector<Animation *> m_Animations;
	int m_Index;
	bool m_Animating;

public:
	AnimationContainer(bool simultaneous = false);
	~AnimationContainer();

	// No copying/moving
	AnimationContainer(const AnimationContainer &) = delete;
	AnimationContainer &operator=(const AnimationContainer &) = delete;

	AnimationContainer(const AnimationContainer &&) = delete;
	AnimationContainer &operator=(const AnimationContainer &&) = delete;

	bool IsSimultaneous() const;
	void SetSimultaneous(bool simultaneous);

	bool IsAnimating() const;

	void AddAnimation(Animation *animation);
	void Reset(float time);

	void Update(float time);
};

class AnimationPlayer
{
	std::vector<AnimationContainer *> m_Containers;
	int m_Index;
	bool m_Animating;

public:
	AnimationPlayer();
	~AnimationPlayer();

	// No copying/moving
	AnimationPlayer(const AnimationPlayer &) = delete;
	AnimationPlayer &operator=(const AnimationPlayer &) = delete;

	AnimationPlayer(const AnimationPlayer &&) = delete;
	AnimationPlayer &operator=(const AnimationPlayer &&) = delete;

	bool IsAnimating() const;

	void AddContainer(AnimationContainer *container);
	void Update(float time);
	void Reset(float time);
};