    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x34(r1)
    li r5, 0x16
    li r6, 0x0
    stmw r25, 0x14(r1)
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_8020B098
    li r3, 0x1
    b @L_8020B31C
@L_8020B098:
    li r3, 0x0
    bl fn_801F2A7C
    mr. r31, r3
    beq @L_8020B268
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    mr. r30, r3
    beq @L_8020B268
    mr r3, r31
    li r4, 0x0
    bl fn_801F86C0
    bl fn_801EF634
    mr r4, r3
    li r3, 0x0
    bl fn_801F1DBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B22C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B1F0
    mr r3, r31
    bl fn_801F9034
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B1F0
    li r26, 0x0
    b @L_8020B1E4
@L_8020B124:
    mr r3, r30
    rlwinm r5, r26, 0, 16, 31
    li r4, 0x3
    bl fn_8012A5B0
    mr r27, r3
    bl fn_801233F4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020B1E0
    mr r3, r27
    bl fn_8011FC74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8020B1E0
    mr r3, r31
    mr r4, r27
    bl fn_801F9930
    mr. r25, r3
    beq @L_8020B1E0
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020B1E0
    mr r3, r25
    li r4, 0x0
    li r5, 0xd0
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020B1E0
    mr r3, r27
    addi r6, r1, 0x8
    addi r7, r1, 0xc
    li r4, 0x0
    li r5, 0x0
    bl fn_80128A64
    rlwinm. r0, r3, 0, 16, 31
    mr r4, r3
    beq @L_8020B1E0
    lhz r5, 0x8(r1)
    mr r3, r27
    mr r7, r30
    addi r6, r1, 0xc
    li r8, 0x1
    li r9, 0x1
    li r10, 0x0
    bl fn_8012805C
    li r3, 0xa
    bl fn_801EFFC4
@L_8020B1E0:
    addi r26, r26, 0x1
@L_8020B1E4:
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_8020B124
@L_8020B1F0:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B218
    mr r3, r30
    bl fn_80129840
@L_8020B218:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x28
    li r6, 0x0
    bl fn_801F54A4
@L_8020B22C:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B268
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_8020B268
    mr r4, r30
    bl fn_8012AC64
@L_8020B268:
    bl fn_8006B57C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020B318
    li r26, 0x0
    b @L_8020B30C
@L_8020B280:
    mr r4, r26
    li r3, 0x0
    bl fn_801F47B4
    mr. r25, r3
    beq @L_8020B308
    rlwinm r0, r26, 0, 16, 31
    li r27, 0x0
    mullw r30, r0, r29
    b @L_8020B2FC
@L_8020B2A4:
    mr r3, r25
    mr r4, r27
    bl fn_801F7258
    mr. r31, r3
    beq @L_8020B2F8
    li r4, 0x0
    bl fn_801F86C0
    add r0, r27, r30
    rlwinm r3, r0, 0, 24, 31
    bl fn_8006B0F8
    mr. r28, r3
    beq @L_8020B2F8
    mr r3, r31
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    mr. r4, r3
    beq @L_8020B2F8
    mr r3, r28
    bl fn_8012AC64
@L_8020B2F8:
    addi r27, r27, 0x1
@L_8020B2FC:
    rlwinm r0, r27, 0, 16, 31
    cmplw r0, r29
    blt @L_8020B2A4
@L_8020B308:
    addi r26, r26, 0x1
@L_8020B30C:
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_8020B280
@L_8020B318:
    li r3, 0x1
@L_8020B31C:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
