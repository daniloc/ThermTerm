#ifndef SRC_UTILITY_OBSERVATION
#define SRC_UTILITY_OBSERVATION

#include <Arduino.h>

class Observer
{
public:
    virtual void objectDidChange() = 0; // pure virtual function
};

template <typename T, int MaxObservers>
class Subject
{
protected:
    T *observers[MaxObservers];
    int observerIndex = 0;

public:
    bool registerObserver(T *observer)
    {
        if (observerIndex < MaxObservers)
        {
            observers[observerIndex++] = observer;
            return true;
        }
        return false;
    }

    bool removeObserver(T *observer)
    {
        for (int i = 0; i < observerIndex; ++i)
        {
            if (observers[i] == observer)
            {
                // Move all elements after i one step to the left
                for (int j = i; j < observerIndex - 1; ++j)
                {
                    observers[j] = observers[j + 1];
                }
                // Decrease the count of observers
                observerIndex--;
                return true;
            }
        }
        return false; // observer was not found
    }

    void notifyObservers()
    {
        for (int i = 0; i < observerIndex; ++i)
        {
            observers[i]->objectDidChange();
        }
    }

    // implement other methods as needed
};

#endif // SRC_UTILITY_OBSERVATION
