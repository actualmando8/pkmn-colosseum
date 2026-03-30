    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r27, r3
    mr r28, r5
    bne @L_8020E5A0
    li r3, 0x0
    b @L_8020E600
@L_8020E5A0:
    clrlwi r31, r4, 16
    li r29, 0x0
    b @L_8020E5F0
@L_8020E5AC:
    clrlwi r0, r29, 16
    mulli r0, r0, 0xc
    add r30, r27, r0
    mr r3, r30
    bl fn_801FD104
    neg r3, r3
    subic r0, r3, 0x1
    subfe r0, r0, r3
    clrlwi. r0, r0, 24
    beq @L_8020E5EC
    mr r3, r30
    bl fn_801FD104
    cmplw r3, r28
    bne @L_8020E5EC
    mr r3, r30
    b @L_8020E600
@L_8020E5EC:
    addi r29, r29, 0x1
@L_8020E5F0:
    clrlwi r0, r29, 16
    cmplw r0, r31
    blt @L_8020E5AC
    li r3, 0x0
@L_8020E600:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
