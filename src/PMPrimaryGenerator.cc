#include "PMPrimaryGenerator.hh"          // Declaración de la clase PMPrimaryGenerator

#include "G4ParticleGun.hh"               // Generador de partículas de Geant4
#include "G4ParticleTable.hh"             // Tabla de partículas disponibles
#include "G4ParticleDefinition.hh"        // Definición de tipos de partículas
#include "G4Event.hh"                     // Representa un evento de simulación
#include "G4ThreeVector.hh"               // Vectores en 3D para posición/momentum
#include "G4SystemOfUnits.hh"             // Unidades físicas definidas (eV, MeV, cm, etc.)
#include "G4RandomDirection.hh"           // Generación de direcciones aleatorias
#include "Randomize.hh"                   // Funciones de random de Geant4
#include <cmath>                           // Funciones matemáticas estándar

// Constructor: inicializa el ParticleGun con 1 partícula por evento
PMPrimaryGenerator::PMPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1); // Un neutrón por evento

    // Selección de la partícula: neutrón
    G4ParticleDefinition* particle =
        G4ParticleTable::GetParticleTable()->FindParticle("neutron");
    fParticleGun->SetParticleDefinition(particle);
}

// Destructor: libera la memoria del ParticleGun
PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

// Método principal para generar la partícula primaria en cada evento
void PMPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    // 1. Dirección de emisión aleatoria en 4π
    G4ThreeVector direction = G4RandomDirection();
    fParticleGun->SetParticleMomentumDirection(direction);

    // 2. Energía según distribución de Watt para Cf-252
    G4double a = 1.025 * MeV;     // Parámetro a de Watt (escala)
    G4double b = 2.926 / MeV;     // Parámetro b de Watt (forma)
    G4double energy = WattDistribution(a, b);
    fParticleGun->SetParticleEnergy(energy);

    // 3. Posición de emisión aleatoria dentro de un cilindro (reactor)
    G4double radius = 15.61 * cm;  // Radio del cilindro
    G4double height = 38.08 * cm;  // Altura del cilindro

    // Generación en coordenadas cilíndricas
    G4double r = radius * std::sqrt(G4UniformRand());          // Distribución radial uniforme en área
    G4double theta = 2.0 * CLHEP::pi * G4UniformRand();        // Ángulo azimutal aleatorio
    G4double z = (G4UniformRand() - 0.5) * height;             // Coordenada z uniforme en altura

    // Transformación a coordenadas cartesianas
    G4double x = r * std::cos(theta);
    G4double y = r * std::sin(theta);

    G4ThreeVector randomPos(x, y, z);
    fParticleGun->SetParticlePosition(randomPos);

    // 4. Generación final del vértice primario en el evento
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

// ===== Función de distribución de Watt (muestreo por rechazo) =====
G4double PMPrimaryGenerator::WattDistribution(G4double a, G4double b)
{
    while (true)
    {
        // xi1 y xi2: números aleatorios uniformes en [0,1)
        G4double xi1 = G4UniformRand();
        G4double xi2 = G4UniformRand();

        // Propuesta de energía E con distribución exponencial
        G4double E = -a * std::log(xi1);
        // Función de rechazo proporcional a sinh(sqrt(bE))
        G4double f = std::sinh(std::sqrt(b * E));

        // Aceptar E si xi2 < f
        if (xi2 < f)
        {
            return E;  // Energía muestreada según Watt
        }
    }
}
