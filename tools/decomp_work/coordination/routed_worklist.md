# Routed near-miss worklist (why_diff) — 2026-06-17

Classified top 30 of `_winnable_queue.md` via why_diff first-divergence.


## permuter  (7)

| match | fn | file | cat | note |
|---|---|---|---|---|
| 98.6 | fn_8011A280 | gs_field_world.c | REG-RENAME | first reg divergence @ aligned insn 17: target `mr      r27,r3`  vs  o |
| 98.6 | fn_8011A570 | gs_field_world.c | REG-RENAME | first reg divergence @ aligned insn 17: target `mr      r27,r3`  vs  o |
| 98.6 | fn_8011A9EC | gs_field_world.c | REG-RENAME | first reg divergence @ aligned insn 17: target `mr      r27,r3`  vs  o |
| 98.6 | fn_8011AB50 | gs_field_world.c | REG-RENAME | first reg divergence @ aligned insn 17: target `mr      r27,r3`  vs  o |
| 98.6 | fn_8011AFCC | gs_field_world.c | REG-RENAME | first reg divergence @ aligned insn 17: target `mr      r27,r3`  vs  o |
| 98.21 | fn_800330B8 | gs_npc_event.c | SCHEDULE-SWAP | instruction order differs (same instructions reordered) @ target 6..6  |
| 95.0 | fn_80068738 | ui_core.c | REG-RENAME | first reg divergence @ aligned insn 8: target `addi    r31,r3,lbl_803A |

## worker-ex  (13)

| match | fn | file | cat | note |
|---|---|---|---|---|
| 98.75 | fn_80024DBC | gs_title.c | DIFFERENT-INSN | different instruction(s) @ target 5..6 / ours 5..6 |
| 98.75 | fn_80024F2C | gs_title.c | DIFFERENT-INSN | different instruction(s) @ target 5..6 / ours 5..6 |
| 98.75 | fn_8002509C | gs_title.c | DIFFERENT-INSN | different instruction(s) @ target 5..6 / ours 5..6 |
| 98.75 | fn_8002520C | gs_title.c | DIFFERENT-INSN | different instruction(s) @ target 5..6 / ours 5..6 |
| 98.66 | fn_8004B598 | scene_init.c | DIFFERENT-INSN | different instruction(s) @ target 0..0 / ours 0..1 |
| 98.45 | fn_80211A78 | colosseum_event.c | DIFFERENT-INSN | different instruction(s) @ target 41..42 / ours 41..42 |
| 98.41 | fn_8002537C | gs_title.c | DIFFERENT-INSN | different instruction(s) @ target 8..9 / ours 8..9 |
| 98.41 | fn_80025490 | gs_title.c | DIFFERENT-INSN | different instruction(s) @ target 8..9 / ours 8..9 |
| 98.33 | fn_8000C824 | gs_party_access.c | DIFFERENT-INSN | different instruction(s) @ target 7..8 / ours 7..8 |
| 98.33 | fn_8000C92C | gs_party_access.c | DIFFERENT-INSN | different instruction(s) @ target 7..8 / ours 7..8 |
| 98.33 | fn_800218BC | gs_title.c | DIFFERENT-INSN | different instruction(s) @ target 35..35 / ours 35..36 |
| 98.32 | fn_8021C308 | colosseum_script.c | DIFFERENT-INSN | different instruction(s) @ target 6..7 / ours 6..7 |
| 95.23 | fn_80057DE8 | scene_init.c | DIFFERENT-INSN | different instruction(s) @ target 6..8 / ours 6..7 |

## worker-fr  (2)

| match | fn | file | cat | note |
|---|---|---|---|---|
| 99.6 | fn_80128E38 | gs_field_world.c | FRAME-SIZE | frame mismatch: target 0x30 vs ours 0x40 (ours larger by 0x10) |
| 98.54 | fn_80008868 | gs_task.c | FRAME-SIZE | frame mismatch: target 0x30 vs ours 0x60 (ours larger by 0x30) |

## stub  (7)

| match | fn | file | cat | note |
|---|---|---|---|---|
| 99.9 | fn_80040308 | scene_init.c | FRAME-SIZE | frame mismatch: target 0xE0 vs ours 0x0 (target larger by 0xE0) |
| 99.68 | fn_8004B7EC | scene_init.c | FRAME-SIZE | frame mismatch: target 0x30 vs ours 0x0 (target larger by 0x30) |
| 99.65 | fn_800DE128 | gs_render.c | FRAME-SIZE | frame mismatch: target 0x40 vs ours 0x0 (target larger by 0x40) |
| 98.8 | fn_800109A0 | gs_npc_interact.c | FRAME-SIZE | frame mismatch: target 0x20 vs ours 0x0 (target larger by 0x20) |
| 98.24 | fn_8000ED34 | gs_npc_interact.c | FRAME-SIZE | frame mismatch: target 0x40 vs ours 0x0 (target larger by 0x40) |
| 98.11 | fn_8000F964 | gs_npc_interact.c | FRAME-SIZE | frame mismatch: target 0x40 vs ours 0x0 (target larger by 0x40) |
| 95.24 | fn_800E0790 | gs_render.c | FRAME-SIZE | frame mismatch: target 0x10 vs ours 0x0 (target larger by 0x10) |

## reloc-wall  (1)

| match | fn | file | cat | note |
|---|---|---|---|---|
| 97.5 | fn_800F670C | gs_thread.c | MATCH | no divergence found -- traces are identical |