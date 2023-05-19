#include <syslog.h>
#include <string>

class LogManager {
public:
    LogManager(const std::string& ident) {
        // Abre una conexión con syslog
        openlog(ident.c_str(), LOG_PID | LOG_CONS, LOG_USER);
    }

    ~LogManager() {
        // Cierra la conexión con syslog al destruir el objeto
        closelog();
    }

    void logMessage(int priority, const std::string& message) {
        // Genera un mensaje en la bitácora utilizando syslog
        syslog(priority, "%s", message.c_str());
    }

    void logDebug(const std::string& message) {
        logMessage(LOG_DEBUG, message);
    }

    void logInfo(const std::string& message) {
        logMessage(LOG_INFO, message);
    }

    void logWarning(const std::string& message) {
        logMessage(LOG_WARNING, message);
    }

    void logError(const std::string& message) {
        logMessage(LOG_ERR, message);
    }
};
