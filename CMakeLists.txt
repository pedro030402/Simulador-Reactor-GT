# Establece la versión mínima de CMake requerida para este proyecto
cmake_minimum_required(VERSION 3.5)

# Define el nombre del proyecto y crea un espacio de nombres para los archivos resultantes
project(ReactorSim)

# Busca la instalación de Geant4 en el sistema y marca como obligatoria
find_package(Geant4 REQUIRED)

# Incluye el archivo de configuración de Geant4 y los directorios de encabezados del proyecto
include(${Geant4_USE_FILE})  # Configura variables y rutas propias de Geant4
include_directories(${PROJECT_SOURCE_DIR}/include)  # Carpeta include del proyecto

# Especifica los archivos fuente que se compilarán
set(SOURCES
    src/main.cc                 # Función principal y punto de entrada
    src/DetectorConstruction.cc # Define la geometría del detector
    src/PMPrimaryGenerator.cc   # Generador de partículas primarias personalizado
    src/PrimaryGeneratorAction.cc # Control de la acción de generación primaria
    src/EventAction.cc          # Manejo de eventos (inicio/fin)
    src/RunAction.cc            # Acciones al inicio y fin de la corrida de simulación
    src/ActionInitialization.cc # Inicializa y conecta acciones del usuario
    src/SteppingAction.cc       # Acción en cada paso de transporte de partículas
    src/MyNeutronPhysicsList.cc # Lista de procesos físicos (neutrones)
)

# Crea el ejecutable llamado ReactorSim a partir de los archivos fuente definidos
add_executable(ReactorSim ${SOURCES})

# Añade directorios de encabezados al ejecutable para incluir en tiempo de compilación
target_include_directories(ReactorSim PUBLIC
    ${PROJECT_SOURCE_DIR}/include  # Directorio include como público para dependencias
)

# Enlaza el ejecutable con las bibliotecas proporcionadas por Geant4
target_link_libraries(ReactorSim ${Geant4_LIBRARIES})  # Librerías necesarias de Geant4
