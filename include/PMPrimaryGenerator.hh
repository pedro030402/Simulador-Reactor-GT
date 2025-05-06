#ifndef PMPRIMARYGENERATOR_H
#define PMPRIMARYGENERATOR_H 1

#include "G4VUserPrimaryGeneratorAction.hh"  // Clase base para generadores primarios en Geant4
#include "G4ParticleGun.hh"                 // Herramienta para disparar partículas simples

// Clase PMPrimaryGenerator: implementa una fuente de neutrones Cf-252 con distribución de Watt
class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    // Constructor: configura el ParticleGun para un neutrón por evento
    PMPrimaryGenerator();
    
    // Destructor virtual: libera recursos asociados al ParticleGun
    virtual ~PMPrimaryGenerator();

    // Método override llamado por Geant4 para generar las partículas en cada evento
    virtual void GeneratePrimaries(G4Event*);

private:
    G4ParticleGun* fParticleGun;  // Objeto que dispara las partículas con dirección, energía y posición

    // Función auxiliar: muestrea la energía según la distribución de Watt (parametrizada por a y b)
    G4double WattDistribution(G4double a, G4double b);
};

#endif // PMPRIMARYGENERATOR_H