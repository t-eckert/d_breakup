#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction():G4VUserDetectorConstruction(),
fScoringVolume(0)
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Materials ----------------------------------------------------------------
  G4NistManager* NIST_Manager = G4NistManager::Instance();

  // Def Hydrogen, low-density gas as vacuum
  G4Material* vacuum = new G4Material(
    "Vacuum",                           // name
    1.,                                 // z
    1.01*g/mole,                        // a
    universe_mean_density,              // density
    kStateGas,                          // state
    0.1*kelvin,                         // temperature
    1.e-19*pascal                       // pressure
  );

  // Def Deuterium Isotope as Element
  G4Isotope* isoD = new G4Isotope(
    "Deuterium (Isotope)",              // name
    1,                                  // n protons
    2,                                  // z
    2.014*g/mole                        // a
  );
  G4Element* elD = new G4Element(
    "Deuterium",                        // name
    "D",                                // symbol
    1                                   // parts
  );
  elD->AddIsotope(isoD, 1*perCent);     // (isotope, ratio)

  // Def Oxygen
  G4Element* elO = new G4Element(
    "Oxygen",                           // name
    "O",                                // symbol
    8.,                                 // z
    16.00*g/mole                        // a
  );

  // Def Heavy Water
  G4Material* D2O = new G4Material(
    "Heavy Water",                      // name
    1.11*g/cm3,                         // density
    2                                   // parts
  );
  D2O->AddElement(elD,2);               // (element, n atoms)
  D2O->AddElement(elO,1);               // (element, n atoms)

  // Def pure deuterium
  G4Material* D = new G4Material(
    "Deuterium",                        // name
    1.11*g/cm3,                         // density
    1                                   // parts
  );
  D->AddElement(elD,1);               // (element, n atoms)

  // Def polyethylene
  G4Material* polyethylene = NIST_Manager->FindOrBuildMaterial("G4_POLYETHYLENE");

  // World --------------------------------------------------------------------
  G4double world_size_XY = 20*cm;
  G4double world_size_Z = 20*cm;

  G4Material* world_material = vacuum;

  // Def size of Chamber
  G4Box* solid_world = new G4Box(
    "Chamber",                          // name
    0.5*world_size_XY,                  // x length
    0.5*world_size_XY,                  // y length
    0.5*world_size_Z                    // z length
  );

  // Def logic for Chamber
  G4LogicalVolume* logic_world = new G4LogicalVolume(
    solid_world,                        // solid instance
    world_material,
    "Chamber"
  );

  G4VisAttributes* World_Vis_Att = new G4VisAttributes(G4Colour(1.,1.,0.));
  World_Vis_Att->SetForceWireframe(true);
  logic_world->SetVisAttributes(World_Vis_Att);

  G4VPhysicalVolume* phys_world = new G4PVPlacement(
    0,                                  // rotation
    G4ThreeVector(),                    // at (0,0,0)
    logic_world,                        // logical volume
    "Chamber",                          // name
    0,                                  // mother volume
    false,                              // no boolean operation
    0,                                  // copy number
    true                                // check overlaps
  );
  
  // D Plane -----------------------------------------------------------------

  // Def materials and location of sphere
  G4Material* material_D_plane = D;
  G4ThreeVector origin = G4ThreeVector(0,0,0);

  // Def sphere geometry
  G4double r_min=0.*cm, r_max=1*cm;
  G4double phi_min=0., phi_max=twopi;
  G4double theta_min=0., theta_max=pi;

  // Def solid volume
  G4Box* solid_D_plane = new G4Box(
    "D Plane",                          // name    
    2*cm,                                // radius
    2*cm,                               // phi angle
    0.01*cm                              // theta angle
  );

  // Def logical volume
  G4LogicalVolume* logic_D_plane = new G4LogicalVolume(
    solid_D_plane,                      // solid
    material_D_plane,                   // heavy water
    "D Plane"                            // name
  );
  
  // Place the shape in space
  new G4PVPlacement(
    0,                                  // no rotation
    origin,                             // location at origin
    logic_D_plane,                      // logical volume
    "D Plane",                          // name
    logic_world,                        // mother volume
    false,                              // overlapping structure
    0,                                  // copy number
    true                                // check for overlapping
  );

  // Scoring Volume -----------------------------------------------------------
  
  // Def materials and location of SV
  G4Material* material_SV = polyethylene;
  G4ThreeVector position_SV = G4ThreeVector(0,0,0);

  // SV def as thin plane (10,10,0.01) cm
  // Distances are given from halfpoint
  G4double xy_SV = 5*cm, z_SV = 0.005*cm;
  
  // Def solid volume
  G4Sphere* solid_SV = new G4Sphere(
    "Scoring Volume",                   // name    
    5*cm, 5.01*cm,                      // radius
    0, twopi,                           // phi angle
    0, pi                               // theta angle
  );

  // Def logical volume
  G4LogicalVolume* logic_SV = new G4LogicalVolume(
    solid_SV,                           // solid
    material_SV,                        // polyethylene
    "Scoring Volume"                    // name
  );

  G4VisAttributes* SV_Vis_Att = new G4VisAttributes(G4Colour(0.,1.,1.));
  SV_Vis_Att->SetForceWireframe(true);
  logic_SV->SetVisAttributes(SV_Vis_Att);

  // Place the shape in space
  new G4PVPlacement(
    0,                                  // rotation
    position_SV,                        // (0,0,-80) cm
    logic_SV,                           // logical volume
    "Scoring Volume",                   // name
    logic_world,                        // mother volume
    false,                              // overlapping structure
    0,                                  // copy number
    true                                // check overlapping
  );
  
  // Def scoring volume
  fScoringVolume = logic_SV;

  return phys_world;
}