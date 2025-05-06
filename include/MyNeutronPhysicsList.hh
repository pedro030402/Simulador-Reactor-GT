#ifndef MYNEUTRONPHYSICSLIST_HH
#define MYNEUTRONPHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"  // Clase base para listas modulares de física en Geant4

// MyNeutronPhysicsList: define y registra módulos de física específicos para simulaciones de neutrones
class MyNeutronPhysicsList : public G4VModularPhysicsList {
public:
    // Constructor: registra los procesos físicos deseados (EM, hadrónico, decaimiento, etc.)
    MyNeutronPhysicsList();
    
    // Destructor virtual: limpia los módulos registrados automáticamente
    virtual ~MyNeutronPhysicsList();

    // ConstructParticle: invoca constructores de familia para registrar tipos de partículas
    virtual void ConstructParticle() override;
    
    // ConstructProcess: llama al método base para enlazar procesos físicos a las partículas registradas
    virtual void ConstructProcess() override;
};

#endif  // MYNEUTRONPHYSICSLIST_HH
