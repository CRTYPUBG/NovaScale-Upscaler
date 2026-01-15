# -*- mode: python ; coding: utf-8 -*-


a = Analysis(
    ['ui\\main.py'],
    pathex=['ui'],
    binaries=[],
    datas=[('ui/novascale.dll', '.'), ('ui/app.ico', '.'), ('ui/slpsh_screen.png', '.'), ('ui/logo.png', '.'), ('shaders/basic_bilinear.hlsl', 'shaders')],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    noarchive=False,
    optimize=0,
)
pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name='NovaScale',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=False,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
    icon=['ui\\app.ico'],
)
