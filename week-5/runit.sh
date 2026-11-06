#!/bin/sh
set -u
cd "$(dirname "$0")" || exit 1
[ -x ./page_sim_backend ] || { echo "page_sim_backend missing" >&2; exit 1; }
[ -f gui.py ] || { echo "gui.py missing" >&2; exit 1; }
S=$(mktemp -d) || exit 1
trap 'kill $P 2>/dev/null; rm -rf "$S"' EXIT INT TERM
PAGE_SIM_SOCKET="$S/s.sock" ./page_sim_backend "$S/s.sock" & P=$!
while [ ! -S "$S/s.sock" ]; do kill -0 $P 2>/dev/null || exit 1; sleep 0.1; done
PAGE_SIM_SOCKET="$S/s.sock" python3 gui.py "$S/s.sock"
