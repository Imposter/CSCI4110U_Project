#include "Animation.h"

Animation::KeyFrame::KeyFrame(float t, Axis tAxis, float r, Axis rAxis, float duration)
	: Translation(t), TranslationAxis(tAxis), Rotation(r), RotationAxis(rAxis), Duration(duration)
{
}

Animation::Animation(Transform *transform)
	: m_Transform(transform), m_Started(false), m_Animating(true), m_CurrentFrame(0), m_LastFrameTime(0.0f), m_LastTime(0.0f)
{
}

Transform *Animation::GetTransform() const
{
	return m_Transform;
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
