# PC Port Batch 5 Tasklist

Tracked critical-path work after Batch 4C and the HSD host-loader takeover.

## Batch 5A - JObj Graph-Safe Walkers

- [x] Confirm the authoritative PC-port build path is `python tools\pcport_link.py`.
- [x] Identify the tree-recursive JObj walkers that must tolerate shared instance graphs.
- [x] Add host graph-safe walker overrides for animation and removal paths.
- [x] Add graph-safe dirty/flag propagation for shared-instance readiness.
- [x] Relink `build_pc\pcport_bootstrap.exe`.
- [x] Run targeted character-animation and headed field smoke checks.
- [x] Record verification evidence in the tasklist and memory doc.

Verification:

- `python tools\pcport_link.py` -> `compiled 129 objects; 0 failed to compile`, round 2 linked OK with 1708 stubs, rebuilt `build_pc\pcport_bootstrap.exe`.
- `Select-String build_pc\obj\hsd_host.o` confirmed BOOT object symbols for `HSD_JObjSetFlagsAll`, `HSD_JObjClearFlagsAll`, `HSD_JObjSetMtxDirtySub`, `HSD_JObjRemoveAll`, `HSD_JObjRemoveAnimAll`, `HSD_JObjReqAnimAll`, `HSD_JObjAddAnimAll`, and `HSD_JObjAnimAll`.
- `PCPORT_CHARANIM_BANK_PROBE=12 build_pc\pcport_bootstrap.exe` completed successfully; motion bank reported 11 motions and live frame deltas for non-static motions.
- Headed field smoke launched `build_pc\pcport_bootstrap.exe --field`; after 5 seconds the `Pokemon Colosseum PC Bootstrap` window was responding.

## Batch 5B - hsd_jobj Functional Holes

- [x] Decompile or host-bridge the live-impact instance/reference helpers `fn_801A0744` and `fn_801A0D94`.
- [x] Triage `fn_801A0744` and `fn_801A0D94` with DeepSeek V4 Flash.
- [ ] Triage remaining `hsd_jobj` TODO wrappers from `tools/decomp_work/_interesting_reordered.json`.
- [x] Verify that new JObj work changes linked PC-port runtime behavior.
- [x] Route direct `fn_801A0FBC` loader calls through the active PC host loader.

Active lane:

- Worktree: `C:\Users\douglaswhittingham\pkmn-colosseum`
- Branch: `master`
- Worker focus: active host equivalent of `fn_801A0744` / `fn_801A0D94` in `pcport\hsd_host.c`, because the linked PC-port `HSD_JObjLoadJoint` is the BOOT-order host loader.

Lane result:

- No overlay landed; the first `fn_801A0744` / `fn_801A0D94` candidate was rejected as unsafe and reverted in the worktree.
- Blocker: the real Colosseum `.inc` path includes ref-count transitions through `fn_801A0D48`, `fn_801A0D3C`, `fn_801A0CE8`, `fn_801A0C9C`, `fn_801A0C68`, and ID lookup through `fn_8019C128`; the candidate leaned too much on upstream Melee semantics.
- Main-checkout follow-up landed the live-impact host behavior instead of a generated-copy overlay: `pcport\hsd_host.c` now records a descriptor-to-live-JObj map during `HSD_JObjLoadJoint`, skips private child recursion for `JOBJ_INSTANCE` descriptors, and resolves each instance child to the canonical live JObj after all canonical nodes are loaded. This mirrors the practical `fn_801A0744` / `fn_801A0D94` ID-table lookup semantics in the active PC-port path.
- Host ref-count note: the PC loader intentionally does not increment the shared child ref count when wiring an instance child. The graph-safe `HSD_JObjRemoveAll` ownership path visits and destroys each live JObj pointer once; adding the original assembly ref bump here would leave shared children alive after host graph removal.
- `pcport\pcport_main.c` now exposes `--jobj-instance-smoke`, a headless smoke that constructs a root/canonical-child/instance-sibling descriptor graph, loads it through public `HSD_JObjLoadJoint`, requires `instance->child == root->child`, then runs graph-safe flag, animation, and removal walkers.
- Follow-up: `build_pc\bodies\hsd_jobj\fn_801A0744.c` now replaces the generated neutral `FLIP_AS_STUB` body for direct PC-port calls. It builds a descriptor-to-live lookup by walking paired JObj/Joint trees, resolves RObj/DObj refs, and wires `JOBJ_INSTANCE` children to the canonical live JObj. This makes generated `build_pc\gen\hsd\hsd_jobj.c` contain a real typed `void fn_801A0744(HSD_JObj*, HSD_Joint*)` body.
- `pcport\pcport_main.c` now exposes `--jobj-resolve-smoke`, a direct smoke that allocates a live root/canonical-child/instance-sibling JObj graph, calls `fn_801A0744(root, joint)` directly, and requires the instance child to resolve to the canonical live child before running graph-safe walkers.
- Follow-up: `build_pc\bodies\hsd_jobj\fn_801A0FBC.c` now replaces the generated direct-loader wrapper with a small PC overlay that calls the active host `HSD_JObjLoadJoint`. This bypasses the generated `fn_801A1098` method path for direct `gs_material.c` callers while preserving the already-verified host ownership and shared-instance behavior.
- `pcport\pcport_main.c` now exposes `--jobj-load-wrapper-smoke`, which loads a descriptor graph through `fn_801A0FBC` directly, requires instance-child resolution to the canonical live child, and then runs graph-safe flag, animation, and removal walkers.
- Next gameplay-readiness action: move past HSD loader plumbing into a bounded new-game/field progression smoke that exercises player spawn, collision/update, script/event dispatch, and NPC interaction triggers.

Verification:

- `python tools\pcport_link.py` -> `compiled 129 objects; 0 failed to compile`, round 2 linked OK with 1711 stubs, rebuilt `build_pc\pcport_bootstrap.exe`.
- `build_pc\pcport_bootstrap.exe --jobj-load-wrapper-smoke` -> passed with `JObj fn_801A0FBC direct loader wrapper exercised`.
- `build_pc\pcport_bootstrap.exe --jobj-instance-smoke` -> passed with `JObj instance child resolved to canonical live child`.
- `build_pc\pcport_bootstrap.exe --jobj-resolve-smoke` -> passed with `JObj fn_801A0744 direct resolver exercised`.
- `build_pc\pcport_bootstrap.exe --real-material-delta-smoke`
- `build_pc\pcport_bootstrap.exe --real-scene-slice-2-smoke`
- `build_pc\pcport_bootstrap.exe --real-textured-scene-slice-smoke`
- `build_pc\pcport_bootstrap.exe --real-scene-slice-4-smoke`
- `build_pc\pcport_bootstrap.exe --real-tev-scene-slice-3-smoke`
- `PCPORT_CHARANIM_BANK_PROBE=12 build_pc\pcport_bootstrap.exe`
- `PCPORT_MENU_FRAMES=45 build_pc\pcport_bootstrap.exe --field`
- `python tools\test_verify_gate.py` -> 12 passed, 0 failed.

## Batch 5C - hsd_mobj Material Runtime Holes

- [x] Land a generated-copy PC-port overlay for `fn_801A7128`.
- [x] Triage `fn_801A7128` with DeepSeek V4 Flash.
- [x] Host-bridge the TExp helper labels that blocked safe MObj work.
- [x] Restore the six-smoke real TObj/TExp parser verification gate.
- [x] Wire PC-port `hsdMObj.make_texp` and `hsdTObj.make_texp` to conservative host TExp builders.
- [x] Replace the generated PC body for `fn_801A7B24` so it calls the local `load` and `make_texp` method slots correctly.
- [x] Verify the live HSD character tree load path through `HSD_JObjLoadJoint -> HSD_DObjLoadDesc -> fn_801A7B24`.
- [x] Add a targeted real-content material-state delta probe.
- [x] Verify material state changes on real PC-port scene content.
- [x] Replace the generated PC body for `fn_801A6E24` so material setup calls the PC `make_texp` slot with the correct signature.
- [x] Replace the generated PC body for `fn_801A7E84` and route `HSD_MObjAnim` through it so material AObj keys update live MObj state.
- [x] Replace the generated PC body for `fn_801A7D58` so MObj copies allocate independent PC-side material, PE, and TObj-chain storage.
- [x] Retry hsd_jobj `fn_801A0744` / `fn_801A0D94` from the real Colosseum `.inc` sequence.

Active lane:

- Worktree: `C:\Users\douglaswhittingham\pkmn-colosseum`
- Branch: `master`
- Worker focus: `src\hsd\hsd_mobj.c`, PC-port `REPLACE_BODY` overlays for `fn_801A7128`, `fn_801A7B24`, `fn_801A6E24`, `fn_801A7E84`, and `fn_801A7D58`.

Lane result:

- `build_pc\bodies\hsd_mobj\fn_801A7128.c` now lands as a generated-copy-only `REPLACE_BODY` overlay.
- `src\hsd\hsd_mobj.c` now wires the PC-only `hsdMObj.make_texp` slot to `fn_801A7128`; generated `build_pc\gen\hsd\hsd_mobj.c` contains the replacement body and compiled `build_pc\obj\hsd_mobj.o` contains `_fn_801A7128`.
- The overlay follows the Colosseum `.inc` material/lightmap branch shape, uses the host TExp helper bridge for the known label calls, and routes TObj expression building through the local class method when available with a `PCPort_TObjMakeTExp` fallback.
- `pcport\pcport_main.c` now exposes `--real-material-delta-smoke`, which loads a real swizzled MObj descriptor through `HSD_MObjLoadDesc`, rebuilds TExp through the PC `make_texp` slot, calls `HSD_MObjSetAlpha(1.0f)` and `HSD_MObjSetAlpha(0.25f)`, and requires a framebuffer delta on real `menu_bg00` geometry through `fn_800DAD10`.
- `build_pc\bodies\hsd_mobj\fn_801A6E24.c` now replaces the generated material setup path so it builds the temporary TObj chain and calls `make_texp(mobj, tobj_top, &mobj->texp)` instead of casting the method slot to the old `(mobj, tobj, rendermode)` shape.
- `--real-material-delta-smoke` now directly calls `fn_801A6E24(liveMObj)` after live HSD loading and requires a non-null setup-built `mobj->texp`, so the setup overlay is included in the visible-impact gate.
- `build_pc\bodies\hsd_mobj\fn_801A7E84.c` now replaces the empty generated material-animation dispatcher. It follows the Colosseum `.inc` order: ambient RGB, diffuse RGB, inverted alpha, specular RGB, then PE ref0/ref1/dst alpha.
- `pcport\hsd_host.c` now provides a BOOT-order `HSD_MObjAnim` override so MObj AObj keys reach `fn_801A7E84` before the existing TObj animation walk.
- `--real-material-delta-smoke` now verifies the dispatcher on live real content by requiring type 4 to set diffuse red to 31 and type 7 to set alpha to 0.250 before restoring the material for the framebuffer delta check.
- `build_pc\bodies\hsd_mobj\fn_801A7D58.c` now replaces the generated MObj copy helper. The previous generated body called `fn_801BE4CC` as if it cloned TObj chains, but the current PC-linked symbol is an image-format helper; it also called the mis-typed `fn_80193B10` allocator path. The overlay allocates cloned TObj nodes through `HSD_TObjAlloc`, material storage through `HSD_MaterialAlloc`, PE storage through `HSD_MemAlloc`, preserves borrowed image pointers, and avoids aliasing the source MObj's animation object.
- `--real-material-delta-smoke` now calls `fn_801A7D58` against the live swizzled MObj and requires independent `copyMat` / `copyTObj` pointers plus the expected toon render bit before the alpha framebuffer delta check.
- Main checkout added BOOT-order host overrides in `pcport\hsd_host.c` for `fn_801B707C`, `fn_801B6F5C`, `fn_801B6E74`, `fn_801B6CD8`, `fn_801B64EC`, `fn_801B5F08`, `fn_801B5E40`, `fn_801B7C60`, and `fn_801B4300`.
- Main checkout fixed the existing parser gate failure by classifying narrow I8 ramp/mask stages before the generic direct-sample fallback in `pcport\real_content_host.c`.
- Follow-up integrated in main: `pcport\hsd_host.c` now provides conservative `PCPort_MObjMakeTExp` / `PCPort_TObjMakeTExp` builders, `src\hsd\hsd_mobj.c` and `src\hsd\hsd_tobj.c` wire those into the PC-only class init path, and `build_pc\bodies\hsd_mobj\fn_801A7B24.c` corrects the generated MObj loader's local method-slot mapping.
- Remaining MObj blocker: `fn_801A6B8C`'s full release path cannot be safely overlaid until archive-owned versus heap-owned MObj resource ownership is recovered; current live loads borrow material data from swizzled archive storage. `fn_801A6D08` already matches the class release/destroy dispatch shape.
- Next JObj action complete: direct `fn_801A0FBC` loading now routes to the active host loader in the PC generated copy, so `gs_material.c` callers no longer depend on the still-TODO `fn_801A1098` method path.

Verification:

- `python tools\pcport_gen.py --out-dir build_pc\gen src\hsd\hsd_mobj.c` -> generated `hsd_mobj.c` with 7 flipped wrappers and the `fn_801A7128` replacement body.
- `python tools\pcport_link.py` -> `compiled 129 objects; 0 failed to compile`, round 2 linked OK with 1709 stubs, rebuilt `build_pc\pcport_bootstrap.exe`.
- `build_pc\pcport_bootstrap.exe --real-material-delta-smoke` -> live `HSD_MObjLoadDesc`, `fn_801A7E84` material animation dispatch (`animDiffuseR=31`, `animAlpha=0.250`), `fn_801A6E24` setup, `fn_801A7D58` copy helper (`copyMat` / `copyTObj` non-null), and PC `make_texp` rebuilds produced TExp roots; `HSD_MObjSetAlpha(1.0 -> 0.25)` produced `diffPixels=307200` on real `menu_bg00` geometry.
- `build_pc\pcport_bootstrap.exe --real-scene-slice-2-smoke`
- `build_pc\pcport_bootstrap.exe --real-textured-scene-slice-smoke`
- `build_pc\pcport_bootstrap.exe --real-scene-slice-3-smoke`
- `build_pc\pcport_bootstrap.exe --real-scene-slice-4-smoke` -> passed with `kind0=2 stage00=1 stage01=2 stages0=2 kind1=1 stage10=1 stages1=1`.
- `build_pc\pcport_bootstrap.exe --real-tev-scene-slice-3-smoke`
- `build_pc\pcport_bootstrap.exe --real-tev-scene-slice-2-smoke`
- `build_pc\pcport_bootstrap.exe --gsgfx-visible-smoke`
- `PCPORT_MENU_FRAMES=45 build_pc\pcport_bootstrap.exe --field` -> loaded `D1_garage_1F`, 324 collision triangles, rendered 45 frames, exited cleanly.
- `PCPORT_CHARANIM_BANK_PROBE=12 build_pc\pcport_bootstrap.exe` -> loaded `field_common.fsys :: ken_b1`, set up 11 motions, and stepped live HSD animation trees successfully.
- `python tools\test_verify_gate.py` -> 12 passed, 0 failed.
- `git diff --check` -> clean.

## Batch 5D - hsd_cobj Camera Runtime Holes

- [x] Triage remaining CObj runtime gaps after JObj and MObj stabilize.
- [x] Land safe PC-port overlay for viewport rect unpacker `fn_80194400`.
- [ ] Verify camera behavior against headed field captures.

Active lane:

- Worktree: `C:\Users\douglaswhittingham\pkmn-colosseum-wt-cobj`
- Branch: `pcport-5d-cobj`
- Worker focus: `src\hsd\hsd_cobj.c`, small camera/runtime TODO wrappers with visible field impact.

Lane result:

- Integrated `build_pc\bodies\hsd_cobj\fn_80194400.c` via `tools\pcport_stub_tables.json` `replace_body`.
- `fn_801950D0` was triaged but not landed because its common path still depends on unresolved `fn_8019513C`.
- Verification: `python tools\pcport_gen.py --out-dir build_pc\gen src\hsd\hsd_cobj.c`, `python tools\pcport_link.py`, and headed `build_pc\pcport_bootstrap.exe --field` smoke passed.

## Batch 5E - Display/Render Pass Holes

- [x] Triage display pass wrappers in `src\hsd\hsd_displayfunc.c`.
- [ ] Attempt PC-port overlays for `fn_80198038`, `fn_801985E0`, or `fn_80198B20` only if the result is safe enough to compile and review.
- [ ] Re-run headed field capture after any display overlay lands.

Active lane:

- Worktree: `C:\Users\douglaswhittingham\pkmn-colosseum-wt-display`
- Branch: `pcport-5e-display`
- Worker focus: `src\hsd\hsd_displayfunc.c` first; `src\hsd\hsd_pobj_disp.c` only if a smaller safe target is obvious.

Lane result:

- `fn_80198038`, `fn_801985E0`, and `fn_80198B20` were triaged with DeepSeek V4 Flash.
- No overlay landed; all three responses ended `SAFE_OVERLAY: no`.
- Blocker: displayfunc wrappers need recovered constant/flag contracts before a safe PC-port replacement.
- Next display action: retry only `fn_80198B20` after naming `lbl_80478AC0`, `lbl_80478ACC`, `lbl_802746D0`, and the flag-controlled path, or pivot to a smaller `hsd_pobj_disp.c` helper.

Integration rules:

- Do not edit `*_fn_*.inc` truth files.
- Do not flip asm wrapper `#if` switches for PC-port-only progress.
- Prefer `build_pc\bodies\<tu>\<fn>.c` plus `tools\pcport_stub_tables.json` `replace_body` entries.
- Main checkout owns final integration, `python tools\pcport_link.py`, headed smoke, and commit/push.

## Later Critical Path

- [ ] `battle_scene.c`: 19 asm functions / about 6.1K instructions remain.
- [ ] Cross-cutting smalls: `effect_util fn_80132A38`, `gs_event_exec.c`, `gs_texture.c`, and `fn_800D3088`.
- [ ] Continue skipping host-irrelevant thread/task and GX-init no-op shims unless the PC port proves they are needed.
