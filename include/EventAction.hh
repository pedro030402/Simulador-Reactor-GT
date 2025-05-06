#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"  // Clase base para acciones en el inicio y fin de eventos

// Clase EventAction: define comportamientos al inicio y fin de cada evento de simulación
class EventAction : public G4UserEventAction
{
public:
    // Constructor: inicializa contadores de neutrones por evento
    EventAction();
    
    // Destructor virtual
    virtual ~EventAction();

    // Método llamado al comienzo de cada evento (override de G4UserEventAction)
    virtual void BeginOfEventAction(const G4Event*);
    
    // Método llamado al final de cada evento (override)
    virtual void EndOfEventAction(const G4Event*);

    // Incrementa el contador de neutrones de fisión (interfaz para otros módulos)
    void AddFissionNeutron();
    
    // Retorna el número de neutrones de fisión contados en el evento
    int GetFissionNeutrons() const;

    // Incrementa el contador de neutrones absorbidos (interfaz para otros módulos)
    void AddAbsorbedNeutron();
    
    // Retorna el número de neutrones absorbidos contados en el evento
    int GetAbsorbedNeutrons() const;

private:
    int fFissionNeutrons;    // Contador de neutrones producidos por fisión en el evento
    int fAbsorbedNeutrons;   // Contador de neutrones absorbidos en el evento
};

#endif // EVENTACTION_HH
