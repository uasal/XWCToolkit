#!/usr/bin/env bash
set -euo pipefail

TIMEOUT_DUR="5s"
MAP_FILE="mapping.txt"
LOG_DIR="logs"
CONFIG_DIR="/opt/MagAOX/config"

mkdir -p "$LOG_DIR"
EXITCODE=0
missing_exes=()
missing_configs=()

# Verify all executables and configs exist
while read -r relpath id || [[ -n $relpath ]]; do
  [[ "$relpath" =~ ^# ]] && continue
  [[ -z "$id" ]] && continue

  # Check executable
  if [[ ! -x "$relpath" ]]; then
    missing_exes+=("$relpath")
  fi

  # Check config file
  config_path="$CONFIG_DIR/${id}.conf"
  if [[ ! -f "$config_path" ]]; then
    missing_configs+=("$config_path")
  fi
done < "$MAP_FILE"

# Report missing executables
if (( ${#missing_exes[@]} )); then
  echo ">>> ERROR: The following executables were not found or not executable:" >&2
  for exe in "${missing_exes[@]}"; do
    echo "  - $exe" >&2
  done
fi

# Report missing configs
if (( ${#missing_configs[@]} )); then
  echo ">>> ERROR: The following config files were not found:" >&2
  for config in "${missing_configs[@]}"; do
    echo "  - $config" >&2
  done
fi

# Exit if anything is missing
if (( ${#missing_exes[@]} || ${#missing_configs[@]} )); then
  exit 1
fi

leak_failures=()

# Run Valgrind on each executable
while read -r relpath id || [[ -n $relpath ]]; do
  [[ "$relpath" =~ ^# ]] && continue
  [[ -z "$id" ]] && continue

  log="$LOG_DIR/valgrind-${id}.log"

  echo "=== Valgrind on $id ($relpath) ==="
  timeout --preserve-status "$TIMEOUT_DUR" \
    valgrind \
      --leak-check=full \
      --show-leak-kinds=all \
      --track-origins=yes \
      --error-exitcode=1 \
      --log-file="$log" \
      "$relpath" -n "$id" \
    || EXITCODE=1

#   # Inline summary
#   echo ">>> ERROR SUMMARY for $id:"
#   grep "ERROR SUMMARY" "$LOG_DIR/valgrind-${id}.log" || true
# done < "$MAP_FILE"

  # Extract 'definitely lost' count; sum if multiple lines
  leaks=$(
    LC_ALL=C grep "definitely lost:" "$log" \
      | tr -d ',' \
      | awk '{ sum += $4 } END { print (sum ? sum : 0) }'
  )
  echo ">>> $id ($relpath): definitely lost = ${leaks} bytes"

  if [[ $leaks -gt 0 ]]; then
    leak_failures+=("$id ($relpath): ${leaks} bytes")
  fi

  echo
  echo
done < "$MAP_FILE"

# If any leaks were found, this should throw a failure
exit $EXITCODE
