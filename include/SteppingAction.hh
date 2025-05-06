#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"  // Clase base para acciones en cada paso de transporte

// Declaración adelantada de EventAction para evitar dependencias circulares
class EventAction;

// Clase SteppingAction: hereda de G4UserSteppingAction para definir acciones en cada paso
class SteppingAction : public G4UserSteppingAction
{
public:
    // Constructor: recibe puntero a EventAction para informar sobre filtrado o conteo
    SteppingAction(EventAction* eventAction);
    
    // Destructor virtual
    virtual ~SteppingAction();

    // Método principal: se llama en cada paso de transporte de Geant4
    virtual void UserSteppingAction(const G4Step* step);

private:
    EventAction* fEventAction;  // Puntero a la clase EventAction asociada
};

#endif  // STEPPINGACTION_HH
