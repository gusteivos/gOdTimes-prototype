#!/bin/bash
# FILE: release.sh
set -e

RELEASE_DIR="release"
ARCH_NAME="$(uname -m)"
PLATFORM_NAME="$(uname -s | tr '[:upper:]' '[:lower:]')"
ZIP_NAME="${RELEASE_DIR}-${PLATFORM_NAME}-${ARCH_NAME}.zip"

if [[ "$1" == "clean" ]]; then
    rm -rf "$RELEASE_DIR" "$ZIP_NAME"
    exit 0
fi

rm -rf "$RELEASE_DIR"
mkdir -p "$RELEASE_DIR"

cp ./*.AppImage "$RELEASE_DIR"/ 2>/dev/null || true
cp ./config.lua "$RELEASE_DIR/"
cp ./logic.lua "$RELEASE_DIR/"

7z a -tzip "$ZIP_NAME" "$RELEASE_DIR/"
