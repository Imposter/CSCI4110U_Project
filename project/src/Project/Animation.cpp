#include "Animation.h"
#include "../Memory.h"

Animation::KeyFrame::KeyFrame(float t, Axis tAxis, float r, Axis rAxis, float duration)
	: Translation(t), TranslationAxis(tAxis), Rotation(r), RotationAxis(rAxis), Duration(duration)
{
}

Animation::Animation(Transform *transform)
	: m_Original(*transform), m_Transform(transform), m_Started(false), m_Animating(true), m_CurrentFrame(0), m_LastFrameTime(0.0f), m_LastTime(0.0f)
{
}

Transform *Animation::GetTransform() const
{
	return m_Transform;
}

Transform *Animation::GetOriginalTransform()
{
	return &m_Original;
}

void Animation::SetOriginalTransform(const Transform *t)
{
	m_Original = *t;
}

bool Animation::IsStarted() const
{
	return m_Started;
}

bool Animation::IsAnimating() const
{
	return m_Animating;
}

void Animation::AddKeyFrame(const KeyFrame &frame)
{
	m_Frames.push_back(frame);
}

void Animation::Reset(float time)
{
	// Restore properties of transform to before we started animating them
	m_Transform->SetScale(m_Original.GetScale());
	m_Transform->SetRotation(m_Original.GetRotation());
	m_Transform->SetPosition(m_Original.GetPosition());

	m_Started = false;
	m_Animating = true;
	m_CurrentFrame = 0;
	m_LastFrameTime = time;
	m_LastTime = time;
}

void Animation::Update(float time)
{
	if (!m_Animating || m_Frames.empty())
		return;

	if (!m_Started)
	{
		m_LastTime = 0;
		m_LastFrameTime = time;
		m_Started = true;
	}

	// Get current frame
	auto frame = m_Frames[m_CurrentFrame];

	const auto deltaTime = time - m_LastFrameTime;
	if (deltaTime > frame.Duration)
	{
		if (++m_CurrentFrame >= m_Frames.size())
		{
			m_Animating = false;
			return;
		}

		frame = m_Frames[m_CurrentFrame];
		m_LastFrameTime = time;
	}

	// Normalize time and find out how much percentage of this frame we have left
	const auto p = glm::min((deltaTime - m_LastTime) / frame.Duration, 1.0f);

	// Offset rotation
	glm::vec3 rotationAxis(0.0f);
	switch (frame.RotationAxis)
	{
	case kAxis_Forward:
		rotationAxis = m_Transform->Forward();
		break;
	case kAxis_Backward:
		rotationAxis = -m_Transform->Forward();
		break;
	case kAxis_Up:
		rotationAxis = m_Transform->Up();
		break;
	case kAxis_Down:
		rotationAxis = -m_Transform->Up();
		break;
	case kAxis_Left:
		rotationAxis = -m_Transform->Right();
		break;
	case kAxis_Right:
		rotationAxis = m_Transform->Right();
		break;
	default:
		THROW_EXCEPTION(InvalidKeyFrameException, "Invalid rotation axis specified");
	}

	m_Transform->OffsetRotation(rotationAxis, frame.Rotation * p);

	// Offset position
	glm::vec3 translationAxis(0.0f);
	switch (frame.TranslationAxis)
	{
	case kAxis_Forward:
		translationAxis = m_Transform->Forward();
		break;
	case kAxis_Backward:
		translationAxis = -m_Transform->Forward();
		break;
	case kAxis_Up:
		translationAxis = m_Transform->Up();
		break;
	case kAxis_Down:
		translationAxis = -m_Transform->Up();
		break;
	case kAxis_Left:
		translationAxis = -m_Transform->Right();
		break;
	case kAxis_Right:
		translationAxis = m_Transform->Right();
		break;
	default:
		THROW_EXCEPTION(InvalidKeyFrameException, "Invalid translation axis specified");
	}

	m_Transform->OffsetPosition(glm::normalize(translationAxis) * frame.Translation * p);

	m_LastTime = deltaTime;
}

AnimationContainer::AnimationContainer(bool simultaneous)
	: m_Simultaneous(simultaneous), m_Index(0), m_Animating(true)
{
}

AnimationContainer::~AnimationContainer()
{
	for (auto anim : m_Animations)
		Delete(anim);
	m_Animations.clear();
}

bool AnimationContainer::IsSimultaneous() const
{
	return m_Simultaneous;
}

void AnimationContainer::SetSimultaneous(bool simultaneous)
{
	m_Simultaneous = simultaneous;
}

bool AnimationContainer::IsAnimating() const
{
	return m_Animating;
}

void AnimationContainer::AddAnimation(Animation *animation)
{
	m_Animations.push_back(animation);
}

void AnimationContainer::Reset(float time)
{
	// Reset animations in reverse order
	for (auto it = m_Animations.rbegin(); it != m_Animations.rend(); ++it)
	{
		const auto anim = *it;
		anim->Reset(time);
	}

	m_Index = 0;
	m_Animating = true;
}

void AnimationContainer::Update(float time)
{
	// Check if animating
	if (!m_Animating) return;

	// Update animations
	auto startIndex = m_Index;
	auto endIndex = m_Index;
	if (m_Simultaneous)
	{
		startIndex = 0;
		endIndex = m_Animations.size() - 1;
	}

	for (auto i = startIndex; i <= endIndex; i++)
	{
		const auto anim = m_Animations[i];
		anim->Update(time);
	}

	if (!m_Simultaneous)
	{
		auto anim = m_Animations[m_Index];
		if (!anim->IsAnimating()) ++m_Index;
		if (m_Index > m_Animations.size() - 1)
		{
			m_Animating = false;
			return;
		}

		const auto nextAnim = m_Animations[m_Index];
		nextAnim->SetOriginalTransform(anim->GetOriginalTransform());
	}
}

AnimationPlayer::AnimationPlayer()
	: m_Index(0), m_Animating(true)
{
}

AnimationPlayer::~AnimationPlayer()
{
	for (auto container : m_Containers)
		Delete(container);
	m_Containers.clear();
}

bool AnimationPlayer::IsAnimating() const
{
	return m_Animating;
}

void AnimationPlayer::AddContainer(AnimationContainer *container)
{
	m_Containers.push_back(container);
}

void AnimationPlayer::Update(float time)
{
	if (!m_Animating) return;

	const auto container = m_Containers[m_Index];
	container->Update(time);

	if (!container->IsAnimating())
	{
		if (++m_Index > m_Containers.size() - 1)
			m_Animating = false;
	}
}

void AnimationPlayer::Reset(float time)
{
	// Reset all containers in reverse order
	for (auto it = m_Containers.rbegin(); it != m_Containers.rend(); ++it)
	{
		const auto container = *it;
		container->Reset(time);
	}

	m_Index = 0;
	m_Animating = true;
}
