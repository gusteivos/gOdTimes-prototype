#!/bin/bash
# FILE: appimage.sh
set -e

APPIMAGETOOL=${APPIMAGETOOL:-appimagetool}
APPDIR="AppDir"
USR_BIN="$APPDIR/usr/bin"
USR_LIB="$APPDIR/usr/lib"
ICON="$APPDIR/gOdTimes-prototype.png"
BIN="$USR_BIN/gOdTimes-prototype.out"
SDL_LIB="$USR_LIB/libSDL2-2.0.so.0"
LUA_LIB="$USR_LIB/liblua5.3.so.0"

if [[ "$1" == "clean" ]]; then
    rm -f "$BIN" "$SDL_LIB" "$LUA_LIB" "$ICON"
    rm -f "$APPDIR"/.DirIcon
    rm -rf *.AppImage
    exit 0
fi

cp gOdTimes-prototype.out "$USR_BIN/"
cp /lib/x86_64-linux-gnu/libSDL2-2.0.so.0 "$USR_LIB/"
cp /lib/x86_64-linux-gnu/liblua5.3.so.0 "$USR_LIB/"
chmod +x "$APPDIR/AppRun"

convert -size 256x256 xc:white -gravity center -pointsize 24 -fill black -annotate +0+0 "gOdTimes prototype" "$ICON"

"$APPIMAGETOOL" -v "$APPDIR"
