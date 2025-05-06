#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"  // Clase base para definir generadores primarios en Geant4
#include "PMPrimaryGenerator.hh"             // Generador primario personalizado (Cf-252)
#include "globals.hh"                        // Tipos básicos de Geant4 (G4int, etc.)

// Clase PrimaryGeneratorAction: configura y ejecuta la generación de partículas primarias
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    // Constructor: inicializa el puntero al generador personalizado
    PrimaryGeneratorAction();
    
    // Destructor virtual: libera los recursos del generador
    virtual ~PrimaryGeneratorAction();

    // Método override que Geant4 invoca para generar las partículas de cada evento
    virtual void GeneratePrimaries(G4Event*);

private:
    PMPrimaryGenerator* fPMGenerator;  // Puntero al objeto que implementa la distribución de Cf-252
};

#endif  // PRIMARYGENERATORACTION_HH
