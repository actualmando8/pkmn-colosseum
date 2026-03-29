    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    lwz r4, lbl_8047A590(r13)
    lis r3, lbl_803A9A08@ha
    addi r30, r3, lbl_803A9A08@l
    li r3, 0x1
    li r0, 0x0
    cmplwi r4, 0x0
    addi r28, r30, 0x8
    addi r29, r30, 0x4
    stw r3, 0x0(r30)
    li r26, 0x1
    stw r0, 0x0(r28)
    stw r0, 0x0(r29)
    stw r0, 0xc(r30)
    beq @L_80057F54
    mr r27, r4
    b @L_80057F3C
    @L_80057EC0:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    bne @L_80057EE0
    li r3, 0x48a
    bl fn_8017B07C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80057F38
    @L_80057EE0:
    lwz r4, 0x0(r27)
    li r3, 0x48a
    bl fn_8017B13C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80057F38
    li r26, 0x0
    b @L_80057F04
    @L_80057F00:
    bl fn_800F0308
    @L_80057F04:
    li r3, 0x48a
    bl fn_8017B2CC
    cmpwi r3, 0x1
    beq @L_80057F00
    li r31, 0x1
    b @L_80057F24
    @L_80057F1C:
    stw r31, 0x0(r29)
    bl fn_800F0308
    @L_80057F24:
    lwz r0, 0x0(r28)
    cmpwi r0, 0x0
    bne @L_80057F1C
    li r0, 0x0
    stw r0, 0x0(r29)
    @L_80057F38:
    addi r27, r27, 0x4
    @L_80057F3C:
    lwz r4, 0x0(r27)
    cmplwi r4, 0x0
    beq @L_80057F54
    lwz r0, 0xc(r30)
    cmpwi r0, 0x0
    beq @L_80057EC0
    @L_80057F54:
    lwz r0, 0xc(r30)
    lis r3, lbl_803A9A08@ha
    addi r3, r3, lbl_803A9A08@l
    li r4, 0x0
    cmpwi r0, 0x0
    stw r4, 0x0(r3)
    beq @L_80057F80
    li r3, 0x48a
    bl fn_8017B1CC
    li r3, 0x48a
    bl fn_800F915C
    @L_80057F80:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
