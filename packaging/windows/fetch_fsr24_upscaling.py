#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Fetch only the signed, pinned SDK 2.4 upscaling DLLs. No FG runtime updates."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import tempfile
import urllib.request

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
PIN = json.loads((HERE/'fsr24-upscaling.json').read_text(encoding='utf-8'))

def verified(path, row):
    try:
        with path.open('rb') as stream:
            return path.stat().st_size == row['bytes'] and hashlib.file_digest(stream, 'sha256').hexdigest() == row['sha256']
    except OSError:
        return False

def prepare(target, check=False):
    target = Path(target)
    for row in PIN['files']:
        path = target/Path(row['path']).name
        if verified(path, row):
            continue
        if check:
            raise ValueError('SDK 2.4 file missing or changed: '+path.name)
        target.mkdir(parents=True, exist_ok=True)
        url = f"https://raw.githubusercontent.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/{PIN['commit']}/{row['path']}"
        temporary = None
        try:
            with tempfile.NamedTemporaryFile(dir=target, suffix='.part', delete=False) as stream:
                temporary = Path(stream.name)
                request = urllib.request.Request(url, headers={'User-Agent':'bbport-fsr24-upscaling'})
                with urllib.request.urlopen(request, timeout=120) as response:
                    while block := response.read(1 << 20):
                        stream.write(block)
            if not verified(temporary, row):
                raise ValueError('SDK 2.4 file size or SHA256 mismatch: '+path.name)
            os.replace(temporary, path)
        finally:
            if temporary:
                temporary.unlink(missing_ok=True)
    return target

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--dir', type=Path, default=ROOT/'out/fsr24')
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    try:
        print('SDK 2.4 upscaling verified:', prepare(args.dir, args.check))
    except (OSError, ValueError) as error:
        parser.exit(1, str(error)+'\n')

if __name__ == '__main__':
    main()
