#include "RunAction.hh"   // Declaración de la clase RunAction
#include "G4Run.hh"       // Definición de G4Run para información de la corrida

// Constructor: puede inicializar contadores o estados antes de la corrida
RunAction::RunAction() {}

// Destructor: libera recursos si fuera necesario al finalizar la aplicación
RunAction::~RunAction() {}

// Método llamado al comienzo de cada corrida de simulación\
// Aquí se pueden reiniciar contadores o preparar distribuciones de datos
void RunAction::BeginOfRunAction(const G4Run*)
{
    // Nada que contar inicialmente: se deja espacio para prealocar estados
}

// Método llamado al final de cada corrida de simulación\
// Usualmente se reportan totales y estadísticas finales
void RunAction::EndOfRunAction(const G4Run*)
{
    // No hay cálculos o impresiones: ciclo finalizado sin salida adicional
}

// Función de interfaz para agregar neutrones de fisión
// Conservada para compatibilidad con otras clases, pero sin efecto aquí
void RunAction::AddFissionNeutrons(G4int)
{
    // Implementación vacía: mantiene la firma de la API
}

// Función de interfaz para agregar neutrones absorbidos
// Conservada para compatibilidad con otras clases, pero sin efecto aquí
void RunAction::AddAbsorbedNeutrons(G4int)
{
    // Implementación vacía: mantiene la firma de la API
}
