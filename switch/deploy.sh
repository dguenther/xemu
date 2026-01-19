#!/bin/bash
# Deploy script for Switch development
# Usage: ./deploy.sh [SWITCH_IP]
# Uses the same devkitpro container as quick-build.sh for consistency

SWITCH_IP="${1:-}"

if [ -z "$SWITCH_IP" ]; then
    echo "Usage: $0 <SWITCH_IP>"
    echo ""
    echo "Example: $0 192.168.1.100"
    echo ""
    echo "Make sure your Switch is running in Homebrew Menu"
    echo "(nxlink server starts automatically)"
    exit 1
fi

if [ ! -f "dist-switch/xemu.nro" ]; then
    echo "Error: dist-switch/xemu.nro not found"
    echo "Run ./switch/quick-build.sh first"
    exit 1
fi

echo "=== Deploying to Switch at $SWITCH_IP ==="
echo "Make sure your Switch is in Homebrew Menu..."
echo ""
echo "Launching xemu via nxlink (stdout/stderr will appear below)..."
echo "Press Ctrl+C to stop watching logs (app will continue running on Switch)"
echo "─────────────────────────────────────────────────────────────────"
echo ""

# Deploy via nxlink inside the devkitpro container
# Use --network=host so the container can reach the Switch on the local network
podman run --rm -t --network=host -v "$(pwd):/src:z" -w /src devkitpro/devkita64 \
    nxlink -s dist-switch/xemu.nro -a "$SWITCH_IP"

echo ""
echo "─────────────────────────────────────────────────────────────────"
echo "Deployment complete!"
echo ""
echo "Note: If you see connection errors, verify:"
echo "  1. Switch IP is correct: $SWITCH_IP"
echo "  2. Switch is in Homebrew Menu (not running another app)"
echo "  3. Switch and PC are on the same network"
