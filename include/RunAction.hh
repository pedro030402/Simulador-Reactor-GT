#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"  // Clase base para acciones al inicio y fin de cada corrida
#include "globals.hh"          // Tipos básicos de Geant4 (G4int, etc.)

class G4Run;  // Declaración adelantada de G4Run para evitar incluir su cabecera completa

// Clase RunAction: define comportamientos al inicio y fin de una corrida de simulación
class RunAction : public G4UserRunAction
{
public:
    // Constructor: inicializa contadores de neutrones
    RunAction();
    
    // Destructor virtual
    virtual ~RunAction();

    // Método llamado al comienzo de cada corrida (override de G4UserRunAction)
    virtual void BeginOfRunAction(const G4Run*) override;
    
    // Método llamado al final de cada corrida (override)
    virtual void EndOfRunAction(const G4Run*) override;

    // Métodos para agregar neutrones de fisión y contar absorbidos durante la corrida
    void AddFissionNeutrons(G4int n);
    void AddAbsorbedNeutrons(G4int n);

private:
    G4int totalFissionNeutrons;  // Contador acumulado de neutrones de fisión en la corrida
    G4int totalAbsorbedNeutrons; // Contador acumulado de neutrones absorbidos en la corrida
};

#endif // RUNACTION_HH