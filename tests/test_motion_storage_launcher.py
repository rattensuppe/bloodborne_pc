# SPDX-License-Identifier: GPL-3.0-or-later
"""Check the launcher guard against native storage-path metadata without loading the UI."""
import ast,re,unittest
from pathlib import Path

class StorageGuard(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        source=Path(__file__).resolve().parents[1]/'launcher/bbport_launcher_win.py'
        tree=ast.parse(source.read_text(encoding='utf-8'))
        node=next(n for n in tree.body if isinstance(n,ast.FunctionDef) and n.name=='object_motion_unavailable')
        scope={'re':re,'_':lambda en,*a:en}
        exec(compile(ast.Module(body=[node],type_ignores=[]),str(source),'exec'),scope)
        cls.check=staticmethod(scope['object_motion_unavailable'])
    def test_rx6000_native_ssbo(self):
        self.assertEqual(self.check(dict(vendor_id=0x1002,device_id=0x73bf,object_motion_ssbo=True),
                                    'RX6800XT, discrete, 16384 MiB'),'')
    def test_unknown_or_old_probe_keeps_guard(self):
        for flag in (None,False,'true'):
            self.assertTrue(self.check(dict(vendor_id=0x1002,device_id=0x73bf,object_motion_ssbo=flag),
                                       'RX6800XT, discrete, 16384 MiB'))
    def test_low_vram_guard_retained(self):
        self.assertIn('6 GB',self.check(dict(vendor_id=0x1002,device_id=0x743f,object_motion_ssbo=True),
                                       'RX6500XT, discrete, 4096 MiB'))
    def test_other_gpus_unchanged(self):
        for vendor,device in ((0x1002,0x744c),(0x1002,0x7550),(0x10de,0x2684),(0x8086,0x56a0)):
            self.assertEqual(self.check(dict(vendor_id=vendor,device_id=device),'GPU, discrete, 16384 MiB'),'')
        self.assertTrue(self.check(dict(vendor_id=0x1002,device_id=0x731f),'RX5700, discrete, 8192 MiB'))
    def test_explicit_override_unchanged(self):
        self.assertEqual(self.check(dict(vendor_id=0x1002,device_id=0x731f),
                                    'GPU, discrete, 4096 MiB','BB_OBJECT_MOTION=1'),'')

if __name__=='__main__':unittest.main()
