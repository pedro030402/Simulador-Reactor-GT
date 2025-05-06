#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run*) override;

    void AddFissionNeutrons(G4int n);
    void AddAbsorbedNeutrons(G4int n);

private:
    G4int totalFissionNeutrons;
    G4int totalAbsorbedNeutrons;
};

#endif
