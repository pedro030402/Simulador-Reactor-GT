#include "ActionInitialization.hh"       // Declaración de la clase ActionInitialization
#include "PrimaryGeneratorAction.hh"    // Generador primario personalizado
#include "RunAction.hh"                  // Acciones al inicio/fin de corrida
#include "EventAction.hh"                // Acciones al inicio/fin de evento
#include "SteppingAction.hh"             // Acción en cada paso de transporte

// Constructor: puede inicializar variables de configuración de acciones
ActionInitialization::ActionInitialization() {}

// Destructor: limpieza de recursos si fuera necesario
ActionInitialization::~ActionInitialization() {}

// Método Build: registra las acciones de usuario en el RunManager
void ActionInitialization::Build() const {
    // 1) Generación de partículas primarias
    SetUserAction(new PrimaryGeneratorAction());   // Configura fuente Cf-252

    // 2) Acciones de Run (BeginOfRun y EndOfRun)
    auto runAction = new RunAction();
    SetUserAction(runAction);

    // 3) Acciones de Evento (BeginOfEvent y EndOfEvent)
    auto eventAction = new EventAction();
    SetUserAction(eventAction);

    // 4) Acción de Stepping: usa eventAction para reportar o filtrar pasos
    SetUserAction(new SteppingAction(eventAction));
}
