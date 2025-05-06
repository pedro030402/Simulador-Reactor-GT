#include "PrimaryGeneratorAction.hh"
#include "PMPrimaryGenerator.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    fPMGenerator = new PMPrimaryGenerator();
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fPMGenerator;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fPMGenerator->GeneratePrimaries(anEvent);
}
