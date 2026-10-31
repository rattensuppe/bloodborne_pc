#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Frozen console smoke check. No network, launcher window or saved settings."""
import hashlib
from pathlib import Path
import sys
import traceback


def check(against=None):
    import ssl
    import tkinter.font
    from PIL import Image
    import bbport_tls

    context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
    assert context.check_hostname and context.verify_mode == ssl.CERT_REQUIRED
    verified = bbport_tls.tls_context(stores=[])
    assert verified.check_hostname and verified.verify_mode == ssl.CERT_REQUIRED
    assert Image.new('RGB', (2, 2)).size == (2, 2)
    if against is not None:
        # A successful probe only covers the launcher when the native Python/SSL
        # binaries match. Neither executable needs the build PC's Python install.
        own = Path(sys._MEIPASS)
        other = Path(against)
        names = ['_ssl.pyd', *[p.name for p in own.glob('libcrypto*.dll')],
                 *[p.name for p in own.glob('libssl*.dll')],
                 f'python{sys.version_info.major}{sys.version_info.minor}.dll']
        assert len(names) >= 4, 'TLS runtime binaries missing from the smoke build'
        for name in names:
            assert hashlib.sha256((own / name).read_bytes()).digest() == hashlib.sha256((other / name).read_bytes()).digest(), name
    print('PASS: frozen TLS, certificate verification, Pillow and Tk font imports', flush=True)


def main():
    try:
        check(sys.argv[1] if len(sys.argv) > 1 else None)
    except Exception:
        traceback.print_exc()
        return 1
    return 0


if __name__ == '__main__':
    sys.exit(main())
