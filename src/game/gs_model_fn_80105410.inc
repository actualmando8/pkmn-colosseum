    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lis r3, lbl_80404ACC@ha
    li r4, 0x0
    addi r3, r3, lbl_80404ACC@l
    li r5, 0x9c
    bl memset
    rlwinm r0, r30, 0, 16, 31
    mulli r31, r0, 0xb4
    mr r3, r31
    bl fn_800E3534
    rlwinm r0, r3, 0, 16, 31
    lis r4, lbl_80404ACC@ha
    addi r4, r4, lbl_80404ACC@l
    cmplwi r0, 0x0
    sth r3, 0x2(r4)
    bne @L_80105478
    lis r3, lbl_80271EC4@ha
    addi r3, r3, lbl_80271EC4@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801054A0
@L_80105478:
    mr r3, r0
    bl fn_800E27B0
    lis r6, lbl_80404ACC@ha
    mr r5, r31
    addi r6, r6, lbl_80404ACC@l
    li r4, 0x0
    stw r3, 0x8(r6)
    sth r30, 0x0(r6)
    bl memset
    bl fn_80109358
@L_801054A0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
