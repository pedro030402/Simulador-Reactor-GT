// main.cc

#include <ctime>                             // Para obtener la hora actual como semilla
#include "G4RunManagerFactory.hh"            // Fábrica de RunManager para gestionar la simulación
#include "G4UImanager.hh"                    // Interfaz de usuario (comandos)
#include "MyNeutronPhysicsList.hh"          // Lista de física personalizada para neutrones
#include "G4VisExecutive.hh"                 // Gestión de visualización
#include "G4UIExecutive.hh"                  // Interfaz gráfica interactiva
#include "G4ScoringManager.hh"               // Administrador de scoring para análisis de resultados
#include "DetectorConstruction.hh"           // Definición de geometría y materiales del detector
#include "ActionInitialization.hh"           // Inicializa generador, stepping y run actions

#include "CLHEP/Random/Randomize.h"          // Motor de números aleatorios y configuración de semillas

int main(int argc, char** argv)
{
    // 1) Configurar el generador de números aleatorios
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine()); // Usa RanecuEngine como motor RNG
    long seed = static_cast<long>(std::time(nullptr));        // Semilla basada en el tiempo actual
    CLHEP::HepRandom::setTheSeed(seed);
    G4cout << "Random seed set to " << seed << G4endl;

    // 2) Crear el RunManager (modo serial)
    auto* runManager = G4RunManagerFactory::CreateRunManager(
        G4RunManagerType::Serial);

    // 3) Inicializar subsistemas: detector, física y acciones de usuario
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new MyNeutronPhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    // 4) Inicializar el ScoringManager (opcional para tallies y análisis)
    G4ScoringManager::GetScoringManager();

    // 5) Preparar interfaz de usuario interactiva si no se pasan argumentos
    G4UIExecutive* ui = nullptr;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    // 6) Configurar y arrancar el gestor de visualización
    G4VisManager* visManager = new G4VisExecutive();
    visManager->Initialize();

    // 7) Ejecutar macro de inicialización de vis y UI commands
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand(
        "/control/execute /home/pedro/ReactorSim/macros/init_vis.mac");

    // 8) Iniciar la sesión de UI (ventana interactiva)
    if (ui) {
        ui->SessionStart();
        delete ui;
    }

    // 9) Liberar memoria y cerrar RunManager y VisManager
    delete visManager;
    delete runManager;

    return 0; // Termina la aplicación
}
