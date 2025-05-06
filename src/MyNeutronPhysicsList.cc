// MyNeutronPhysicsList.cc
#include "MyNeutronPhysicsList.hh"

#include "G4DecayPhysics.hh"                 // Decaimientos básicos
#include "G4RadioactiveDecayPhysics.hh"      // Desintegración radiactiva (neutrones retardados)
#include "G4EmStandardPhysics.hh"            // Física electromagnética estándar
#include "G4HadronPhysicsQGSP_BERT_HP.hh"    // Física hadrónica de neutrones
#include "G4IonPhysics.hh"                   // Física de iones (fragmentos de fisión)

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

// Constructor: registra los módulos de física
MyNeutronPhysicsList::MyNeutronPhysicsList()
 : G4VModularPhysicsList()
{
    // 1) Física electromagnética
    RegisterPhysics(new G4EmStandardPhysics());

    // 2) Física de decaimientos de partículas inestables
    RegisterPhysics(new G4DecayPhysics());

    // 3) Física de desintegración radiactiva (neutrones retardados)
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    // 4) Física hadrónica de neutrones (QGSP_BERT_HP)
    RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP());

    // 5) Física de iones (fragmentos de fisión)
    RegisterPhysics(new G4IonPhysics());
}

// Destructor
MyNeutronPhysicsList::~MyNeutronPhysicsList() {}

// Construcción de partículas (incluye iones genéricos)
void MyNeutronPhysicsList::ConstructParticle()
{
    G4BosonConstructor     bosonCon;
    G4LeptonConstructor    leptonCon;
    G4MesonConstructor     mesonCon;
    G4BaryonConstructor    baryonCon;
    G4IonConstructor       ionCon;  // fragmentos e iones genéricos

    bosonCon.ConstructParticle();
    leptonCon.ConstructParticle();
    mesonCon.ConstructParticle();
    baryonCon.ConstructParticle();
    ionCon.ConstructParticle();
}

// Construcción de procesos (usa módulos registrados)
void MyNeutronPhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();
}
