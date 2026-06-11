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

- [ ] Decompile or host-bridge the live-impact instance/reference helpers `fn_801A0744` and `fn_801A0D94`.
- [ ] Triage remaining `hsd_jobj` TODO wrappers from `tools/decomp_work/_interesting_reordered.json`.
- [ ] Verify that new JObj work changes linked PC-port runtime behavior.

## Batch 5C - hsd_mobj Material Runtime Holes

- [ ] Attack `fn_801A7128` and nearby alpha/material setter holes.
- [ ] Verify material state changes on real PC-port scene content.

## Batch 5D - hsd_cobj Camera Runtime Holes

- [ ] Triage remaining CObj runtime gaps after JObj and MObj stabilize.
- [ ] Verify camera behavior against headed field captures.

## Later Critical Path

- [ ] `battle_scene.c`: 19 asm functions / about 6.1K instructions remain.
- [ ] Cross-cutting smalls: `effect_util fn_80132A38`, `gs_event_exec.c`, `gs_texture.c`, and `fn_800D3088`.
- [ ] Continue skipping host-irrelevant thread/task and GX-init no-op shims unless the PC port proves they are needed.
