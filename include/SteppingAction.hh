#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class G4Step;
class EventAction;  // Declaración adelantada

class SteppingAction : public G4UserSteppingAction
{
public:
    // Constructor con puntero a EventAction
    SteppingAction(EventAction* eventAction);
    virtual ~SteppingAction();

    virtual void UserSteppingAction(const G4Step* step);

private:
    EventAction* fEventAction;  // Puntero a EventAction
};

#endif
