#ifndef Bugsnag_hpp
#define Bugsnag_hpp

#include "Configuration.hpp"

#include <iostream>
#include <map>

using namespace std;

namespace bugsnag {
enum BreadcrumbType {
  navigation,
  request,
  process,
  log,
  user,
  state,
  error,
  manual
};

class Bugsnag {
public:
  static void init(string apiKey) { init(new Configuration(apiKey)); }

  static void init(Configuration *config);

  static void notify(string name, string message);

  static void notify(exception *ex) { notify(typeid(ex).name(), ex->what()); }

  static void setUser(string id, string email, string name);

  static void startSession();

  static void pauseSession();

  static void resumeSession();

  static void leaveBreadcrumb(string name, BreadcrumbType type,
                              map<string, string> metadata);

  static void addMetadata(string section, string key, void *value);

  static void clearMetadata(string section, string key);

  static void clearMetadata(string section);

  static void setContext(string context);
};
} // namespace bugsnag
#endif // Bugsnag_hpp
