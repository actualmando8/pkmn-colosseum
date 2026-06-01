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
(Continue / New Game / Battle Colosseum) → mode**. Do it in this dependency order:

### (1) Input + game-loop + state machine — THE FOUNDATION (do first)
Everything interactive needs this. The port has no input and no transitions.
- Map GLFW keyboard/gamepad → the game's PAD reads. `pad_shim.c` is stubbed; wire START/A/B/
  d-pad to `PADRead`-equivalent state (src/dolphin/pad).
- Run a real per-frame loop + a scene/state machine. Trace `gs_thread.c`, `gs_task.c`, and
  the title main loop (`gs_title.c` fn_800203B4 / fn_80025730) to see how the game drives
  scene transitions, then let the port host that instead of the fixed `RunMenuScene` render.
- Minimal win: a `state` enum (TITLE → MENU) where pressing START advances it.
- Effort: **medium-large** (the foundation).

### (2) In-game main menu (Story / Battle / Options) — MOST TRACTABLE (do second)
- Sprites already decoded: `topmenu.fsys:menu_033` (RGBA8 276×574 = STORY MODE/Continue/New
  Game, BATTLE MODE/Colosseum Battle/Battle Now, OPTIONS). PNGs in `build_pc/logo_probe/topmenu/`.
- The game's menu is the **GS retained-mode 2D draw-slot system**: `fn_800FA280` (resource
  lookup) → `fn_80132A38` (draw-slot setter) → a per-frame consumer; driven by
  `src/game/menu/menu_middle.c` + `gs_title.c`. Either host that system, or (simpler first)
  render the `menu_033` sub-sprites as 2D overlays (`DrawTexturedScreenRect` with UV sub-rects)
  + a cursor + map selection input to the next state.
- Effort: **medium** (2D sprites the port already decodes + a cursor + selection).

### (3) Post-START save prompt (do third)
- The memory-card / save-file check + a yes/no dialog shown after START, before the menu.
- Trace the save/memcard subsystem (CARD* fns) + the prompt/dialog UI + its sprites.
- Effort: **medium**.

### (4) Boot THP videos — LARGEST, most isolated (separate sub-project)
- Nintendo / The Pokémon Company / Genius Sonority / opening demo = `movie/*.thp`
  (gs_logo.thp, tpc.thp, openingdemo.thp, …) on the disc.
- Needs a **host THP video+audio player**: file read → JPEG-like intra-frame decode → GL
  texture per frame + frame timing + ADPCM/PCM audio. The decompiled THP player is in
  `src/game/movie.c` (THPPlayerOpen=fn_801E189C, THPPlayerGetState=fn_801E1874,
  moviePlay*); the codec object files (GSmovie.o/THPRead.o/THPDraw.o/THPPlayer.o/
  THPVideoDecode.o/THPAudioDecode.o) were found as ELF `.o`s inside `pokemon_logo.fsys`
  (an `ar` archive) — they are NOT decompiled to C in `src/`.
- Effort: **large** — treat as its own project; do it last.

> A 4-agent scoping workflow (`wmsr9u7q2`, run id `wf_ce093685-ffa`) was launched to detail
> each of these (game-flow fn-level trace, assets, exact port steps, effort). If you want that
> agent-level detail, **re-run a focused scoping workflow** in the fresh session (it has a
> clean context budget) — this roadmap is enough to start without it.

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

1. Read this doc + `project_pc_port_state.md` + the current `RunMenuScene` in
   `src/pcport/pcport_main.c`.
2. Confirm the build still runs (`pcport_link.py` → `--menu` → BMP).
3. Start on **(1) input + game-loop foundation** — wire GLFW input through `pad_shim` and add
   a TITLE→MENU state transition on START — then **(2) the main menu**.
