# Pokémon Colosseum PC Port — Session Handoff

**Scope of this doc:** continue ONLY the **PC port** (Ship-of-Harkinian–style native port).
**Do NOT** do any byte-match decompilation / matching-build work in this thread.

Branch: `chore/decomp-tooling-reboot`. Latest port commit: `be92efc7` (and descendants).
Repo: `C:\Users\douglaswhittingham\pkmn-colosseum`. Platform: Windows, PowerShell.

---

## 1. What the port is + current state (all committed + working)

The port (`src/pcport/`) is a host layer that runs the game's **own decompiled C** on
GX→OpenGL/GLFW shims (not emulation). It currently boots to a **static 3D title render**:

- `RunMenuScene` (src/pcport/pcport_main.c) loads `title.fsys:logo_demo`, parses the HSD
  scene, and renders it every frame: the **3D desert/ruins** scene + a 2D overlay (the crisp
  **Pokémon Colosseum logo**, **PRESS START**, **copyright**), with face-normal lighting.
- It is a **render harness**: a capped frame loop that exits on window close / frame cap.
  **No input, no game loop, no audio, no state machine, no save/memory-card.**

**Capabilities that already exist (reuse these):**
- FSYS member load + LZSS: `PCPort_LoadFsysMember` (real_content_host.c). LZSS bounds fix is in.
- HSD archive parse + relocate: `PCPort_HSDArchiveParseBE`; public-symbol resolve.
- Scene-graph render: `RenderJointTree` → the game's own GSgfx draw bridge `fn_800DAD10`.
- GC texture decode (gx_texture.c): RGBA8, RGB5A3, I8, CMPR all real; CI4/CI8 + TLUT done.
  Textures are **modulated by the material diffuse** (texture×diffuse) before upload.
- 2D ortho overlays: `BeginMenuOverlay` + `DrawTexturedScreenRect(tex, sx,sy,sw,sh, u0,v0,u1,v1)`
  (screen-space rect + UV sub-rect; gated lighting off so overlays stay full-bright).
- Camera: `BuildViewMatrixLookAt(eye,interest,up,viewMtx)`; the title uses the end-pose of
  `cam_logo_demo_stop` (eye≈(0,38.9,409.8), interest≈(0,39.65,1.56), fov 45).
- Face-normal directional lighting in the gx_tev GLSL shader, gated by a host
  `lightingEnabled` flag (3D scene on, 2D overlays off).
- Decoded menu/title sprites already exist as PNGs under `build_pc/logo_probe/` (e.g.
  `topmenu/menu_033_hdr_RGBA8_276x574.png` = the Story/Battle/Options panels).

**Env toggles (for testing):** `PCPORT_NO_RENDER_3D=1` (2D sky backdrop instead of 3D scene),
`PCPORT_RENDER_DEBUG=1` (per-PObj `[rjt]` diagnostics), `PCPORT_MENU_FRAMES=N` (frame cap),
`PCPORT_DUMP=path.bmp` (dump the framebuffer to BMP on the last frame),
`PCPORT_CAM_EYE/INT/UP="x,y,z"` (override the camera), `PCPORT_NO_TITLE_CAM=1`,
`PCPORT_MENU_ARCHIVE`/`PCPORT_MENU_MEMBER` (render a different fsys scene member).

---

## 2. Build / run / screenshot (IMPORTANT gotchas)

git + python are **NOT on PATH**. Prepend in every PowerShell call:
```
$env:PATH = "C:\Program Files\Git\cmd;C:\Users\douglaswhittingham\AppData\Local\Programs\Python\Python312;" + $env:PATH
```
python.exe: `C:\Users\douglaswhittingham\AppData\Local\Programs\Python\Python312\python.exe`.

**Build the shim lib** (only needed when a shim file — gx_*.c, *_shim.c, real_content_host.c —
changes): `powershell -File tools\pcport_build.ps1`.
> The final `pcport_bootstrap` CMake LINK step ALWAYS ends with ~35 unresolved
> `hsd_pobj_disp` externals — that is **EXPECTED/pre-existing**, not your bug. It still
> rebuilds `pcport_shim.lib`.

**Link the runnable exe** (the real build): `python tools\pcport_link.py`. Must print
`compiled 37 objects; 0 failed to compile` and `LINKED OK` (it auto-stubs ~187 residual
asm-only fn_/lbl_ symbols). If only `pcport_main.c` changed you can skip `pcport_build.ps1`
and just run `pcport_link.py`.

**Run:** `build_pc\pcport_bootstrap.exe --menu`. To capture a frame:
`$env:PCPORT_DUMP="build_pc\out.bmp"; $env:PCPORT_MENU_FRAMES="3"; build_pc\pcport_bootstrap.exe --menu`
then convert with Pillow to PNG and view it (the assistant can Read PNGs to visually verify).
For a live window the user can watch: set `PCPORT_MENU_FRAMES` high and run in the background.

---

## 3. Integration roadmap — turn the static title into the interactive flow

Target flow: **boot videos → title → PRESS START → save prompt → main menu
(Continue / New Game / Battle Colosseum) → mode**. This roadmap is the synthesis of a
5-agent read-only scoping pass (`wmsr9u7q2`) whose claims were verified against source —
the file:line and fn_ references below are confirmed. The four features are **NOT peers**:
#1 is a foundation the others structurally depend on; #4 is a large isolated project.

### ⭐ The single best FIRST increment (cheapest visible proof of interactivity, ~1 day)
In `pad_shim.c` + `pcport_main.c`, do exactly three things:
1. Implement `PADShim_UpdateKeyboard` (pad_shim.c:293) against **GLFW** (the file is scaffolded
   for SDL2 but the build links GLFW): `glfwGetKey(PCPort_GetHostWindow(), …)` →
   Enter=START(0x1000), Z=A(0x100), X=B(0x200), arrows=dpad(0x1/2/4/8). Key state is already
   fresh — `glfwPollEvents()` runs every frame in `VIWaitForRetrace_PC` (os_shim.c:379).
2. Add a host edge-detector: each frame call the shim `PADRead`, compute `pressed = held & ~prev`.
3. Change `RunMenuScene`'s `for(frame<frameCap)` (pcport_main.c:4930) to loop until
   `glfwWindowShouldClose`; on START-pressed, swap the overlay from `menu_018` → `menu_033`.
This links **no new TUs** and turns the static render into something that reacts to a button
and changes screen — the literal skeleton (`enum` state + input + present loop) #2/#3 plug into.

### (1) Input + hosted game-loop + state machine — THE FOUNDATION (do first). Effort: MEDIUM.
- **Take the HOST-DRIVEN path, NOT engine-hosting.** Refactor `RunMenuScene`→`RunGame(window)`
  with `enum { ST_TITLE, ST_SAVE_PROMPT, ST_MAIN_MENU }`, reuse the existing present skeleton
  (`VIWaitForRetrace_PC`→`ClearBackbuffer`→`GSgfx_BeginFrame`→draw→`GSgfxSwapBuffers(1)`).
  ~200–400 lines in `pcport_main.c` + filling `pad_shim.c`'s GLFW TODOs.
- **Keep `src/dolphin/pad/Pad.c` OUT of the link** — its `PADRead`/`PADInit` collide with the
  shim and (EnabledBits=0 on PC) would shadow it with no-input. `pcport_bootstrap` today links
  only the render bridge + shims (CMakeLists.txt:267-277) — no game-engine TUs. Good; keep it.
- **DEFER the authentic engine-hosted path** (LARGE→VERY-LARGE): linking
  `gs_thread.c`/`gs_task.c`/`gs_title.c`/`main.c`/`input.c` needs a host cooperative-**fibre**
  runtime for `fn_800A263C` (fiber create) + `fn_800F0308` (per-frame vsync yield the whole
  engine blocks on) + host stand-ins for the asm-only SI poll `fn_800AB150`/`fn_800D0F44`. The
  engine is a vsync-tied fibre scheduler with a broad extern closure — not worth it for boot/menu.

### (2) Main menu (Story / Battle / Options) — fold into #1. Effort: SMALL.
- Assets already decoded: `topmenu.fsys:menu_033` (RGBA8 276×574 = STORY/Continue/New Game,
  BATTLE/Colosseum/Battle Now, OPTIONS), PNG at `build_pc/logo_probe/topmenu/`; menu_016/031/032.
- In ST_MAIN_MENU: draw `menu_033` via `DrawTexturedScreenRect` + a cursor-highlight quad, move
  a cursor index on dpad-pressed, A selects. No engine, no new decode.
- (The scoping agent for this returned null — the parallel[2] StructuredOutput failure — but the
  asset/draw facts are corroborated by the other three results; treat as a small known increment.)

### (3) Post-START save prompt (Yes/No + save-presence check). Effort: MEDIUM (pragmatic).
- **The save/card subsystem is a decomp BLACK BOX** — `fn_801E1300/0FB4/1274/1B2C/12A0` and
  `menuCB_SaveLoad.c` (`menuCBBios_SaveDataAvailable`) have **NO C and NO `.s`/`.inc`** in the
  tree. You cannot link or reference it; **reimplement host-side**.
- Pragmatic host module: presence check = `does <save-dir>/colosseum.sav exist?` (stub
  "no save found" first — matches the game's own `GStitle_Init` fallback); write = a stdio blob.
  Skip the GCI/SHA-1 format (save.h: 0x40 header + SHA-1 via fn_801CC380, GC6E/01) unless
  Dolphin cross-compat is a goal.
- UI = a few `DrawTexturedScreenRect` quads (window frame + Yes/No + cursor). Sprites live in
  `save_menu.fsys` + `prog_memcard.fsys` — **not yet probed**; enumerate with the existing
  FSYS+HSD decode tooling first. The real game's dialog (`menu_dialog.c` fn_80059034) is GS
  retained-mode + message-bank text (`fn_8001E224`) — the port has NO text/glyph renderer, so
  bake prompt strings as sprites or add a minimal bitmap font. Don't port `menu_dialog.c`.

### (4) Boot THP videos — LARGE, isolated; its own parallel track. Effort: MEDIUM (video-only).
- **Key discovery: the orchestration + the ENTIRE THP player are already C-active.** Boot order
  in `src/game/movie.c`: `moviePlayGSLogo`(:410)→`moviePlayTPCLogo`(:420)→`moviePlayOpeningDemo`
  (:111); `movieWaitForFinish`(:82) is the per-movie blocking loop. The whole THP player/parser
  is C-active in `src/game/battle/battle_logic.c`: `THPPlayerOpen`=fn_801E189C, task entry
  fn_801E1924, open+parse fn_801E4778, `THPDraw`(YUV→GX-TEV) fn_801E1FF8, audio fn_801E2CA8,
  `THPPlayerGetState`=fn_801E1874. (These are GC-target pseudo-register C, not host-buildable
  as-is.) All 5 THP files present + verified (THP v1.1, 29.97fps): `movie/{gs_logo(silent),
  tpc(audio),openingdemo(64MB,audio),autodemo01(101MB),staffroll(109MB)}.thp`.
- **Take PATH A (host-side decoder), NOT Path B.** New `src/pcport/thp_player.c`:
  `PCPortTHP_Open/GetState/Stop/PumpFrame`. Reuse `dvd_shim.c` to **stream** frames (never load
  whole-file — openingdemo/autodemo/staffroll are 64–109MB). Parse the 0x40 BE header as
  fn_801E4778 does. The ONE genuinely new capability = a **baseline-JPEG/MJPEG video decoder**
  (THP frames are JPEG-derived: entropy-decode + dequant + IDCT → YUV planes); CPU YUV420→RGBA8
  (no host perf constraint); present via the **existing 2D fullscreen-quad path**
  (`DrawTexturedScreenRect`/`GXHostInitTexObjRGBA8`). Drive from a new `RunBootSequence(window)`
  state machine before the title, at 29.97fps with skip-on-key.
- Ship **video-only/muted first** (gs_logo/autodemo/staffroll are silent; opening plays fine
  muted). ADPCM audio (audio_shim.c stubbed) or faithful Path B (run the game's C player through
  GX-TEV + a cooperative task pump — same trap as engine-hosting) pushes it to LARGE.
- Hangs off only the shared quad-present helper + an optional skip-key → **can proceed in
  parallel** with #2/#3 on its own track.

### Dependency graph
`#1 INPUT+LOOP (foundation)` → `#2 MAIN_MENU` → `#3 SAVE_PROMPT`. `#4 THP` is parallel (only
needs the quad-present helper + a skip-key). Engine-hosting is an optional, deferred swap-out
of #1's host state machine — only revisit if the goal expands to authentic in-game scene logic.

---

## 4. Known issues / deferred

- **Desert pillars are tan-on-tan / low-contrast.** They render (positions confirmed) but in
  the port's reading they share the ground's haze texture (`0x293E0`) + diffuse (`B3B3B3`), so
  there's no material difference for lighting to reveal. Real fix = texture-assignment (why
  pillars resolve to the haze, not the sandstone `tex05/15/16`) + camera framing. **Deferred**,
  lower priority than the flow.
- The title horizon has a slight diagonal (a built-in geometry tilt the game's camera
  compensates for). Minor.
- No audio (audio_shim.c stubbed); no disc mount beyond loading named fsys members.

---

## 5. Constraints (hard rules)

- Edit only `src/pcport/**` (the host port) + build scripts (`tools/pcport_*`) + docs.
- Do **NOT** touch matching-build files: `*_fn_*.inc`, symbols, splits, `objdiff.json`,
  CodeWarrior configs. Do **NOT** flip `#if 0`→`#if 1` on asm wrappers. **No byte-match decomp.**
- The repo-public prep is done: API keys scrubbed from history; decomp.dev report wired
  (`master`, `GC6E01_report` artifact); MIT license. Don't redo those.

---

## 6. Memory / vault

- Auto-memory: `project_pc_port_state.md` is the landmark port note (read it first). Also
  `project_two_axis_metric.md`, `project_security_scrub_and_decompdev.md`.
- Vault session log: `Daily/2026-05-31.md` + `Daily/2026-06-01.md`; `Projects/pkmn-colosseum.md`.

## 7. First action in the fresh session

1. Read this doc + `project_pc_port_state.md` + the current `RunMenuScene` (pcport_main.c:4710,
   loop at :4930) + the 2D primitives (`BeginMenuOverlay`:472, `DrawTexturedScreenRect`:504,
   `LoadRawMenuTexObj`:538) + `pad_shim.c` (`PADShim_UpdateKeyboard`:293).
2. Confirm the build still runs (`pcport_link.py` → `--menu` → `PCPORT_DUMP` BMP).
3. Do the **⭐ single best first increment** in §3: GLFW input in `pad_shim.c` + host
   edge-detector + un-cap the loop + START swaps `menu_018`→`menu_033`. Links no new TUs, ~1 day,
   and yields the first real proof of interactivity.
4. From there, grow it into the `RunGame(window)` host state machine (#1) → main menu (#2) →
   save prompt (#3). THP video (#4) is a separate parallel track.
