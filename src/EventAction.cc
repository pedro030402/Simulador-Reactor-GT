#include "EventAction.hh"   // Declaración de la clase EventAction
#include "G4Event.hh"       // Definición de G4Event para información del evento

// Constructor: se puede inicializar contadores por evento (aquí no se usa)
EventAction::EventAction() {}

// Destructor: limpieza de recursos si se hubieran creado (vacío)
EventAction::~EventAction() {}

// Método llamado al inicio de cada evento de simulación
void EventAction::BeginOfEventAction(const G4Event*)
{
    // No se reinician contadores por evento en esta implementación
}

// Método llamado al final de cada evento de simulación
void EventAction::EndOfEventAction(const G4Event*)
{
    // No se reportan ni acumulan conteos en RunAction aquí
}

// Métodos para mantener la interfaz de conteo de neutrones de fisión
// Implementación vacía, conservan la firma para compatibilidad
void EventAction::AddFissionNeutron()
{
    // Sin efecto en esta versión
}

// Devuelve el número de neutrones de fisión contabilizados
// Siempre retorna 0 en esta implementación
int EventAction::GetFissionNeutrons() const
{
    return 0;
}

// Métodos para mantener la interfaz de conteo de neutrones absorbidos
void EventAction::AddAbsorbedNeutron()
{
    // Sin efecto en esta versión
}

// Devuelve el número de neutrones absorbidos contabilizados
// Siempre retorna 0 en esta implementación
int EventAction::GetAbsorbedNeutrons() const
{
    return 0;
}