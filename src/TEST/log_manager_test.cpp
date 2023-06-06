#include "LogManager.hpp"

int main() {
    LogManager logger("MyApp"); // Abre una conexión con syslog

    logger.logWarning("Atención: esto es una advertencia de prueba");
    logger.logError("Se ha producido un mensaje de error de prueba");

    return 0;
}
