    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stfd f30, 0x40(r1)
    psq_st f30, 0x48(r1), 0, 0
    stmw r25, 0x24(r1)
    mr r30, r4
    mr r29, r3
    mr r26, r30
    li r31, 0x0
    lwz r0, 0x68(r30)
    cmpwi r0, 0x2
    beq @L_801DC4C8
    bge @L_801DC4D4
    cmpwi r0, 0x1
    bge @L_801DC4B8
    b @L_801DC4D4
@L_801DC4B8:
    li r0, 0x0
    li r31, -0x4
    stw r0, 0x18(r29)
    b @L_801DC4D4
@L_801DC4C8:
    lwz r0, 0x6c(r26)
    li r31, 0x0
    stw r0, 0x18(r29)
@L_801DC4D4:
    lwz r0, 0x0(r26)
    addi r3, r29, 0x2c
    li r4, 0x0
    li r5, 0x40
    stw r0, 0x0(r29)
    lwz r0, 0x4(r26)
    stw r0, 0x4(r29)
    lwz r0, 0x10(r26)
    stw r0, 0x8(r29)
    lwz r0, 0x14(r26)
    stw r0, 0xc(r29)
    lwz r0, 0x18(r26)
    stw r0, 0x10(r29)
    lwz r0, 0x1c(r26)
    stw r0, 0x14(r29)
    lwz r0, 0x60(r26)
    stw r0, 0x1c(r29)
    lwz r0, 0x64(r26)
    stw r0, 0x24(r29)
    lwz r0, 0x8(r26)
    stw r0, 0x20(r29)
    lwz r0, 0xc(r26)
    stw r0, 0x28(r29)
    bl memset
    lfd f30, -21240(r2)
    mr r27, r26
    lfs f31, -21248(r2)
    mr r26, r29
    li r25, 0x0
    lis r28, 0x4330
@L_801DC54C:
    bl fn_800D37CC
    lwz r0, 0x20(r27)
    xoris r3, r3, 0x8000
    stw r3, 0xc(r1)
    addi r25, r25, 0x1
    xoris r0, r0, 0x8000
    addi r27, r27, 0x4
    stw r28, 0x8(r1)
    cmpwi r25, 0x10
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f30
    stw r28, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f30
    fmuls f0, f0, f1
    fdivs f0, f0, f31
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    stw r0, 0x2c(r26)
    addi r26, r26, 0x4
    blt @L_801DC54C
    li r0, 0x0
    add r30, r31, r30
    stw r0, 0xa8(r29)
    addi r30, r30, 0x70
    mr r3, r30
    stw r0, 0xac(r29)
    stw r0, 0x74(r29)
    stw r0, 0x70(r29)
    stw r0, 0x6c(r29)
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    psq_l f30, 0x48(r1), 0, 0
    lfd f30, 0x40(r1)
    lmw r25, 0x24(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
