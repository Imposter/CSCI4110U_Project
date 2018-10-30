#pragma once

#include <mutex>
#include <vector>
#include <functional>

struct EventArgs
{
	bool Cancel;

	EventArgs()
		: Cancel(false)
	{
	}
};

template <typename TArg = EventArgs>
class Event
{
	std::mutex m_Mutex;
	std::vector<std::function<void(TArg &)>> m_Callbacks;

public:
	Event() = default;
	
	Event(const Event &) = delete;
	Event &operator=(const Event &) = delete;

	Event(const Event &&) = delete;
	Event &operator=(const Event &&) = delete;

	template <typename TFunction>
	Event &Add(const TFunction &function)
	{
		m_Mutex.lock();
		m_Callbacks.push_back(function);
		m_Mutex.unlock();

		return *this;
	}

	template <typename TFunction>
	Event &Remove(const std::function<TFunction> &function)
	{
		m_Mutex.lock();
		for (auto it = m_Callbacks.begin(); it != m_Callbacks.end();)
		{
			if (it->template target<TFunction>() == function.template target<TFunction>())
			{
				m_Callbacks.erase(it);
				break;
			}
			++it;
		}
		m_Mutex.unlock();

		return *this;
	}

	void Clear()
	{
		m_Mutex.lock();
		m_Callbacks.clear();
		m_Mutex.unlock();
	}

	void Trigger(TArg arg)
	{
		std::vector<std::function<void(TArg &)>> callbacks(m_Callbacks.size());

		m_Mutex.lock();
		std::copy(m_Callbacks.begin(), m_Callbacks.end(), callbacks.begin());
		m_Mutex.unlock();

		for (auto iterator = callbacks.begin(); iterator != callbacks.end(); ++iterator)
			(*iterator)(arg);
	}

	void operator()(TArg arg)
	{
		Trigger(arg);
	}
};