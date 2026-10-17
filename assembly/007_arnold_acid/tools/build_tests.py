#!/usr/bin/env python3
"""Build or clean all Arnold Acid tests using pasmo and iDSK."""
import os
import shutil
import subprocess
import sys

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TOOLS_DIR = os.path.join(ROOT_DIR, "tools")
REPO_TOOLS_BIN = os.path.abspath(os.path.join(ROOT_DIR, "../../tools/install/bin"))
if os.path.isdir(REPO_TOOLS_BIN):
    os.environ["PATH"] = REPO_TOOLS_BIN + os.pathsep + os.environ.get("PATH", "")

sys.path.insert(0, TOOLS_DIR)
import addhead
import bin2cpr
import sjasm2pasmo

# Input binary assets referenced via incbin (MUST NOT be removed by clean)
INCBIN_ASSETS = {
    "lib/hw/sysfont.bin",
    "lib/spec/font.bin",
    "lib/spec/sysfont.bin",
    "cpc/mode3/mode3scr.bin",
    "cpctests/flash/pattern_mode1.bin",
    "cpctests/flash/pattern_mode2.bin",
    "cpctests/images/pattern_mode1.bin",
    "cpctests/images/pattern_mode1fa.bin",
    "cpctests/images/pattern_mode1fb.bin",
    "cpctests/images/pattern_mode2.bin",
    "cpctests/images/pattern_mode2fa.bin",
    "cpctests/images/pattern_mode2fb.bin",
    "crtc/intlace/lotus.bin",
    "crtc/intsync/imgbot.bin",
    "crtc/intsync/imgtop.bin",
    "snapshot/sprites.bin",
    "z80tests/woody/z80tests/48.rom",
}

# Suites driven by files.txt -> DSK mapping
# (rel_dir, dsk_map_by_disk_id, extra_pasmo_args)
FILES_TXT_SUITES = [
    ("aleste", {"1": "aleste.dsk"}, []),
    ("cpc", {"1": "cpc.dsk"}, []),
    ("cpc/power_on_low_rom", {"1": "lowrom.dsk"}, []),
    ("cpc/power_on_low_rom/visual", {"1": "poweron.dsk"}, []),
    ("cpcrom", {"1": "cpcrom.dsk"}, []),
    ("crtc", {"1": "crtc1.dsk", "2": "crtc2.dsk"}, []),
    ("devices/amram2", {"1": "amram2.dsk"}, []),
    ("devices/brunwordmk2", {"1": "brunwordmk2.dsk"}, []),
    ("devices/dkram", {"1": "dkram.dsk"}, []),
    ("devices/dobbertin", {"1": "dobbertin.dsk"}, []),
    ("devices/ide", {"1": "ide.dsk"}, []),
    ("devices/inicronram", {"1": "inicron.dsk"}, []),
    ("devices/kempston", {"1": "kempston.dsk"}, []),
    ("devices/megaflash", {"1": "megaflash.dsk"}, []),
    ("devices/mf2", {"1": "mf2.dsk"}, []),
    ("devices/multiplay", {"1": "multiplay.dsk"}, []),
    ("devices/playcity", {"1": "playcity.dsk"}, []),
    ("devices/rombo", {"1": "rombo.dsk"}, []),
    ("devices/sym2", {"1": "sym2.dsk"}, []),
    ("devices/vortexram", {"1": "vortexram.dsk"}, []),
    ("devices/vortexwd20", {"1": "vortexwd20.dsk"}, []),
    ("devices/xmem", {"1": "xmem.dsk"}, []),
    ("devices/yarek4mb", {"1": "yarek4mb.dsk"}, []),
    ("disc/drivetest", {"1": "drivetest.dsk"}, ["--equ", "CPC=1", "--equ", "SPEC=0"]),
    ("disc/fdc", {"1": "fdc.dsk"}, []),
    ("kccompact", {"1": "kcc.dsk"}, []),
    ("keyboard", {"1": "keyb.dsk"}, []),
    ("monitor", {"1": "monitor.dsk"}, []),
    ("plus", {"1": "asic1.dsk", "2": "asic2.dsk"}, []),
    ("ppi", {"1": "ppi.dsk"}, []),
    ("psg", {"1": "psg.dsk"}, ["--equ", "CPC=1", "--equ", "SPEC=0"]),
    ("snapshot", {"1": "snapshot.dsk"}, []),
    ("z80tests", {"1": "cpu.dsk"}, []),
]


def run_cmd(cmd, cwd):
    res = subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if res.returncode != 0:
        raise RuntimeError(
            f"Command failed in {cwd}: {' '.join(cmd)}\nSTDOUT:\n{res.stdout}\nSTDERR:\n{res.stderr}"
        )


def create_dsk_with_files(cwd, dsk_name, bin_files):
    dsk_path = os.path.join(cwd, dsk_name)
    if os.path.exists(dsk_path):
        os.remove(dsk_path)
    run_cmd(["iDSK", dsk_name, "-n"], cwd=cwd)
    for bf in bin_files:
        run_cmd(["iDSK", dsk_name, "-i", bf, "-f"], cwd=cwd)


def build_files_txt_suite(rel_dir, dsk_map, extra_args):
    cwd = os.path.join(ROOT_DIR, rel_dir)
    files_txt = os.path.join(cwd, "files.txt")
    disk_bins = {k: [] for k in dsk_map}
    with open(files_txt, "r", encoding="latin-1") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            parts = [p.strip() for p in line.split(",")]
            asm_base = parts[0]
            target_base = parts[1]
            disk_id = parts[2] if len(parts) > 2 else "1"
            out_bin = f"{target_base}.bin"
            run_cmd(["pasmo", "--amsdos", *extra_args, f"{asm_base}.asm", out_bin], cwd=cwd)
            disk_bins[disk_id].append(out_bin)
    for disk_id, dsk_name in dsk_map.items():
        if disk_bins[disk_id]:
            create_dsk_with_files(cwd, dsk_name, disk_bins[disk_id])


def build_extra_targets():
    # 1. aleste/aleste_ram_cpc.asm
    run_cmd(
        ["pasmo", "--amsdos", "aleste_ram_cpc.asm", "ram_cpc.bin"],
        cwd=os.path.join(ROOT_DIR, "aleste"),
    )

    # 2. cpc/mode3
    mode3_dir = os.path.join(ROOT_DIR, "cpc/mode3")
    run_cmd(["pasmo", "--amsdos", "mode3.asm", "mode3.bin"], cwd=mode3_dir)
    run_cmd(["pasmo", "--amsdos", "mode3tst.asm", "mode3tst.bin"], cwd=mode3_dir)
    create_dsk_with_files(mode3_dir, "mode3.dsk", ["mode3.bin", "mode3tst.bin"])

    # 3. cpctests
    cpctests_dir = os.path.join(ROOT_DIR, "cpctests")
    cpctests_list = [
        ("flood.asm", "flood.bin"),
        ("flood2.asm", "flood2.bin"),
        ("gaint.asm", "gaint.bin"),
        ("hsynclen.asm", "hsynclen.bin"),
        ("hsyncpos.asm", "hsyncpos.bin"),
        ("hsyncposlen.asm", "hsyncpl.bin"),
        ("im2.asm", "im2.bin"),
        ("intpos.asm", "intpos.bin"),
        ("kev.asm", "kev.bin"),
        ("raster.asm", "raster.bin"),
        ("vsync.asm", "vsync.bin"),
        ("vsynclen.asm", "vsynclen.bin"),
    ]
    for src, dst in cpctests_list:
        run_cmd(["pasmo", "--amsdos", src, dst], cwd=cpctests_dir)
    create_dsk_with_files(cpctests_dir, "cpctests.dsk", [dst for _, dst in cpctests_list])

    flood_sub = os.path.join(ROOT_DIR, "cpctests/flood")
    run_cmd(["pasmo", "--amsdos", "flood2.asm", "flood2.bin"], cwd=flood_sub)
    create_dsk_with_files(flood_sub, "flood2.dsk", ["flood2.bin"])

    images_sub = os.path.join(ROOT_DIR, "cpctests/images")
    img_list = [
        ("pattern_m1.asm", "pat_m1.bin"),
        ("pattern_m1f.asm", "pat_m1f.bin"),
        ("pattern_m2.asm", "pat_m2.bin"),
        ("pattern_m2f.asm", "pat_m2f.bin"),
    ]
    for src, dst in img_list:
        run_cmd(["pasmo", "--amsdos", src, dst], cwd=images_sub)
    create_dsk_with_files(images_sub, "patterns.dsk", [dst for _, dst in img_list])

    # 4. crtc/intlace, crtc/intsync, crtc/crtc
    intlace_dir = os.path.join(ROOT_DIR, "crtc/intlace")
    run_cmd(["pasmo", "--amsdos", "intlace.asm", "intlace.bin"], cwd=intlace_dir)
    create_dsk_with_files(intlace_dir, "intlace.dsk", ["intlace.bin"])

    intsync_dir = os.path.join(ROOT_DIR, "crtc/intsync")
    run_cmd(["pasmo", "--amsdos", "intsync.asm", "intsync.bin"], cwd=intsync_dir)
    create_dsk_with_files(intsync_dir, "intsync.dsk", ["intsync.bin"])

    # 5. crtc_detect
    crtc_det_dir = os.path.join(ROOT_DIR, "crtc_detect")
    det_list = [
        ("crtc_12_01_2005_v3_0_offset.asm", "ofstv30.bin"),
        ("crtc_amstlive_05_2000_madram.asm", "madram00.bin"),
        ("crtc_amstlive_06_2002_madram.asm", "madram02.bin"),
        ("batman_begins_rhino.asm", "rhino.bin"),
        ("crtc_13_04_1991_v1_1_longshot.asm", "longshot.bin"),
        ("crtc_v1_1_longshot.asm", "longsht2.bin"),
        ("alien3.asm", "alien3.bin"),
    ]
    for src, dst in det_list:
        run_cmd(["pasmo", "--amsdos", src, dst], cwd=crtc_det_dir)
    create_dsk_with_files(crtc_det_dir, "crtctype.dsk", [dst for _, dst in det_list])

    # 6. devices/csd
    csd_dir = os.path.join(ROOT_DIR, "devices/csd")
    run_cmd(["pasmo", "--amsdos", "csdtest.asm", "csdtest.bin"], cwd=csd_dir)
    create_dsk_with_files(csd_dir, "csdtest.dsk", ["csdtest.bin"])
    run_cmd(["pasmo", "--bin", "dummycart.asm", "dummycart.bin"], cwd=csd_dir)
    bin2cpr.build_cpr(
        os.path.join(csd_dir, "dummycart.bin"),
        os.path.join(csd_dir, "dummycart.cpr"),
    )

    # 7. disc/fdc_* and disc/fdctest3 and disc/morefdctest
    for sub in (
        "disc/fdc_gen",
        "disc/fdc_hang",
        "disc/fdc_lowlevel",
        "disc/fdc_read",
        "disc/fdc_seek",
        "disc/fdc_time",
        "disc/fdc_wp",
        "disc/fdc_write",
        "disc/fdctest3",
    ):
        d = os.path.join(ROOT_DIR, sub)
        run_cmd(["pasmo", "--equ", "CPC=1", "--equ", "SPEC=0", "--amsdos", "fdctest.asm", "fdctest.bin"], cwd=d)
        create_dsk_with_files(d, "cpc_fdctest.dsk", ["fdctest.bin"])

    fdc_po_dir = os.path.join(ROOT_DIR, "disc/fdc_poweron")
    run_cmd(["pasmo", "--bin", "fdctest.asm", "game.bin"], cwd=fdc_po_dir)
    run_cmd(["pasmo", "--bin", "single.asm", "cart.bin"], cwd=fdc_po_dir)
    bin2cpr.build_cpr(
        os.path.join(fdc_po_dir, "cart.bin"),
        os.path.join(fdc_po_dir, "cart.cpr"),
    )

    morefdc_dir = os.path.join(ROOT_DIR, "disc/morefdctest")
    morefdc_list = [
        ("checkdrv.asm", "checkdrv.bin"),
        ("readtim.asm", "readtim.bin"),
        ("smashhead.asm", "smashhd.bin"),
    ]
    for src, dst in morefdc_list:
        run_cmd(["pasmo", "--equ", "CPC=1", "--equ", "SPEC=0", "--amsdos", src, dst], cwd=morefdc_dir)
    create_dsk_with_files(morefdc_dir, "cpc_fdctest.dsk", [dst for _, dst in morefdc_list])

    # 8. dissassembler & examples & monitor/safezone
    diss_dir = os.path.join(ROOT_DIR, "dissassembler")
    run_cmd(["pasmo", "--amsdos", "opcodes.asm", "opcodes.bin"], cwd=diss_dir)
    create_dsk_with_files(diss_dir, "opcodes.dsk", ["opcodes.bin"])

    ex_dir = os.path.join(ROOT_DIR, "examples")
    run_cmd(["pasmo", "--amsdos", "8kscreen.asm", "8kscreen.bin"], cwd=ex_dir)
    run_cmd(["pasmo", "--amsdos", "flipscreens.asm", "flipscr.bin"], cwd=ex_dir)
    create_dsk_with_files(ex_dir, "examples.dsk", ["8kscreen.bin", "flipscr.bin"])

    sz_dir = os.path.join(ROOT_DIR, "monitor/safezone")
    run_cmd(["pasmo", "--amsdos", "safezone.asm", "safezone.bin"], cwd=sz_dir)
    create_dsk_with_files(sz_dir, "safezone.dsk", ["safezone.bin"])

    # 9. plus extras & power_on_test_cart
    plus_dir = os.path.join(ROOT_DIR, "plus")
    run_cmd(["pasmo", "--amsdos", "andy.asm", "andy.bin"], cwd=plus_dir)
    run_cmd(["pasmo", "--amsdos", "asicjoy.asm", "asicjoy.bin"], cwd=plus_dir)

    potc_dir = os.path.join(ROOT_DIR, "plus/power_on_test_cart")
    for i in range(1, 32):
        run_cmd(["pasmo", "--bin", f"page{i}.s", f"page{i}.bin"], cwd=potc_dir)
    run_cmd(["pasmo", "--bin", "test.s", "test.bin"], cwd=potc_dir)
    run_cmd(["pasmo", "--bin", "boot.s", "boot.bin"], cwd=potc_dir)
    with open(os.path.join(potc_dir, "cart.bin"), "wb") as out_f:
        with open(os.path.join(potc_dir, "boot.bin"), "rb") as in_f:
            out_f.write(in_f.read())
        for i in range(1, 32):
            with open(os.path.join(potc_dir, f"page{i}.bin"), "rb") as in_f:
                out_f.write(in_f.read())
    bin2cpr.build_cpr(
        os.path.join(potc_dir, "cart.bin"),
        os.path.join(potc_dir, "cart.cpr"),
    )

    potc_vis_dir = os.path.join(ROOT_DIR, "plus/power_on_test_cart/visual")
    run_cmd(["pasmo", "--bin", "poweron.asm", "poweron.bin"], cwd=potc_vis_dir)
    bin2cpr.build_cpr(
        os.path.join(potc_vis_dir, "poweron.bin"),
        os.path.join(potc_vis_dir, "poweron.cpr"),
    )

    # 10. rom
    rom_dir = os.path.join(ROOT_DIR, "rom")
    run_cmd(["pasmo", "--bin", "shortrom.s", "short.bin"], cwd=rom_dir)
    addhead.add_amsdos_header(
        os.path.join(rom_dir, "short.bin"),
        os.path.join(rom_dir, "shorthdr.bin"),
        load_addr=0xC000,
    )
    run_cmd(["pasmo", "--bin", "normrom.s", "norm.bin"], cwd=rom_dir)
    addhead.add_amsdos_header(
        os.path.join(rom_dir, "norm.bin"),
        os.path.join(rom_dir, "normhdr.bin"),
        load_addr=0xC000,
    )
    run_cmd(["pasmo", "--bin", "longrom.s", "long1.bin"], cwd=rom_dir)
    run_cmd(["pasmo", "--bin", "rom.s", "rom.bin"], cwd=rom_dir)
    with open(os.path.join(rom_dir, "long1.bin"), "rb") as f:
        b1 = f.read()
    with open(os.path.join(rom_dir, "long.bin"), "wb") as f:
        f.write(b1 + b1)
    os.remove(os.path.join(rom_dir, "long1.bin"))
    addhead.add_amsdos_header(
        os.path.join(rom_dir, "long.bin"),
        os.path.join(rom_dir, "longhdr.bin"),
        load_addr=0xC000,
    )
    with open(os.path.join(rom_dir, "message.txt"), "rb") as f:
        msg = f.read()
    for base in ("short", "shorthdr", "norm", "normhdr", "long", "longhdr"):
        with open(os.path.join(rom_dir, f"{base}.bin"), "rb") as f:
            b = f.read()
        with open(os.path.join(rom_dir, f"{base}extra.bin"), "wb") as f:
            f.write(b + msg)

    # 11. snapshot/snaptest1.asm
    snap_dir = os.path.join(ROOT_DIR, "snapshot")
    run_cmd(["pasmo", "--amsdos", "snaptest1.asm", "snaptst1.bin"], cwd=snap_dir)

    # 12. z80tests/patrik_z80test/cpc/z80test-1.0/src
    patrik_src = os.path.join(ROOT_DIR, "z80tests/patrik_z80test/cpc/z80test-1.0/src")
    patrik_parent = os.path.dirname(patrik_src)
    patrik_progs = [
        ("z80ccf", "z80ccf.bin", "z80ccf.dsk"),
        ("z80doc", "z80doc.bin", "z80doc.dsk"),
        ("z80docflags", "z80docfl.bin", "z80docflags.dsk"),
        ("z80flags", "z80flags.bin", "z80flags.dsk"),
        ("z80full", "z80full.bin", "z80full.dsk"),
        ("z80memptr", "z80mempt.bin", "z80memptr.dsk"),
    ]
    for prog, bin_name, dsk_name in patrik_progs:
        tmp_asm = os.path.join(patrik_src, f"_{prog}_pasmo.asm")
        data_bin = os.path.join(patrik_src, "data.bin")
        sjasm2pasmo.preprocess_patrik_z80test(os.path.join(patrik_src, f"{prog}.asm"), tmp_asm)
        run_cmd(["pasmo", "--bin", f"_{prog}_pasmo.asm", "data.bin"], cwd=patrik_src)
        os.remove(tmp_asm)
        run_cmd(["pasmo", "--amsdos", "cpc.asm", bin_name], cwd=patrik_src)
        create_dsk_with_files(patrik_src, dsk_name, [bin_name])
        shutil.copy2(os.path.join(patrik_src, dsk_name), os.path.join(patrik_parent, dsk_name))
    if os.path.exists(os.path.join(patrik_src, "data.bin")):
        os.remove(os.path.join(patrik_src, "data.bin"))

    # 13. z80tests/woody/z80tests
    woody_dir = os.path.join(ROOT_DIR, "z80tests/woody/z80tests")
    run_cmd(["pasmo", "--alocal", "--amsdos", "z80tests.asm", "z80tests.bin"], cwd=woody_dir)
    create_dsk_with_files(woody_dir, "z80tests.dsk", ["z80tests.bin"])


def build_all():
    for rel_dir, dsk_map, extra_args in FILES_TXT_SUITES:
        build_files_txt_suite(rel_dir, dsk_map, extra_args)
    build_extra_targets()


def clean_all():
    for root, dirs, files in os.walk(ROOT_DIR):
        for f in files:
            abs_p = os.path.join(root, f)
            rel_p = os.path.relpath(abs_p, ROOT_DIR).replace("\\", "/")
            if rel_p in INCBIN_ASSETS:
                continue
            _, ext = os.path.splitext(f)
            if ext.lower() in (".bin", ".dsk", ".cpr", ".cdt", ".lst", ".out") or f.startswith("_"):
                os.remove(abs_p)


if __name__ == "__main__":
    action = sys.argv[1] if len(sys.argv) > 1 else "build"
    if action == "build":
        build_all()
    elif action == "clean":
        clean_all()
    elif action == "build_dir":
        target_rel = sys.argv[2].strip("/")
        for rel_dir, dsk_map, extra_args in FILES_TXT_SUITES:
            if rel_dir == target_rel:
                build_files_txt_suite(rel_dir, dsk_map, extra_args)
                break
        else:
            sys.exit(f"Unknown files.txt suite directory: {target_rel}")
    else:
        sys.exit(f"Unknown action: {action}")
