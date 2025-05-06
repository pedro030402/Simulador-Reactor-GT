// main.cc

#include <ctime>
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "MyNeutronPhysicsList.hh"   // Física personalizada
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4ScoringManager.hh"
#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "CLHEP/Random/Randomize.h"   // Para motor y semillas

int main(int argc, char** argv)
{
    // 1) Seleccionar y sembrar el motor de RNG
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
    long seed = static_cast<long>(std::time(nullptr));
    CLHEP::HepRandom::setTheSeed(seed);
    G4cout << "Random seed set to " << seed << G4endl;

    // 2) Crear el Run Manager
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);

    // 3) Inicializar detector, física y acciones
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new MyNeutronPhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    // 4) Scoring (opcional)
    G4ScoringManager::GetScoringManager();

    // 5) Interfaz gráfica (si no hay argumentos)
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // 6) Visualización
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // 7) Ejecutar macro de inicio
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/control/execute /home/pedro/ReactorSim/macros/init_vis.mac");

    // 8) Iniciar sesión UI (si aplica)
    if (ui) {
        ui->SessionStart();
        delete ui;
    }

    // 9) Limpiar
    delete visManager;
    delete runManager;

    return 0;
}
