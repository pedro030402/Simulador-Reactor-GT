#include "RunAction.hh"
#include "G4Run.hh"

RunAction::RunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*)
{
    // Sin conteos: solo preallocación de estado inicial
}

void RunAction::EndOfRunAction(const G4Run*)
{
    // Sin cálculo de k; terminado sin impresión
}

void RunAction::AddFissionNeutrons(G4int)
{
    // Función mantenida pero sin efecto para preservar interfaz
}

void RunAction::AddAbsorbedNeutrons(G4int)
{
    // Función mantenida pero sin efecto para preservar interfaz
}
