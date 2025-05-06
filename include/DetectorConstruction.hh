#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H 1

#include "G4VUserDetectorConstruction.hh"  // Clase base para la construcción de detectores en Geant4
#include "globals.hh"                       // Tipos básicos de Geant4 (G4double, etc.)

// DetectorConstruction: define la geometría, materiales y volúmenes del detector y el mundo
class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    // Constructor: puede inicializar parámetros geométricos o materiales predeterminados
    DetectorConstruction();
    
    // Destructor virtual: libera recursos si se hubieran asignado dinámicamente
    virtual ~DetectorConstruction();

    // Método override que construye y retorna el volumen físico del mundo
    virtual G4VPhysicalVolume* Construct();
};

#endif // DETECTORCONSTRUCTION_H
