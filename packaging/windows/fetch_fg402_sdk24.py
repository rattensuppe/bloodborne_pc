#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Fetch the pinned SDK 2.4 ML-FG effect alongside unchanged upscaler DLLs."""
import argparse
import json
from pathlib import Path
import fetch_fsr24_upscaling as sdk

PIN = json.loads((Path(__file__).with_name('fsr24-fg402.json')).read_text(encoding='utf-8'))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--dir', type=Path, default=sdk.ROOT/'out/fsr24')
    parser.add_argument('--check', action='store_true')
    args = parser.parse_args()
    try:
        print('SDK 2.4 ML-FG verified:', sdk.prepare(args.dir, args.check, PIN))
    except (OSError, ValueError) as error:
        parser.exit(1, str(error)+'\n')


if __name__ == '__main__':
    main()
