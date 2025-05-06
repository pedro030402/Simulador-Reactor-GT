#include "DetectorConstruction.hh"  // Declaración de la clase DetectorConstruction

#include "G4NistManager.hh"          // Gestor de materiales NIST
#include "G4Box.hh"                  // Sólido cúbico para el mundo
#include "G4Tubs.hh"                 // Sólido cilíndrico para el reactor
#include "G4LogicalVolume.hh"        // Volumen lógico (geometría + material)
#include "G4PVPlacement.hh"          // Colocación física de volúmenes
#include "G4SystemOfUnits.hh"        // Unidades (cm, m, g/cm3, etc.)
#include "G4Material.hh"             // Definición de materiales
#include "G4Element.hh"              // Definición de elementos e isótopos
#include "G4VisAttributes.hh"        // Atributos de visualización
#include "G4Colour.hh"               // Colores RGBA
#include "G4MultiFunctionalDetector.hh" // (No usado aquí; para detectores multiuso)
#include "G4SDManager.hh"            // (No usado; para gestionar detectores sensibles)
#include "G4PSPassageCellFlux.hh"    // (No usado; para tallies de flujo)

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // === 1) Material manager NIST ===
    G4NistManager* nist = G4NistManager::Instance();

    // === 2) Definición de elementos puros ===
    G4Element* elH  = nist->FindOrBuildElement("H");
    G4Element* elO  = nist->FindOrBuildElement("O");
    G4Element* elZr = nist->FindOrBuildElement("Zr");
    G4Element* elB  = nist->FindOrBuildElement("B");

    // === 3) Uranio enriquecido al 20% (isótopos U-235 y U-238) ===
    G4Isotope* U235 = new G4Isotope("U235", 92, 235, 235.0439242 * g/mole);
    G4Isotope* U238 = new G4Isotope("U238", 92, 238, 238.0507882 * g/mole);
    G4Element* elUEnriched = new G4Element("EnrichedUranium", "U", 2);
    elUEnriched->AddIsotope(U235, 20.0 * perCent);
    elUEnriched->AddIsotope(U238, 80.0 * perCent);

    // === 4) Materiales base ===
    G4Material* water = nist->FindOrBuildMaterial("G4_WATER");  // Agua pura

    // Ácido bórico H3BO3 (densidad 1.6 g/cm3)
    G4Material* boricAcid = new G4Material("BoricAcid", 1.6 * g/cm3, 3);
    boricAcid->AddElement(elH, 3);
    boricAcid->AddElement(elB, 1);
    boricAcid->AddElement(elO, 3);

    // Hidruro de zirconio con U enrich (densidad 6.0 g/cm3)
    G4Material* UZrH = new G4Material("UZrH", 6.0 * g/cm3, 2);
    UZrH->AddElement(elUEnriched, 1);
    UZrH->AddElement(elH, 1);

    // Zirconio metálico
    G4Material* zirconium = nist->FindOrBuildMaterial("G4_Zr");

    // === 5) Mezcla homogénea del núcleo de reactor ===
    G4Material* reactorMat = new G4Material("HomogeneousReactorMaterial", 2.7 * g/cm3, 4);
    reactorMat->AddMaterial(water,     0.48);   // 48% agua
    reactorMat->AddMaterial(boricAcid, 0.0014); // 0.14% ácido bórico
    reactorMat->AddMaterial(UZrH,      0.5021); // 50.21% UZrH
    reactorMat->AddMaterial(zirconium, 0.0165); // 1.65% Zr metálico

    // === 6) Volumen mundo (caja de 2 m de lado) ===
    G4double world_size = 2.0 * m;
    G4Material* air = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld = new G4Box("World", world_size/2, world_size/2, world_size/2);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, {}, logicWorld, "World", 0, false, 0);
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible()); // Mundo invisible

    // === 7) Volumen del reactor (cilindro) ===
    G4double radius = 15.61 * cm;
    G4double height = 38.08 * cm;

    G4Tubs* solidReactor = new G4Tubs("Reactor", 0, radius, height/2, 0., 360.*deg);
    G4LogicalVolume* logicReactor = new G4LogicalVolume(solidReactor, reactorMat, "Reactor");
    new G4PVPlacement(0, {}, logicReactor, "Reactor", logicWorld, false, 0);

    // Atributos de visualización: color cian semitransparente, sólido
    G4VisAttributes* visReactor = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.1));
    visReactor->SetForceSolid(true);
    logicReactor->SetVisAttributes(visReactor);

    return physWorld;  // Retorna el volumen físico del mundo
}
