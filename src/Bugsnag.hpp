#ifndef Bugsnag_hpp
#define Bugsnag_hpp

#include <iostream>
#include <map>

using namespace std;

namespace bugsnag {
enum BreadcrumbType {
  /**
   * A navigation action, such as pushing a view controller or dismissing an
   * alert
   */
  Navigation,
  /** A network request */
  Request,
  /** A background process, such performing a database query */
  Process,
  /** A log message */
  Log,
  /** A user event, such as authentication or control events */
  User,
  /** Change in application or view state */
  State,
  /**  A call to Bugsnag::notify() (internal use only) */
  Error,
  /**
   * Any breadcrumb sent via Bugsnag::leaveBreadcrumb() where type is not
   * otherwise specified
   */
  Manual
};

class Bugsnag {
public:
  /** Send an error to Bugsnag */
  static void notify(string name, string message);

  /** Send an error to Bugsnag */
  static void notify(exception &ex) { notify(typeid(ex).name(), ex.what()); }

  /**
   * Set information about the current user of the application which will be
   * present and searchable in subsequent error reports.
   * */
  static void setUser(string *id, string *email, string *name);

  /**
   * Starts tracking a new session.
   *
   * By default, sessions are automatically started when the application enters
   * the foreground. If you wish to manually call startSession at the
   * appropriate time in your application instead, the default behaviour can be
   * disabled via shouldAutoCaptureSessions.
   *
   * Any errors which occur in an active session count towards your
   * application's stability score. You can prevent errors from counting towards
   * your stability score by calling stopSession and resumeSession at the
   * appropriate time in your application.
   *
   * @see pauseSession
   * @see resumeSession
   */
  static void startSession();

  /**
   * Stops tracking a session.
   *
   * When a session is stopped, errors will not count towards your application's
   * stability score. This can be advantageous if you do not wish these
   * calculations to include a certain type of error, for example, a crash in a
   * background service. You should disable automatic session tracking via
   * shouldAutoCaptureSessions if you call this method.
   *
   * A stopped session can be resumed by calling resumeSession,
   * which will make any subsequent errors count towards your application's
   * stability score. Alternatively, an entirely new session can be created by
   * calling startSession.
   *
   * @see startSession
   * @see resumeSession
   */
  static void pauseSession();

  /**
   * Resumes a session which has previously been stopped, or starts a new
   * session if none exists.
   *
   * If a session has already been resumed or started and has not been stopped,
   * calling this method will have no effect. You should disable automatic
   * session tracking via shouldAutoCaptureSessions if you call this method.
   *
   * It's important to note that sessions are stored in memory for the lifetime
   * of the application process and are not persisted on disk. Therefore calling
   * this method on app startup would start a new session, rather than
   * continuing any previous session.
   *
   * You should call this at the appropriate time in your application when you
   * wish to resume a previously started session. Any subsequent errors which
   * occur in your application will be reported to Bugsnag and will count
   * towards your application's stability score.
   *
   * @see startSession
   * @see pauseSession
   */
  static void resumeSession();

  /**
   * Leave a "breadcrumb" log message with additional information about the
   * environment at the time the breadcrumb was captured.
   */
  static void leaveBreadcrumb(string &name, BreadcrumbType type,
                              map<string, string> metadata);

  /**
   * Add custom data to send to Bugsnag with every exception. If value is null,
   *  delete the current value for key
   */
  static void addMetadata(string section, string key, string *value);

  /** Remove custom data from Bugsnag reports. */
  static void clearMetadata(string section);

  /** A general summary of what was occuring in the application */
  static void setContext(string context);
};
} // namespace bugsnag
#endif // Bugsnag_hpp
