// MyNeutronPhysicsList.cc
#include "MyNeutronPhysicsList.hh"           // Declaración de la clase de lista de física modular

// Módulos de física básicos de Geant4
#include "G4DecayPhysics.hh"                 // Física de decaimientos de partículas inestables
#include "G4RadioactiveDecayPhysics.hh"      // Desintegración radiactiva (neutrones retardados)
#include "G4EmStandardPhysics.hh"            // Física electromagnética estándar
#include "G4HadronPhysicsQGSP_BERT_HP.hh"    // Física hadrónica con modelo QGSP_BERT y alta precisión
#include "G4IonPhysics.hh"                   // Física de iones (fragmentos de fisión)

// Constructores de familias de partículas
#include "G4BosonConstructor.hh"             // Construye partículas bosónicas (e.g., fotones)
#include "G4LeptonConstructor.hh"            // Construye leptones (e.g., electrones, neutrinos)
#include "G4MesonConstructor.hh"             // Construye mesones (e.g., piones, kaones)
#include "G4BaryonConstructor.hh"            // Construye bariones (e.g., protones, neutrones)
#include "G4IonConstructor.hh"               // Construye iones genéricos y fragmentos de fisión

// Constructor: registra los modelos físicos que se usarán
MyNeutronPhysicsList::MyNeutronPhysicsList()
 : G4VModularPhysicsList()
{
    // 1) Física electromagnética estándar para electrones, fotones, etc.
    RegisterPhysics(new G4EmStandardPhysics());

    // 2) Física de decaimientos de partículas inestables
    RegisterPhysics(new G4DecayPhysics());

    // 3) Física de desintegración radiactiva, incluyendo neutrones retardados
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    // 4) Física hadrónica de neutrones y otras partículas con modelo QGSP_BERT_HP
    RegisterPhysics(new G4HadronPhysicsQGSP_BERT_HP());

    // 5) Física de iones y fragmentos de fisión
    RegisterPhysics(new G4IonPhysics());
}

// Destructor: libera los módulos registrados automáticamente
MyNeutronPhysicsList::~MyNeutronPhysicsList() {}

// Construye todas las partículas definidas en los módulos anteriores
void MyNeutronPhysicsList::ConstructParticle()
{
    // Cada constructor registra sus partículas en el ParticleTable
    G4BosonConstructor     bosonCon;
    G4LeptonConstructor    leptonCon;
    G4MesonConstructor     mesonCon;
    G4BaryonConstructor    baryonCon;
    G4IonConstructor       ionCon;   

    bosonCon.ConstructParticle();
    leptonCon.ConstructParticle();
    mesonCon.ConstructParticle();
    baryonCon.ConstructParticle();
    ionCon.ConstructParticle();  // Iones y fragmentos de fisión
}

// Construye los procesos físicos usando los módulos registrados
void MyNeutronPhysicsList::ConstructProcess()
{
    G4VModularPhysicsList::ConstructProcess();  // Llama al método base para enlazar procesos
}
