    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r31, r3
    mr r26, r4
    beq @L_802032D0
    li r4, 0x0
    li r5, 0x122
    li r6, 0x0
    bl fn_8012640C
    mr r5, r26
    li r4, 0x4
    bl fn_8020E57C
    mr. r26, r3
    beq @L_802032D0
    bl fn_801FD0EC
    mr r29, r3
    mr r3, r26
    bl fn_8020E758
    rlwinm. r0, r29, 0, 16, 31
    beq @L_802032D0
    cmplwi r0, 0x165
    beq @L_802032D0
    cmplwi r0, 0xffff
    beq @L_802032D0
    mr r3, r31
    li r4, 0x0
    li r5, 0xf7
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_802032D0
    cmplwi r31, 0x0
    bne @L_8020322C
    li r28, 0x0
    b @L_802032B0
@L_8020322C:
    mr r3, r31
    li r4, 0x0
    li r5, 0x122
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    li r3, 0x0
    b @L_80203250
@L_8020324C:
    addi r3, r3, 0x1
@L_80203250:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8020324C
    li r28, 0x0
    mr r26, r28
    b @L_802032A4
@L_80203268:
    rlwinm r0, r26, 0, 24, 31
    mulli r0, r0, 0xc
    add r27, r30, r0
    mr r3, r27
    bl fn_8020E614
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802032A0
    mr r3, r27
    bl fn_801FD0EC
    rlwinm. r0, r3, 0, 16, 31
    beq @L_802032A0
    cmplwi r0, 0x165
    beq @L_802032A0
    addi r28, r28, 0x1
@L_802032A0:
    addi r26, r26, 0x1
@L_802032A4:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_80203268
@L_802032B0:
    rlwinm. r0, r28, 0, 24, 31
    bne @L_802032D0
    mr r3, r31
    rlwinm r7, r29, 0, 16, 31
    li r4, 0x0
    li r5, 0xf7
    li r6, 0x0
    bl fn_801254B4
@L_802032D0:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
