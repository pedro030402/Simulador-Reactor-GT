#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4Neutron.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
    : fEventAction(eventAction) {}

SteppingAction::~SteppingAction() {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto track = step->GetTrack();
    // Filtrar neutrones fuera de rango energético
    if (track->GetDefinition() == G4Neutron::Definition()) {
        G4double E = step->GetPreStepPoint()->GetKineticEnergy();
        // Solo permitimos [0.025 eV, 2 MeV]
        if (E < 0.025*eV || E > 2.0*MeV) {
            track->SetTrackStatus(fStopAndKill);
        }
    }
}
