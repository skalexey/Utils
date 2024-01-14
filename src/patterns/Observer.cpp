#include <assert.h>
#include <utils/patterns/Observer.h>
#include <utils/patterns/Observable.h>
#include <utils/Log.h>
LOG_TITLE("Observer");
#ifdef OBSERVER_LOG_VERBOSE
	SET_LOCAL_LOG_VERBOSE(true);
#else
	SET_LOCAL_LOG_VERBOSE(false);
#endif
#ifdef LOG_ON
	#include <utils/string_utils.h>
#endif

namespace vl
{
	Observer::~Observer()
	{
		LOCAL_VERBOSE(utils::format_str("~Observer() %p", this));

		auto it = GetAllSubscriptions().find(this);
		if (it != GetAllSubscriptions().end())
		{
			LOCAL_VERBOSE(utils::format_str("	Found %d subscriptions", GetAllSubscriptions().size()));

			auto& sc = it->second;
			for (auto& [observable, info] : sc)
			{
				LOCAL_VERBOSE(utils::format_str("	Unsubscribe from %p", observable));
				auto observers = observable->GetObservers();
				assert(observers != nullptr);
				auto it = std::find(observers->begin(), observers->end(), this);
				assert(it != observers->end());
				observers->erase(it);
			}
			GetAllSubscriptions().erase(it);
		}
	}

	std::unordered_map<Observer*, std::unordered_map<Observable*, SubscriptionInfo>>& Observer::GetAllSubscriptions()
	{
		using T = std::unordered_map<Observer*, std::unordered_map<Observable*, SubscriptionInfo>>;
		static T* mSubscriptions = new T;
		return *mSubscriptions;
	}

	const std::unordered_map<Observable*, SubscriptionInfo>* Observer::GetSubscriptions() const
	{
		auto it = GetAllSubscriptions().find((Observer*)this);
		if (it != GetAllSubscriptions().end())
			return &it->second;
		return nullptr;
	}

	std::unordered_map<Observable*, SubscriptionInfo>* Observer::GetSubscriptions()
	{
		return const_cast<std::unordered_map<Observable*, SubscriptionInfo>*>
			(const_cast<const Observer*>(this)->GetSubscriptions());
	}

	const SubscriptionInfo* Observer::GetSubscriptionInfo(const Observable* o) const
	{
		if (auto subs = GetSubscriptions())
		{
			auto it = subs->find((Observable*)o);
			if (it != subs->end())
				return &it->second;
		}
		return nullptr;
	}

	SubscriptionInfo* Observer::GetSubscriptionInfo(Observable* o)
	{
		return const_cast<SubscriptionInfo*>
			(const_cast<const Observer*>(this)->GetSubscriptionInfo(o));
	}
}