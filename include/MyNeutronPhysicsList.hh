// MyNeutronPhysicsList.hh
#ifndef MYNEUTRONPHYSICSLIST_HH
#define MYNEUTRONPHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"

class MyNeutronPhysicsList : public G4VModularPhysicsList {
public:
    MyNeutronPhysicsList();
    virtual ~MyNeutronPhysicsList();

    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;
};

#endif  // MYNEUTRONPHYSICSLIST_HH
