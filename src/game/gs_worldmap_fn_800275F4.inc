    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr r22, r3
    lwz r4, 0x60(r22)
    lis r3, lbl_80266E18@ha
    addi r0, r3, lbl_80266E18@l
    addi r26, r1, 0x8
    lwz r3, 0x24(r4)
    li r24, 0x0
    li r28, 0x0
    lwz r23, 0x0(r3)
    mulli r3, r23, 0x18
    add r27, r0, r3
    @L_80027630:
    li r31, 0x0
    addi r29, r27, 0x8
    mr r30, r31
    li r25, 0x0
    b @L_800276A8
    @L_80027644:
    lbz r5, 0x8b(r22)
    li r0, 0x0
    li r3, -0x100
    mr r4, r26
    sth r6, 0x8(r1)
    or r21, r5, r3
    li r3, 0x37
    sth r0, 0xa(r1)
    bl fn_80132A38
    li r3, 0xce
    bl fn_800FA444
    srwi r0, r3, 16
    mr r4, r28
    extsh r0, r0
    mr r5, r21
    subfic r3, r0, 0x1b
    li r6, 0xce
    srwi r0, r3, 31
    add r0, r0, r3
    srawi r0, r0, 1
    add r3, r31, r0
    bl fn_800FB680
    addi r31, r31, 0x1b
    addi r30, r30, 0x2
    addi r25, r25, 0x1
    @L_800276A8:
    cmpwi r23, 0x0
    blt @L_800276B8
    cmpwi r23, 0x2
    blt @L_800276C0
    @L_800276B8:
    li r6, 0x0
    b @L_80027708
    @L_800276C0:
    cmpwi r24, 0x0
    blt @L_800276D0
    cmpwi r24, 0x4
    blt @L_800276D8
    @L_800276D0:
    li r6, 0x0
    b @L_80027708
    @L_800276D8:
    lwz r21, 0x0(r29)
    mr r3, r21
    bl fn_800FA314
    cmpwi r25, 0x0
    blt @L_800276F4
    cmpw r25, r3
    blt @L_800276FC
    @L_800276F4:
    li r6, 0x0
    b @L_80027708
    @L_800276FC:
    mr r3, r21
    bl fn_800FA280
    lhzx r6, r3, r30
    @L_80027708:
    clrlwi r0, r6, 16
    cmplwi r0, 0x0
    bne @L_80027644
    addi r28, r28, 0x23
    addi r27, r27, 0x4
    addi r24, r24, 0x1
    cmpwi r24, 0x4
    blt @L_80027630
    li r3, 0x0
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
