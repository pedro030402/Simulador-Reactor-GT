#include "PrimaryGeneratorAction.hh"  // Declaración de la clase PrimaryGeneratorAction
#include "PMPrimaryGenerator.hh"       // Declaración del generador primario personalizado PMPrimaryGenerator

// Constructor: crea la instancia del generador primario
PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    // Asigna un nuevo generador personalizado para producir partículas
    fPMGenerator = new PMPrimaryGenerator();
}

// Destructor: libera la memoria ocupada por el generador primario
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fPMGenerator;
}

// Método invocado por Geant4 para generar las partículas iniciales de cada evento
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // Delegamos la generación al objeto PMPrimaryGenerator
    fPMGenerator->GeneratePrimaries(anEvent);
}
