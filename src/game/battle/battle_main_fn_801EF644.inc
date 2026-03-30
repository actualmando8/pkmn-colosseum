    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r3
    li r30, 0x0
    extsb r31, r29
    b @L_801EF6D8
@L_801EF664:
    extsb. r0, r29
    blt @L_801EF688
    lis r3, lbl_803752A0@ha
    clrlslwi r4, r30, 16, 1
    addi r0, r3, lbl_803752A0@l
    add r3, r0, r4
    lhz r0, 0x2(r3)
    cmpw r31, r0
    bne @L_801EF6D4
@L_801EF688:
    lis r4, lbl_803752A0@ha
    lis r3, lbl_803727C8@ha
    clrlslwi r0, r30, 16, 1
    addi r4, r4, lbl_803752A0@l
    addi r3, r3, lbl_803727C8@l
    lhzx r0, r4, r0
    mulli r0, r0, 0xc
    lwzx r28, r3, r0
    mr r3, r28
    bl fn_8017D56C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801EF6D4
@L_801EF6B8:
    mr r3, r28
    bl fn_8017B2CC
    cmpwi r3, 0x0
    beq @L_801EF6D4
    blt @L_801EF6D4
    bl fn_800F0308
    b @L_801EF6B8
@L_801EF6D4:
    addi r30, r30, 0x2
@L_801EF6D8:
    lhz r0, -31504(r13)
    rlwinm r3, r30, 0, 16, 31
    cmplw r3, r0
    blt @L_801EF664
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr