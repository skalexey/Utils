#include <algorithm>
#include <assert.h>
#include <utils/patterns/Observable.h>
#include <utils/patterns/Observer.h>
#include <utils/Log.h>
LOG_TITLE("Observable");
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
	void Observable::Unsubscribe(Observer* o)
	{
		LOCAL_VERBOSE(utils::format_str("	%p->Unsubscribe(%p)", this, o));
		auto sc = o->GetSubscriptions();
		assert(sc != nullptr);
		auto scIt = sc->find(this);
		assert(scIt != sc->end());
		sc->erase(scIt);
	}

	Observable::Observable()
	{
		LOCAL_VERBOSE(utils::format_str("Observable() %p", this));
	}

	bool Observable::Attach(Observer* o, const std::string& title)
	{
		LOCAL_VERBOSE(utils::format_str("%p->Attach(%p)", this, o));
		if (auto observers = GetObservers())
		{
			auto it = std::find(observers->begin(), observers->end(), o);
			if (it != observers->end())
			{
				LOCAL_VERBOSE("	Already attached");
				return false;
			}
		}
		GetObserversStorage().GetObservers()[this].push_back(o);
		auto& sc = Observer::GetAllSubscriptions()[o];
		assert(sc.find(this) == sc.end());
		sc[this] = {this, title};
		return true;
	}

	bool Observable::Detach(Observer* o)
	{
		LOCAL_VERBOSE(utils::format_str("%p->Detach(%p)", this, o));
		if (auto observers = GetObservers())
		{
			auto it = std::find(observers->begin(), observers->end(), o);
			if (it != observers->end())
			{
				observers->erase(it);
				Unsubscribe(o);
				return true;
			}
		}
		LOG_WARNING(utils::format_str("An Attempt to detach not registered observer %p from subject %p", o, this));
		return false;
	}

	Observable::~Observable()
	{
		LOCAL_VERBOSE(utils::format_str("~Observable() %p", this));
		if (auto observers = GetObservers())
		{
			LOCAL_VERBOSE(utils::format_str("	Found %d observers", observers->size()));
			for (auto o : *observers)
				Unsubscribe(o);
			GetObserversStorage().GetObservers().erase(this);
		}
	}

	void Observable::Notify(vl::VarPtr info)
	{
		auto& observers = GetObserversStorage().GetObservers();
		auto it = observers.find(this);
		if (it == observers.end())
			return;
		for (auto o : it->second)
			o->Update(this, info);
	}

	Observable::ObserversStorage& Observable::GetObserversStorage()
	{
		static ObserversStorage* mObserversStorage = new ObserversStorage;
		return *mObserversStorage;
	}

	const std::vector<Observer*>* Observable::GetObservers() const
	{
		auto& observers = GetObserversStorage().GetObservers();
		auto it = observers.find((Observable*)(this));
		if (it != observers.end())
			return &it->second;
		return nullptr;
	}

	std::vector<Observer*>* Observable::GetObservers()
	{
		return const_cast<std::vector<Observer*>*>(const_cast<const Observable*>(this)->GetObservers());
	}
}
