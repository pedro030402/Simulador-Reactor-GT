#include "EventAction.hh"
#include "G4Event.hh"

EventAction::EventAction() {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event*)
{
    // Sin conteos por evento
}

void EventAction::EndOfEventAction(const G4Event*)
{
    // No reportar conteos a RunAction
}

void EventAction::AddFissionNeutron()
{
    // Método vacío para mantener interfaz
}

int EventAction::GetFissionNeutrons() const
{
    return 0;
}

void EventAction::AddAbsorbedNeutron()
{
    // Método vacío para mantener interfaz
}

int EventAction::GetAbsorbedNeutrons() const
{
    return 0;
}
