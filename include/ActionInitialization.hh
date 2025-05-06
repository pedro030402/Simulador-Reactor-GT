#ifndef ACTION_INITIALIZATION_HH
#define ACTION_INITIALIZATION_HH

#include "G4VUserActionInitialization.hh"  // Clase base para inicialización de acciones de usuario

// ActionInitialization: registra las acciones que se usarán durante la simulación
class ActionInitialization : public G4VUserActionInitialization {
public:
    // Constructor: aquí se pueden pasar parámetros de configuración si fueran necesarios
    ActionInitialization();
    
    // Destructor virtual: limpia recursos si se hubieran asignado
    virtual ~ActionInitialization();

    // Build: método override que Geant4 invoca para registrar las acciones de generación,
    // evento, stepping y corrida de usuario
    virtual void Build() const override;
};

#endif  // ACTION_INITIALIZATI