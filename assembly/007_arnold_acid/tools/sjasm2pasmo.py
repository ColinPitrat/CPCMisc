#!/usr/bin/env python3
"""Preprocess Patrik Rak's z80test sjasm sources into Pasmo-compatible Z80 assembly."""
import os
import re
import sys


def preprocess_patrik_z80test(src_file, out_file):
    src_dir = os.path.dirname(os.path.abspath(src_file))
    with open(src_file, "r", encoding="latin-1") as f:
        top_text = f.read()

    m_name = re.search(r'macro\s+testname\s+db\s+"([^"]+)"\s+endm', top_text)
    assert m_name, f"Could not find testname macro in {src_file}"
    testname_str = m_name.group(1)

    consts = {"selftests": 0}
    for k in ("maskflags", "onlyflags", "postccf", "memptr"):
        m = re.search(rf"^{k}\s+equ\s+(\d+)", top_text, re.M)
        assert m, f"Missing {k} in {src_file}"
        consts[k] = int(m.group(1))

    consts["opsize"] = 4 + consts["postccf"]
    consts["datasize"] = 16
    consts["vecsize"] = consts["opsize"] + consts["datasize"]
    consts["stop"] = 256 if consts["postccf"] else 0
    consts["tail"] = 0x3F if consts["postccf"] else 0

    def read_src(name):
        with open(os.path.join(src_dir, name), "r", encoding="latin-1") as f:
            return f.readlines()

    # Expand files in order: main.asm includes print.asm, idea.asm (includes crctab.asm), tests.asm
    raw_lines = []
    for line in read_src("main.asm"):
        s = line.strip()
        if s == "include print.asm":
            raw_lines.extend(read_src("print.asm"))
        elif s == "include idea.asm":
            for iline in read_src("idea.asm"):
                if iline.strip() == "include crctab.asm":
                    raw_lines.extend(read_src("crctab.asm"))
                else:
                    raw_lines.append(iline)
        elif s == "include tests.asm":
            raw_lines.extend(read_src("tests.asm"))
        else:
            raw_lines.append(line)

    out_lines = []
    for k, v in consts.items():
        out_lines.append(f"{k} equ {v}")

    cond_stack = []  # list of (active_bool, seen_true_bool)
    in_macro_def = False
    current_global = ""
    veccount = 0
    areg = 0

    def is_active():
        return all(c[0] for c in cond_stack)

    def eval_cond(expr):
        expr = expr.strip()
        if expr == "0":
            return False
        if expr == "! onlyflags":
            return not bool(consts["onlyflags"])
        if expr == "! ( onlyflags & maskflags )":
            return not bool(consts["onlyflags"] & consts["maskflags"])
        if expr in consts:
            return bool(consts[expr])
        raise ValueError(f"Unsupported conditional expression: {expr!r}")

    for line in raw_lines:
        # Strip comment
        code = line.split(";", 1)[0].rstrip()
        stripped = code.strip()
        if not stripped:
            continue

        # Handle macro definition blocks in idea.asm / testmacros.asm
        if re.match(r"^macro\b", stripped):
            in_macro_def = True
            continue
        if in_macro_def:
            if re.match(r"^endm\b", stripped):
                in_macro_def = False
            continue
        if stripped.startswith(".veccount") or stripped == "include testmacros.asm":
            continue

        # Handle conditionals
        m_if = re.match(r"^(if|elseif|else|endif)\b(.*)$", stripped)
        if m_if:
            kw, rest = m_if.group(1), m_if.group(2).strip()
            if kw == "if":
                parent_ok = is_active()
                val = eval_cond(rest) if parent_ok else False
                cond_stack.append([val, val])
            elif kw == "elseif":
                parent_ok = all(c[0] for c in cond_stack[:-1])
                if not parent_ok or cond_stack[-1][1]:
                    cond_stack[-1][0] = False
                else:
                    val = eval_cond(rest)
                    cond_stack[-1][0] = val
                    cond_stack[-1][1] = val
            elif kw == "else":
                parent_ok = all(c[0] for c in cond_stack[:-1])
                cond_stack[-1][0] = parent_ok and not cond_stack[-1][1]
                cond_stack[-1][1] = True
            elif kw == "endif":
                cond_stack.pop()
            continue

        if not is_active():
            continue

        # Skip duplicate equ definitions already emitted at top
        m_equ = re.match(r"^(\w+)\s+equ\s+", stripped)
        if m_equ and m_equ.group(1) in consts:
            continue

        # Handle equ definitions on a single line (do not split into label: + equ)
        if m_equ:
            rest_equ = stripped.replace("% 256", " MOD 256").replace("%256", " MOD 256")
            rest_equ = re.sub(r"\b(data|test)\.([A-Za-z_]\w*)", r"\1__\2", rest_equ)
            out_lines.append(rest_equ)
            continue

        # Check for label at start of line (non-indented or starting with .)
        label = None
        rest = stripped
        if not line[0].isspace():
            parts = stripped.split(None, 1)
            lbl_tok = parts[0]
            rest = parts[1] if len(parts) > 1 else ""
            if lbl_tok.endswith(":"):
                lbl_tok = lbl_tok[:-1]
            if lbl_tok.startswith("."):
                label = f"{current_global}__{lbl_tok[1:]}"
            else:
                current_global = lbl_tok
                label = lbl_tok
        elif stripped.startswith("."):
            parts = stripped.split(None, 1)
            lbl_tok = parts[0]
            rest = parts[1] if len(parts) > 1 else ""
            if lbl_tok.endswith(":"):
                lbl_tok = lbl_tok[:-1]
            label = f"{current_global}__{lbl_tok[1:]}"

        if label:
            out_lines.append(f"{label}:")
        if not rest:
            continue

        # Expand testname macro
        if rest == "testname":
            out_lines.append(f'    db "{testname_str}"')
            continue

        # Expand combine macro in idea.asm
        if rest.startswith("combine "):
            args = [a.strip() for a in rest[len("combine "):].split(",")]
            base = args[0]
            if base.startswith("."):
                base = f"{current_global}__{base[1:]}"
            count = consts["opsize"] - 2 if args[1] == "opsize-2" else consts[args[1]]
            offset = int(args[2]) if len(args) > 2 else 0
            last = int(args[3]) if len(args) > 3 else 1
            for idx in range(count):
                out_lines.append("    ld a,(bc)")
                out_lines.append("    xor (hl)")
                out_lines.append("    ex de,hl")
                out_lines.append("    xor (hl)")
                out_lines.append(f"    ld ({base}+{offset}+{idx}),a")
                if (idx < count - 1) or (not last):
                    out_lines.append("    inc c")
                    out_lines.append("    inc e")
                    out_lines.append("    inc l")
            continue

        # Expand flags macro
        if rest.startswith("flags "):
            args = [a.strip() for a in rest[len("flags "):].split(",")]
            # sn,s,zn,z,f5n,f5,hcn,hc,f3n,f3,pvn,pv,nn,n,cn,c
            vals = [int(args[i]) for i in (1, 3, 5, 7, 9, 11, 13, 15)]
            if consts["maskflags"]:
                bval = (
                    (vals[0] << 7) | (vals[1] << 6) | (vals[2] << 5) | (vals[3] << 4)
                    | (vals[4] << 3) | (vals[5] << 2) | (vals[6] << 1) | vals[7]
                )
                out_lines.append(f"    db 0x{bval:02x}")
            else:
                out_lines.append("    db 0xff")
            continue

        # Expand vec macro
        if rest.startswith("vec "):
            args = [a.strip() for a in rest[len("vec "):].split(",")]
            # op1,op2,op3,op4,memn,mem,an,a,fn,f,bcn,bc,den,de,hln,hl,ixn,ix,iyn,iy,spn,sp
            op1, op2, op3, op4 = args[0], args[1], args[2], args[3]
            mem_v, a_v, f_v = args[5], args[7], args[9]
            bc_v, de_v, hl_v, ix_v, iy_v, sp_v = args[11], args[13], args[15], args[17], args[19], args[21]
            a_int = int(a_v, 0)
            if consts["postccf"]:
                if (veccount % 3) == 0:
                    if op4 == "stop":
                        out_lines.append(f"    db {op1},{op2},{op3},tail,0")
                    elif op3 == "stop":
                        out_lines.append(f"    db {op1},{op2},tail,{op4},0")
                    elif op2 == "stop":
                        out_lines.append(f"    db {op1},tail,{op3},{op4},0")
                    else:
                        out_lines.append(f"    db {op1},{op2},{op3},{op4},tail")
                    areg = 0
                else:
                    out_lines.append(f"    db {op1},{op2},{op3},{op4},0")
                    areg = areg | a_int
            else:
                out_lines.append(f"    db {op1},{op2},{op3},{op4}")

            out_lines.append(f"    db {f_v}")
            if consts["postccf"] and ((veccount % 3) == 2):
                aval = a_int | ((~areg) & 0x28)
                out_lines.append(f"    db 0x{aval:02x}")
            else:
                out_lines.append(f"    db {a_v}")
            out_lines.append(f"    dw {bc_v},{de_v},{hl_v},{ix_v},{iy_v}")
            out_lines.append(f"    dw {mem_v}")
            out_lines.append(f"    dw {sp_v}")
            veccount += 1
            continue

        # Expand crcs macro
        if rest.startswith("crcs "):
            args = [a.strip() for a in rest[len("crcs "):].split(",")]
            # allflagsn,allflags,alln,all,docflagsn,docflags,docn,doc,ccfn,ccf,mptrn,mptr
            allflags, all_v, docflags, doc, ccf, mptr = (
                int(args[1], 0), int(args[3], 0), int(args[5], 0),
                int(args[7], 0), int(args[9], 0), int(args[11], 0),
            )
            if consts["postccf"]:
                chosen = ccf
            elif consts["memptr"]:
                chosen = mptr
            elif consts["maskflags"]:
                chosen = docflags if consts["onlyflags"] else doc
            else:
                chosen = allflags if consts["onlyflags"] else all_v
            b0, b1, b2, b3 = (chosen >> 24) & 0xFF, (chosen >> 16) & 0xFF, (chosen >> 8) & 0xFF, chosen & 0xFF
            out_lines.append(f"    db 0x{b0:02x}, 0x{b1:02x}, 0x{b2:02x}, 0x{b3:02x}")
            continue

        # Expand name macro
        if rest.startswith("name "):
            arg = rest[len("name "):].strip()
            out_lines.append(f"    db {arg}, 0")
            continue

        # Handle align 256
        if re.match(r"^align\s+256\b", rest):
            out_lines.append("    defs (($ + 255) and 0xff00) - $")
            continue

        # Fix sjasm instruction idioms for Pasmo
        if rest == "ex      af,af" or rest == "ex af,af":
            rest = "ex af,af'"
        elif rest == "add     (hl)" or rest == "add (hl)":
            rest = "add a,(hl)"

        # Replace modulo %256 / % 256 and local label references outside quotes
        if '"' not in rest and "'" not in rest:
            rest = rest.replace("% 256", " MOD 256").replace("%256", " MOD 256")
            rest = re.sub(r"\b(data|test)\.([A-Za-z_]\w*)", r"\1__\2", rest)
            rest = re.sub(r"(?<![\w.])\.([A-Za-z_]\w*)", lambda m: f"{current_global}__{m.group(1)}", rest)

        out_lines.append(f"    {rest}")

    with open(out_file, "w", encoding="latin-1") as f:
        f.write("\n".join(out_lines) + "\n")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit(f"Usage: {sys.argv[0]} <input.asm> <output.pasmo.asm>")
    preprocess_patrik_z80test(sys.argv[1], sys.argv[2])
