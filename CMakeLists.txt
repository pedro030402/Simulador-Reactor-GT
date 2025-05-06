cmake_minimum_required(VERSION 3.5)

project(ReactorSim)

# Buscar Geant4
find_package(Geant4 REQUIRED)

# Incluir directorios de encabezados
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Archivos fuente
set(SOURCES
    src/main.cc
    src/DetectorConstruction.cc
    src/PMPrimaryGenerator.cc
    src/PrimaryGeneratorAction.cc
    src/EventAction.cc
    src/RunAction.cc
    src/ActionInitialization.cc
    src/SteppingAction.cc
    src/MyNeutronPhysicsList.cc
)


# Ejecutable
add_executable(ReactorSim ${SOURCES})

# Incluir encabezados
target_include_directories(ReactorSim PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

# Enlazar con Geant4
target_link_libraries(ReactorSim ${Geant4_LIBRARIES})
