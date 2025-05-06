#!/usr/bin/env python3
"""
calc_k.py — Balance de neutrones y cálculo de k-efectivo

Este script:
 - Analiza un archivo de log de Geant4 para contar eventos de fisión, captura,
   procesos inelásticos y fugas.
 - Detecta automáticamente el número de eventos ejecutados (/run/beamOn) en una macro.
 - Calcula el factor de multiplicación k-efectivo a partir de ganancias y pérdidas de neutrones.
"""

import re  # Para expresiones regulares en el parseo de texto
import sys
import argparse  # Para manejar argumentos de línea de comandos
from collections import Counter  # Para contar ocurrencias de procesos
from pathlib import Path  # Para manejo de rutas de archivos

# Valor promedio de neutrones por fisión (parámetro para k)
NU_BAR = 2.34   # neutrones medios generados por cada fisión

# Patrón para detectar el inicio de un bloque de track en el log
RE_TRACK_HDR = re.compile(
    r"\*.*Particle\s*=\s*(\w+).*Track ID\s*=\s*(\d+).*Parent ID\s*=\s*(\d+)", re.I
)
# Patrón para detectar el fin de un bloque de track
RE_TRACK_END = re.compile(r"^Track\s+\(trackID", re.I)

# Mapeo de nombres de procesos (en el log) a claves para el contador
STEP_COUNTERS = {
    "nFission":                 "fission_event",
    "nFissionHP":               "fission_event",
    "nCapture":                 "capture",
    "nCaptureHP":               "capture",
    "neutronInelastic":         "inelastic",
    "inelastic":                "inelastic",
    "photonNuclear":            "photofission_n",
    "G4GammaNuclearProcess":    "photofission_n",
}


def parse_log(log_path: Path) -> Counter:
    """Parsea el log y cuenta:
    - fission_event: eventos de fisión
    - capture: neutrones capturados
    - inelastic: procesos inelásticos
    - photofission_n: fotofisión inducida
    - leakage: neutrones que escapan a World
    """
    c = Counter()
    gamma_ids = set()  # IDs de tracks de gamma (para fotofisión)
    prev_vol = None    # Volumen anterior para detectar fuga
    in_track = False   # Bandera de si estamos dentro de un bloque de track

    # Leer línea a línea el archivo de log
    for raw in log_path.open(errors="ignore"):
        line = raw.rstrip()

        # Detectar inicio de bloque de track
        hdr = RE_TRACK_HDR.search(line)
        if hdr:
            in_track = True
            prev_vol = None
            part, tid, pid = hdr.group(1).lower(), int(hdr.group(2)), int(hdr.group(3))
            if part == "gamma":
                gamma_ids.add(tid)  # Registrar gamma para fotofisión en neutrones hijos
            elif part == "neutron" and pid in gamma_ids:
                c["photofission_n"] += 1
            continue

        # Detectar fin de bloque de track
        if in_track and RE_TRACK_END.match(line):
            in_track = False
            prev_vol = None
            continue

        # Saltar la cabecera de pasos (Step# …)
        if in_track and line.startswith("Step#"):
            continue

        # Procesar líneas de pasos dentro de un track
        if in_track and line.lstrip().startswith(tuple("0123456789")):
            tokens = line.split()
            if len(tokens) < 3:
                continue
            vol, proc = tokens[-2], tokens[-1]
            # Contar según el proceso encontrado
            if proc in STEP_COUNTERS:
                c[STEP_COUNTERS[proc]] += 1
            # Detectar fuga (de Reactor a World)
            if proc.startswith("Coupled") and prev_vol == "Reactor" and vol == "World":
                c["leakage"] += 1
            prev_vol = vol
            continue

    return c


def detect_macro_and_nevents() -> int:
    "Busca en ./macros, ../macros, ../../macros un .mac con '/run/beamOn N' y retorna N como número de eventos."
    for base in [Path.cwd(), Path.cwd().parent, Path.cwd().parent.parent]:
        macros_dir = base / "macros"
        if macros_dir.is_dir():
            for mac in macros_dir.glob("*.mac"):
                try:
                    for L in mac.open():
                        m = re.match(r'^\s*/run/beamOn\s+(\d+)', L)
                        if m:
                            return int(m.group(1))
                except Exception:
                    continue
    return None


def main():
    # Configuración de argumentos
    p = argparse.ArgumentParser(
        description="Balance de neutrones y k-efectivo"
    )
    p.add_argument("logfile", type=Path, help="Archivo de log generado por Geant4")
    p.add_argument(
        "-m", "--macro", type=Path,
        help="Macro .mac para extraer '/run/beamOn'"
    )
    args = p.parse_args()

    # Verificar que el log existe
    log = args.logfile
    if not log.exists():
        sys.exit(f"No encuentro el log: {log}")

    # Parsear el log y obtener contadores
    cnt = parse_log(log)

    # Determinar número de eventos (source) desde macro
    nevents = None
    if args.macro:
        if not args.macro.exists():
            sys.exit(f"No encuentro la macro: {args.macro}")
        # Leer macro indicada
        for L in args.macro.open():
            m = re.match(r'^\s*/run/beamOn\s+(\d+)', L)
            if m:
                nevents = int(m.group(1))
                break
    else:
        # Auto-detección en directorios macros
        nevents = detect_macro_and_nevents()

    if nevents is not None:
        cnt["source"] = nevents

    # Cálculo de k-efectivo:
    # Ganancias = source + NU_BAR * fission_event + inelastic + photofission_n
    gains = (
        cnt.get("source", 0)
        + cnt.get("fission_event", 0) * NU_BAR
        + cnt.get("inelastic", 0)
        + cnt.get("photofission_n", 0)
    )
    # Pérdidas = fission_event + capture + leakage
    losses = (
        cnt.get("fission_event", 0)
        + cnt.get("capture", 0)
        + cnt.get("leakage", 0)
    )
    k_eff = gains / losses if losses else float("nan")

    # Impresión de resultados resumidos
    print("\nBalance resumido")
    print("----------------")
    print(f"   Ganancias (source)             : {cnt.get('source',0):.2e}")
    print(f"   Ganancias (fisión)             : {(cnt.get('fission_event',0)*NU_BAR):.2e}")
    print(f"   Ganancias (n,in)               : {cnt.get('inelastic',0):.2e}")
    print(f"   Ganancias (fotofisión γ→n)     : {cnt.get('photofission_n',0):.2e}")
    print(f"   Pérdidas  (fisión incidente)   : {cnt.get('fission_event',0):.2e}")
    print(f"   Pérdidas  (captura)            : {cnt.get('capture',0):.2e}")
    print(f"   Pérdidas  (fuga geom.)         : {cnt.get('leakage',0):.2e}")
    print(f"\n→  k-efectivo ≈ {k_eff:.4e}")


if __name__ == "__main__":
    main()
