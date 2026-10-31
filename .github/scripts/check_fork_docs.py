#!/usr/bin/env python3
"""Check local Markdown links in the fork overview; standard library only."""
from pathlib import Path
import re
import sys
from urllib.parse import unquote, urlsplit


def main():
    root = Path(__file__).resolve().parents[2]
    files = [root / 'README.md', *sorted((root / 'docs/fork').glob('*.md'))]
    errors = []
    checked = 0
    for path in files:
        text = path.read_text(encoding='utf-8')
        if '\ufffd' in text:
            errors.append(f'{path.relative_to(root)}: replacement character')
        for raw in re.findall(r'\]\(([^)\s]+)(?:\s+"[^"]*")?\)', text):
            url = urlsplit(raw)
            if url.scheme or raw.startswith('//'):
                continue
            checked += 1
            target = (path.parent / unquote(url.path)).resolve() if url.path else path
            if not target.is_relative_to(root):
                errors.append(f'{path.relative_to(root)} -> {raw}: outside repository')
            elif not target.exists():
                errors.append(f'{path.relative_to(root)} -> {raw}: missing file')
            elif url.fragment and target.suffix == '.md':
                headings = [line.lstrip('#').strip() for line in
                            target.read_text(encoding='utf-8').splitlines() if line.startswith('#')]
                slugs = [re.sub(r'[^\w\- ]', '', h.lower()).replace(' ', '-') for h in headings]
                if unquote(url.fragment) not in slugs:
                    errors.append(f'{path.relative_to(root)} -> {raw}: missing heading')
    if errors:
        print('\n'.join(errors), file=sys.stderr)
        return 1
    print(f'PASS: {len(files)} Markdown files, {checked} local links/anchors')
    return 0


if __name__ == '__main__':
    sys.exit(main())
