#ifndef SRC_SETTINGS_CLI
#define SRC_SETTINGS_CLI

class Settings
{
public:
    void heartbeat();
    bool needsConfiguration();
    void printWelcomePrompt();
};

#endif // SRC_SETTINGS
