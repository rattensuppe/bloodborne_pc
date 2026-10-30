# SPDX-License-Identifier: GPL-3.0-or-later
from pathlib import Path
import sys
import unittest
ROOT=Path(__file__).resolve().parents[1]
sys.path[:0]=[str(ROOT/'launcher'),str(ROOT/'scripts')]
import bbport_fsr41 as family
import bbport_lang

class RepairTests(unittest.TestCase):
    def test_restart_status_is_actionable_and_not_a_hardware_rejection(self):
        state={'supported':False,'reason':'restart'}
        self.assertEqual(family.code(state),family.RESTART)
        self.assertEqual(family.STATUS_KEYS[family.RESTART],'Restart required')
        self.assertEqual(family.COLORS[family.RESTART],'#d9a441')
        self.assertIsNone(family.preferred_route([state]*4))
        self.assertEqual(family.preferred_route([state,{'supported':True},state,state]),1)
        for language in family.TEXTS:
            self.assertEqual(bbport_lang.table(language)['Restart required'],family.RESTART_TEXTS[language])
            self.assertEqual(bbport_lang.table(language)[family.AUTO_EXPOSURE_KEY],family.AUTO_EXPOSURE_TEXTS[language])

if __name__=='__main__':unittest.main()
