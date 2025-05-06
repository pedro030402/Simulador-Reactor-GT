#!/usr/bin/env python3
"""
calc_k.py — Balance de neutrones y k-efectivo

Detecta correctamente la fuga Reactor → World sin depender del ancho de columna.
Si no se pasa --macro, busca en ./macros, ../macros o ../../macros el primer .mac
que contenga '/run/beamOn N' y usa ese N como número de eventos (source).
"""

import re
import sys
import argparse
from collections import Counter
from pathlib import Path

NU_BAR = 2.34   # neutrones medios por fisión

# patrones para trazar bloques de track
RE_TRACK_HDR = re.compile(
    r"\*.*Particle\s*=\s*(\w+).*Track ID\s*=\s*(\d+).*Parent ID\s*=\s*(\d+)", re.I
)
RE_TRACK_END = re.compile(r"^Track\s+\(trackID", re.I)

# mapa de nombres de proceso a claves de contador
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
    """Parsea el log y cuenta fission, capture, inelastic, photofission y leakage."""
    c = Counter()
    gamma_ids = set()
    prev_vol = None
    in_track = False

    for raw in log_path.open(errors="ignore"):
        line = raw.rstrip()

        # inicio de bloque de track
        hdr = RE_TRACK_HDR.search(line)
        if hdr:
            in_track = True
            prev_vol = None
            part, tid, pid = hdr.group(1).lower(), int(hdr.group(2)), int(hdr.group(3))
            if part == "gamma":
                gamma_ids.add(tid)
            elif part == "neutron" and pid in gamma_ids:
                c["photofission_n"] += 1
            continue

        # fin de bloque de track
        if in_track and RE_TRACK_END.match(line):
            in_track = False
            prev_vol = None
            continue

        # salto cabecera de pasos
        if in_track and line.startswith("Step#"):
            continue

        # dentro de bloque: líneas de pasos
        if in_track and line.lstrip().startswith(tuple("0123456789")):
            tokens = line.split()
            if len(tokens) < 3:
                continue
            vol, proc = tokens[-2], tokens[-1]
            if proc in STEP_COUNTERS:
                c[STEP_COUNTERS[proc]] += 1
            if proc.startswith("Coupled") and prev_vol == "Reactor" and vol == "World":
                c["leakage"] += 1
            prev_vol = vol
            continue

        # (ya no usamos la línea primary tracks…)
    return c


def detect_macro_and_nevents() -> int:
    """Busca en ./macros, ../macros o ../../macros un .mac con /run/beamOn N y devuelve N."""
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
    p = argparse.ArgumentParser(description="Balance de neutrones y k-efectivo")
    p.add_argument("logfile", type=Path, help="Archivo de log (filtrado o no)")
    p.add_argument("-m", "--macro", type=Path,
                   help="Macro Geant4 para extraer /run/beamOn")
    args = p.parse_args()

    log = args.logfile
    if not log.exists():
        sys.exit(f"No encuentro el log: {log}")

    # Parseamos el log
    cnt = parse_log(log)

    # Determinamos 'source' desde la macro
    nevents = None
    if args.macro:
        if not args.macro.exists():
            sys.exit(f"No encuentro la macro: {args.macro}")
        # leemos macro indicada
        for L in args.macro.open():
            m = re.match(r'^\s*/run/beamOn\s+(\d+)', L)
            if m:
                nevents = int(m.group(1))
                break
    else:
        # auto-detección
        nevents = detect_macro_and_nevents()

    if nevents is not None:
        cnt["source"] = nevents

    # Cálculo de k-efectivo
    gains = (
        cnt["source"]
        + cnt["fission_event"] * NU_BAR
        + cnt["inelastic"]
        + cnt.get("photofission_n", 0)
    )
    losses = (
        cnt["fission_event"]
        + cnt["capture"]
        + cnt["leakage"]
    )
    k_eff = gains / losses if losses else float("nan")

    print("\nBalance resumido")
    print("----------------")
    print(f"   Ganancias (source)             : {cnt['source']:.2e}")
    print(f"   Ganancias (fisión)             : {(cnt['fission_event'] * NU_BAR):.2e}")
    print(f"   Ganancias (n,in)               : {cnt['inelastic']:.2e}")
    print(f"   Ganancias (fotofisión γ→n)     : {cnt.get('photofission_n', 0):.2e}")
    print(f"   Pérdidas  (fisión incidente)   : {cnt['fission_event']:.2e}")
    print(f"   Pérdidas  (captura)            : {cnt['capture']:.2e}")
    print(f"   Pérdidas  (fuga geom.)         : {cnt['leakage']:.2e}")
    print(f"\n→  k-efectivo ≈ {k_eff:.4e}")


if __name__ == "__main__":
    main()
