#ifndef SRC_UTILITY_SINGLETONTEMPLATE
#define SRC_UTILITY_SINGLETONTEMPLATE

template <typename T>
class Singleton
{
public:
    static T &shared()
    {
        static T instance;
        return instance;
    }

    Singleton(const Singleton &) = delete;
    void operator=(const Singleton &) = delete;

protected:
    Singleton() {}
};

#endif // SRC_UTILITY_SINGLETONTEMPLATE
