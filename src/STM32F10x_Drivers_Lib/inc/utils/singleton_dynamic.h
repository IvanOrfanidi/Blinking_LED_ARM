/**
 * @brief   This is file realize Singleton pattern.
 *
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SINGLETON_DYNAMIC_H
#define __SINGLETON_DYNAMIC_H

/* Includes ------------------------------------------------------------------*/
/* Driver Interrupt */
#include "interrupt.h"

/* Utils */
#include "non_copyable.h"
#include "non_movable.h"

#ifdef __cplusplus

template<typename T>
class SingletonDynamic
  : private NonCopyable<T>
  , private NonMovable<T>
{
  public:
	/// Get instance class
	static T* getInstance()
	{
		// Check interrupt status enable
		LockInterrupt lockInterrupt;
		// Create class instance
		if(nullptr == _instance) {
			_instance = new T();
		}

		return _instance;
	}

	static void destroyInstance()
	{
		delete _instance;
		_instance = nullptr;
	}

  protected:
	SingletonDynamic() = default;
	~SingletonDynamic()
	{
		destroyInstance();
	}

  private:
	/// Class lock interrupt
	class LockInterrupt
	  : private NonCopyable<LockInterrupt>
	  , private NonMovable<LockInterrupt>
	{
	  public:
		LockInterrupt()
		{
			_isEnabled = Interrupt::isEnabledGlobally();
			Interrupt::disableGlobally();
		}

		~LockInterrupt()
		{
			// Enable interrupts back
			if(_isEnabled) {
				Interrupt::enableGlobally();
			}
		}

	  private:
		bool _isEnabled;
	};

	/// Instance
	static T* _instance;
};

template<typename T> T* SingletonDynamic<T>::_instance = nullptr;

extern "C" {
}

#endif //__cplusplus

#endif // __SINGLETON_DYNAMIC_H
