#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4PSPassageCellFlux.hh"

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // === Material manager ===
    G4NistManager* nist = G4NistManager::Instance();

    // === Define elements ===
    G4Element* elH  = nist->FindOrBuildElement("H");
    G4Element* elO  = nist->FindOrBuildElement("O");
    G4Element* elZr = nist->FindOrBuildElement("Zr");
    G4Element* elB  = nist->FindOrBuildElement("B");

    // Uranio enriquecido al 20%
    G4Isotope* U235 = new G4Isotope("U235", 92, 235, 235.0439242 * g/mole);
    G4Isotope* U238 = new G4Isotope("U238", 92, 238, 238.0507882 * g/mole);
    G4Element* elUEnriched = new G4Element("EnrichedUranium", "U", 2);
    elUEnriched->AddIsotope(U235, 20.0 * perCent);
    elUEnriched->AddIsotope(U238, 80.0 * perCent);

    // === Materiales individuales ===

    // Agua
    G4Material* water = nist->FindOrBuildMaterial("G4_WATER");

    // Ácido bórico H3BO3
    G4Material* boricAcid = new G4Material("BoricAcid", 1.6 * g/cm3, 3);
    boricAcid->AddElement(elH,3);
    boricAcid->AddElement(elB, 1);
    boricAcid->AddElement(elO, 3);

    // Hidruro de zirconio con Uranio enriquecido
    G4Material* UZrH = new G4Material("UZrH", 6.0 * g/cm3, 2);
    UZrH->AddElement(elUEnriched, 1);
    UZrH->AddElement(elH, 1);

    // Zirconio metálico
    G4Material* zirconium = nist->FindOrBuildMaterial("G4_Zr");

    // === Mezcla homogénea del núcleo ===
    G4Material* reactorMat = new G4Material("HomogeneousReactorMaterial", 2.7 * g/cm3, 4);
    reactorMat->AddMaterial(water,     0.48);
    reactorMat->AddMaterial(boricAcid, 0.0014);
    reactorMat->AddMaterial(UZrH,      0.5021);
    reactorMat->AddMaterial(zirconium, 0.0165);

    // === Mundo ===
    G4double world_size = 2.0 * m;
    G4Material* air = nist->FindOrBuildMaterial("G4_AIR");

    G4Box* solidWorld = new G4Box("World", world_size/2, world_size/2, world_size/2);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, {}, logicWorld, "World", 0, false, 0);

    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    // === Reactor (cilindro) ===
    G4double radius = 15.61 * cm;
    G4double height = 38.08 * cm;

    G4Tubs* solidReactor = new G4Tubs("Reactor", 0, radius, height/2, 0., 360.*deg);
    G4LogicalVolume* logicReactor = new G4LogicalVolume(solidReactor, reactorMat, "Reactor");
    new G4PVPlacement(0, {}, logicReactor, "Reactor", logicWorld, false, 0);

    G4VisAttributes* visReactor = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.1));
    visReactor->SetForceSolid(true);
    logicReactor->SetVisAttributes(visReactor);

    return physWorld;
}
