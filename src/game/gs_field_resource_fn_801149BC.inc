    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r30, r4
    mr r29, r5
    li r28, 0x0
    bl fn_800F9318
    mr r31, r3
    mr r5, r29
    addi r4, r31, 0x60
    bl fn_80191F64
    lis r4, -32729
    mr r3, r31
    addi r4, r4, 8876
    bl fn_80191ECC
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_80114A14
    li r3, 0x0
    b @L_80114A5C
@L_80114A14:
    lwz r0, 0x0(r31)
    rlwinm r3, r30, 0, 1, 15
    ori r30, r3, 0x1000
    cmplwi r0, 0x0
    beq @L_80114A58
    li r29, 0x0
    b @L_80114A48
@L_80114A30:
    mr r4, r27
    or r5, r30, r28
    li r6, 0x0
    bl fn_800F9378
    addi r29, r29, 0x4
    addi r28, r28, 0x1
@L_80114A48:
    lwz r3, 0x0(r31)
    lwzx r3, r3, r29
    cmplwi r3, 0x0
    bne @L_80114A30
@L_80114A58:
    mr r3, r31
@L_80114A5C:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
