#ifndef PMPrimaryGenerator_h
#define PMPrimaryGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"

class PMPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
public:
    PMPrimaryGenerator();
    virtual ~PMPrimaryGenerator();

    virtual void GeneratePrimaries(G4Event*);

private:
    G4ParticleGun* fParticleGun;

    G4double WattDistribution(G4double a, G4double b);
};

#endif
