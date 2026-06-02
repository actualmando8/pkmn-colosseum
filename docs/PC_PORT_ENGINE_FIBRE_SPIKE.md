# P-A Spike — Host Cooperative-Fibre Runtime (engine-hosting keystone)

> **Status:** spike COMPLETE + PASSING, 2026-06-02. Implements the **P-A** row of
> `docs/PC_PORT_TO_PLAYABLE.md` §4 (the strategic fork's option **(B) authentic
> engine-hosting**). PC-port work only — functional C on host fibres, no
> byte-match decomp. All new code in `src/pcport/**`; the read-only engine
> sources (`src/game/*`, `src/dolphin/*`) were studied, never edited.

---

## 0. One-line outcome

A native **Win32-fibre cooperative runtime** that reimplements the *semantics* of
the engine's per-frame vsync-yield (`fn_800F0308`) and thread context-switch, and
**ticks real frames**: a thread shaped exactly like the real title thread
`gs_title.c fn_8002058C` (`init(); for(;;) yield();`) runs natively, round-tripping
to a host **present** fibre each frame. **The mechanism that gates all gameplay
(P-A) is de-risked and proven.**

---

## 1. Why a host fibre runtime (the problem)

The GameCube build runs the engine as a **cooperative fibre scheduler**. Threads
switch with PowerPC assembly that hand-saves/restores the full register file
(GPR/FPR/SP/LR) into a per-thread context block:

- **`fn_800F0308`** (`src/game/gs_texture.c`, asm) — the per-frame **vsync-yield**
  the whole engine blocks on. It saves the running thread's context (via
  `fn_800F015C`/`fn_800F01F0`), then loads the scheduler-selected *next* context
  (via `fn_800F0030`/`fn_800F00C0`) and `blr`s into it. The real title thread
  `fn_8002058C` is literally `lbl=0; fn_801EF644(-1); lbl=1; for(;;) fn_800F0308();`.
- **`src/dolphin/os/OSThread.c`** — the lower OSThread layer (`OSCreateThread`,
  `__OSThreadSwitch`, `OSLoadContext`/`OSSaveContext`), also asm-origin for the
  context switch. `fn_800A263C` (thread/fibre create) is asm-active here.

That asm **cannot run on x86**. To host the engine's own loop we must reimplement
the context-switch *semantics* — "suspend this coroutine, resume that one,
preserving its entire call stack" — natively. On x86 the clean primitive is
**Win32 Fibers** (`ConvertThreadToFiber`/`CreateFiber`/`SwitchToFiber`): each fibre
owns its stack + CPU state and a switch is explicit and non-preemptive — exactly a
cooperative coroutine.

**Key insight that makes this tractable:** on PowerPC the engine hand-marshalled
the register file because a coroutine switch *is* "swap SP/LR/GPRs." On x86 with
fibres, `SwitchToFiber` preserves the whole CPU + stack automatically, so we
**only reimplement scheduling (who runs next), not register marshalling.** The
`GSThreadCtx` GPR/FPR fields are irrelevant on host; each engine thread-context
maps 1:1 to a host fibre.

---

## 2. What was delivered (all in `src/pcport/`)

| File | Role |
|---|---|
| `os_thread_host.h/.c` | Generic, engine-agnostic **fibre primitive layer** over Win32 Fibers: `HostFibre_InitMain` (convert the host thread, float-state preserved), `HostFibre_Create`, `HostFibre_SwitchTo`, `HostFibre_Current`, resume accounting. POSIX `ucontext` / thread+condvar fallback is documented for portability (reports `HostFibre_Available()==0` off-Windows). |
| `engine_host.h/.c` | **Host reimplementation of the engine's cooperative scheduler** on that layer. `EngineHost_CreateThread` ≙ `OSCreateThread`/`fn_800F07A8` (→ `CreateFiber`); `EngineHost_VsyncYield` ≙ **`fn_800F0308`** (→ switch back to the host/scheduler fibre, returns next frame); `EngineHost_Run` is the host present-driver loop (round-robins every engine thread's per-frame slice, then presents). |
| `engine_spike.c` | The spike entry points + an engine thread shaped exactly like `fn_8002058C`. `RunFibreSelfTest()` (headless) and `RunEngineSpike(window)` (windowed). |
| `pcport_main.c` (minimal wiring) | `--fibre-test` (headless self-test, early return like `--thp-smoke`) and `--engine` (windowed present loop). The existing `--menu` front-end is untouched and still works. |
| `tools/pcport_link.py` | Added the three new TUs to the `BOOT` list. |

### The host↔engine model (single OS thread, cooperative)
```
   [host / present fibre]  <—— EngineHost_VsyncYield (== fn_800F0308) ——  [engine thread fibre]
            |  resume next frame (HostFibre_SwitchTo) ——————————————————————→  |
   VIWaitForRetrace_PC → clear → GSgfxSwapBuffers           init(); for(;;){ work; yield(); }
```
The host fibre is the scheduler/present driver. Each engine thread runs its
per-frame slice then yields; once all registered threads have yielded, the host
presents one frame and resumes them. This is the faithful shape of the real
engine's `for(;;) fn_800F0308()` thread loops driven by the GSgfx swap callback at
vsync.

---

## 3. Proof it works (reproducible)

Build: `python tools/pcport_link.py` → *compiled 41 objects; 0 failed; LINKED OK*.

**Headless mechanism self-test** (no GL):
```
build_pc\pcport_bootstrap.exe --fibre-test        # PCPORT_FIBRE_FRAMES=N (default 10)
```
Output (verbatim, trimmed):
```
[engine_host] cooperative-fibre scheduler initialised (host fibre live)
[spike] engine thread 'title': init slice running
[spike] frame 0 presented (host fibre); t0.work=1 t1.work=0
...
[spike] part A: ticked 10 frames; title.initDone=1 title.work=10
[spike] part A PASS: round-trip count matches frame count
[spike] part B: ticked 10 more frames; t0.work=20 t1.work=10
[spike] part B PASS: both engine threads sliced once per frame
[spike] === self-test PASS ===   (exit 0)
```
- **Part A** — one engine thread shaped like `fn_8002058C`: its init slice runs,
  then it yields once per frame for exactly N frames; the work-tick counter equals
  the frame count (the round-trip is 1:1 with vsync).
- **Part B** — a second engine thread (models `OSCreateThread` spawning a thread
  alongside the main idle loop): **both** threads are sliced exactly once per frame
  (round-robin cooperative scheduling), thread 0 continuing its counter from part A.

**Windowed present loop** (real host present path):
```
PCPORT_ENGINE_FRAMES=120  build_pc\pcport_bootstrap.exe --engine
```
→ 120 frames presented through `VIWaitForRetrace_PC → clear → GSgfxSwapBuffers`
driven *from the host fibre after the engine thread yielded*; `engine.work=120`,
`initDone=1`, clean exit. `PCPORT_ENGINE_FRAMES` unset ⇒ runs until window close.

This satisfies the P-A milestone ("the engine loop advances and you can prove it
TICKS… even 'runs N frames then cleanly yield/exit' proves P-A").

---

## 4. The real-engine boot cascade (assessment — the important finding)

The spike proves the **mechanism**. Booting the *actual* engine (`main.c` →
`GameInit` → `fn_800F07A8` → `for(;;) fn_800FE7A0()`) onto this runtime is **P-B**,
and the cascade is deeper than "link a few TUs":

1. **You do NOT link `gs_thread.c`.** It is a 153 KB grab-bag TU with **86
   still-asm functions** (`gs_task.c`: 42, `gs_texture.c`: 13, incl. the 6 context
   primitives + `fn_800F0308`/`fn_800F028C`). The C-active scheduler functions
   (`GStaskInit`/`GSthreadCreate`/`GStaskCreate`/`GStaskRun`/`GSthreadInit`) are
   *islands* in that asm sea — the TU will not compile host-side as-is (asm blocks
   + missing `.inc` for the non-threading functions), and `pcport_gen.py` only
   flips HSD TUs.
2. **The C-active scheduler sits on more asm/heap.** `GSthreadCreate` allocates the
   stack + `GSThreadCtx` from the **GSmem allocator** (`fn_800E2*/E3*` family),
   calls `fn_800A263C` (OSThread asm thread-create), and inits the ctx via the asm
   `fn_800F015C`/`fn_800F01F0`. The scheduler is wired to GSgfx via the swap
   callback `fn_800FEBA0` (asm).
3. **`GameInit` touches ~60 hardware subsystems before threading** — GX FIFO
   (`0xCC008000`), VI registers, ARAM, DVD, DSP/sound — each needing a host shim or
   stub. (`pcport_link.py` already auto-stubs 187 residual asm-only symbols, but
   *functional* boot needs real shims on the path, not no-op stubs.)

**Therefore the viable P-B path is NOT "link the engine's scheduler" but
"host-reimplement its small C scheduler surface" — which is precisely what
`engine_host.c` is the foundation for.** `GSthreadCreate`/`GStaskCreate`/
`GStaskRun`/`GSthreadInit` are ~6 well-understood functions; mirror their priority
list + task table on `engine_host`'s fibre model (mapping each `GSThread` → a host
fibre and each `GSTask` → a callback entry), feeding host-`malloc` stacks instead
of GSmem handles. The engine's *game* threads (the title/field/battle loops) then
run on this host scheduler **without linking `gs_thread.c` at all** — they only
need their own TUs + the leaf engine code converted to functional C (Track D).

### Still-asm on the immediate boot/scheduler path (the P-B worklist seed)
- `fn_800F0308`, `fn_800F028C`, `fn_800F015C`, `fn_800F0030`, `fn_800F01F0`,
  `fn_800F00C0` (`gs_texture.c`) — **superseded host-side** by `EngineHost_VsyncYield`
  / `HostFibre_SwitchTo`; no PPC port needed, just the host mapping.
- `fn_800A263C` (`OSThread.c`) — thread-create; host-side ≙ `HostFibre_Create`.
- `fn_800FEBA0` (swap callback), the GSmem `fn_800E2*/E3*` family — needed only if
  you link the real scheduler; **the reimplement-the-surface path avoids them.**
- SI-poll input stand-ins `fn_800AB150`/`fn_800D0F44` (per the handoff) — still to
  be located + host-stubbed against `pad_shim` when the real input path is wired.

---

## 5. Recommended next step (P-B)

Grow `engine_host.c` from the spike's stand-in thread into a faithful host
**GSthread/GStask** reimplementation (priority-ordered thread list + task table,
matching `gs_thread.c`'s C surface), then drive **one real engine game thread**
(e.g. a host-built title/field loop TU) on it — input via the `pad_shim` SI
stand-ins, present via the existing path. That converts P-A's proven mechanism into
"the engine's own per-frame logic runs," which is the gate to the field/overworld
(P-C). Audio (P-F) and the TEV→GLSL backend (P-G) remain independent parallel lanes.

---

## 6b. P-B increment 1 — the host GStask/GSthread scheduler (2026-06-02)

Building on the P-A mechanism, P-B stands up the engine's **own** cooperative
scheduler, host-reimplemented (not linked) per the §4 cascade finding.

**`src/pcport/gs_sched_host.{h,c}`** — a faithful host reimplementation of
`gs_thread.c`'s scheduler surface, using the **real** structs/enums from
`include/game/gs_thread.h` (`GSTask` 0x18, `GSThread` 0x24, `GSTASK_FREE/ACTIVE/
DEFERRED`) so it is a drop-in for engine callers:
- **Task layer** (`GStaskInit`/`GStaskCreate`/`GStaskRun`) — reproduced *verbatim*
  from the original C (priority-sorted linked-list insert, 1-based task IDs,
  free-slot search over normal/deferred regions, the per-frame `GStaskRun` walk
  that calls every `ACTIVE && !paused` task's `func(taskId, param)`). GSmem handle
  allocation is replaced by static pools; everything else is identical. This is
  the engine's *dominant per-frame mechanism* and is pure C — no asm.
- **Thread layer** (`GSthreadInit`/`GSthreadCreate`) — each `GSthread` maps to a
  host fibre (`os_thread_host`); `GSthreadYield` is the host equivalent of the
  asm vsync-yield `fn_800F0308`; `GSthreadStepAll` resumes each thread's per-frame
  slice. The GSmem stack/ctx + asm `fn_800F015C` context-init are replaced by the
  fibre.
- Exports `fn_` aliases (`fn_800FE9B0`/`834`/`7A0`, `fn_800F07A8`) so that when
  real engine TUs are linked they bind to this host scheduler instead of the
  `pcport_link.py` auto-stubs. (`fn_800F09D8` is deliberately **not** aliased —
  the decomp annotations conflict on whether it is `GSthreadInit` or render-timing.)

**Proof:**
- `--sched-test` (headless) **PASSES**: priority-ordered task run `[2,3,1]` for
  creation-order priorities `30,10,20`; correct 1-based IDs; `DEFERRED` tasks
  skipped by `GStaskRun`; free-slot reuse on re-init; two fibre-backed `GSthread`s
  each sliced exactly once per `GSthreadStepAll` for 5 steps.
- `--engine-boot` (windowed, `src/pcport/engine_boot.c`) mirrors `main.c`'s
  `GameInit` structure — `GSthreadInit(4)` + `GStaskInit(16,4)` (= the real
  `fn_800FE9B0(0x10,0x4)`), three priority-ordered tasks (one driving the real
  host present path `VIWaitForRetrace_PC → clear → GSgfxSwapBuffers`, exactly as
  the real `TaskVBlank` does), and a main thread created like the real
  `fn_800F07A8(0,0x3E8,0x4000,1,1,GameMainLoop)`. The engine's own main loop
  `for(;;){ GSthreadStepAll(); GStaskRun(); }` ran **120 frames**: main-thread
  work counter `1→120` (1:1 with frames), 3 tasks fired each frame, real present
  path driven by the scheduler. Clean exit. `--menu`/`--fibre-test` unaffected.

**What this proves:** the engine's *own* cooperative scheduler model (task table +
thread fibres, semantically identical to `gs_thread.c`) now drives the host frame
loop, with real present code executing under it as a registered task.

**P-B increment 2 (next):** drive **real engine render** under these tasks — load
`title.fsys:logo_demo` and run the real `RenderJointTree → fn_800DAD10` draw bridge
inside the VBlank task (instead of the flat clear), so the engine scheduler is
presenting the actual title scene. Then begin replacing host-stub task bodies with
the real engine callbacks (`TaskVBlank → fn_80175F6C` world render, etc.),
converting their still-asm leaves to functional C (Track D) as the call graph is
walked. The deepest step — linking the real `main.c GameInit` with its ~60
hardware-subsystem inits — remains gated on the GX-FIFO/VI/ARAM/DVD/DSP shims.

## 6. Constraints honored

Edited only `src/pcport/**` + `tools/pcport_*` + this doc. No `*_fn_*.inc`,
symbols, splits, `objdiff.json`, or CW configs touched; no `#if 0`→`#if 1`
asm-wrapper flips; no byte-match decomp. `src/game/*` and `src/dolphin/*` were
read-only references. The `--menu` front-end is verified still working.
