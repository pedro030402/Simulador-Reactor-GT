#include "SteppingAction.hh"            // Declaración de la clase SteppingAction
#include "G4Step.hh"                     // Definición de G4Step para acceder a cada paso de transporte
#include "G4Track.hh"                    // Definición de G4Track para obtener información de la partícula
#include "G4StepPoint.hh"                // Puntos antes y después del paso (Pre y Post)
#include "G4VProcess.hh"                 // Procesos físicos involucrados en un paso
#include "G4SystemOfUnits.hh"            // Unidades definidas en Geant4 (eV, MeV, etc.)
#include "G4Neutron.hh"                  // Definición de la partícula neutrón

// Constructor: recibe puntero a EventAction para reportar información de cada evento
SteppingAction::SteppingAction(EventAction* eventAction)
    : fEventAction(eventAction) {}

// Destructor: vacío, sin liberaciones específicas necesarias
SteppingAction::~SteppingAction() {}

// Método invocado en cada paso de transporte de una partícula
void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto track = step->GetTrack();  // Obtiene el rastro actual de la partícula

    // Filtrar solo eventos donde la partícula es un neutrón
    if (track->GetDefinition() == G4Neutron::Definition()) {
        // Energía cinética antes del paso
        G4double E = step->GetPreStepPoint()->GetKineticEnergy();
        
        // Rango energético permitido: 0.025 eV a 2 MeV
        if (E < 0.025*eV || E > 2.0*MeV) {
            // Si está fuera de rango, detener y eliminar el rastro
            track->SetTrackStatus(fStopAndKill);
        }
    }
}
