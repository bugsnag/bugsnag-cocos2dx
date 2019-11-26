#ifndef BugsnagConfig_hpp
#define BugsnagConfig_hpp

#include <iostream>
#include <vector>

using namespace std;

namespace bugsnag {
class Configuration {
public:
    Configuration(string apiKey);

    ~Configuration();

    void setEndpoints(string notify, string sessions) {
      notifyEndpoint = notify;
      sessionsEndpoint = sessions;
    }

    string apiKey;

    unsigned int maxBreadcrumbs;

    string appVersion;

    string releaseStage;

    vector<string> enabledReleaseStages;

    unsigned long versionCode;

    bool autoTrackSessions;

    bool autoDetectErrors;

    bool autoDetectANRs;

    bool autoDetectOOMs;

    bool autoDetectNDKCrashes;

private:
    string notifyEndpoint;

    string sessionsEndpoint;
};
}

#endif
