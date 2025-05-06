#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void AddFissionNeutron();
    int GetFissionNeutrons() const;

    void AddAbsorbedNeutron();
    int GetAbsorbedNeutrons() const;

private:
    int fFissionNeutrons;
    int fAbsorbedNeutrons;
};

#endif
