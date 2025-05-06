#include "PMPrimaryGenerator.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include <cmath>

PMPrimaryGenerator::PMPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1); // Un neutrón por evento

    // Tipo de partícula: neutrón
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("neutron");
    fParticleGun->SetParticleDefinition(particle);
}

PMPrimaryGenerator::~PMPrimaryGenerator()
{
    delete fParticleGun;
}

void PMPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    // Dirección aleatoria
    G4ThreeVector direction = G4RandomDirection();
    fParticleGun->SetParticleMomentumDirection(direction);

    // ----- Distribución de Watt para Cf-252 -----
    G4double a = 1.025 * MeV;        // Escala Cf-252
    G4double b = 2.926 / MeV;
    G4double energy = WattDistribution(a, b);
    fParticleGun->SetParticleEnergy(energy);

    // 🔸 Posición aleatoria dentro de un volumen cilíndrico (reactor)
    G4double radius = 15.61 * cm;
    G4double height = 38.08 * cm;

    G4double r = radius * std::sqrt(G4UniformRand());
    G4double theta = 2.0 * CLHEP::pi * G4UniformRand();
    G4double z = (G4UniformRand() - 0.5) * height;

    G4double x = r * std::cos(theta);
    G4double y = r * std::sin(theta);

    G4ThreeVector randomPos(x, y, z);
    fParticleGun->SetParticlePosition(randomPos);

    // Generar el evento primario
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

// ======= Función de distribución de Watt (rejection sampling) =======
G4double PMPrimaryGenerator::WattDistribution(G4double a, G4double b)
{
    while (true)
    {
        G4double xi1 = G4UniformRand();
        G4double xi2 = G4UniformRand();

        G4double E = -a * std::log(xi1); // Distribución exponencial
        G4double f = std::sinh(std::sqrt(b * E)); // Rechazo por sinh(√(bE))

        if (xi2 < f)
        {
            return E;
        }
    }
}
