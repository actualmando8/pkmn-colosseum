    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stmw r26, 0x18(r1)
    mr r27, r3
    addi r28, r4, 0x0
    addi r29, r5, 0x0
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_800B4368
    li r3, -0x3
    b @L_800B4474
    @L_800B4368:
    mr r3, r27
    bl fn_800B1E0C
    lis r4, lbl_803FC840@ha
    addi r31, r3, 0x0
    addi r26, r4, lbl_803FC840@l
    li r30, 0x0
    @L_800B4380:
    lbz r0, 0x0(r31)
    cmplwi r0, 0xff
    bne @L_800B4394
    li r0, -0x4
    b @L_800B43DC
    @L_800B4394:
    lwz r4, 0x10c(r27)
    cmplw r4, r26
    beq @L_800B43D0
    addi r3, r31, 0x0
    li r5, 0x4
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B43D8
    lwz r4, 0x10c(r27)
    addi r3, r31, 0x4
    li r5, 0x2
    addi r4, r4, 0x4
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B43D8
    @L_800B43D0:
    li r0, 0x0
    b @L_800B43DC
    @L_800B43D8:
    li r0, -0xa
    @L_800B43DC:
    cmpwi r0, 0x0
    blt @L_800B4460
    addi r7, r28, 0x0
    addi r6, r31, 0x8
    li r4, 0x20
    b @L_800B442C
    @L_800B43F4:
    lbz r0, 0x0(r6)
    addi r6, r6, 0x1
    lbz r5, 0x0(r7)
    addi r7, r7, 0x1
    extsb r3, r0
    extsb r0, r5
    cmpw r3, r0
    beq @L_800B441C
    li r0, 0x0
    b @L_800B444C
    @L_800B441C:
    extsb. r0, r5
    bne @L_800B442C
    li r0, 0x1
    b @L_800B444C
    @L_800B442C:
    subic. r4, r4, 0x1
    bge @L_800B43F4
    lbz r0, 0x0(r7)
    extsb. r0, r0
    bne @L_800B4448
    li r0, 0x1
    b @L_800B444C
    @L_800B4448:
    li r0, 0x0
    @L_800B444C:
    cmpwi r0, 0x0
    beq @L_800B4460
    stw r30, 0x0(r29)
    li r3, 0x0
    b @L_800B4474
    @L_800B4460:
    addi r30, r30, 0x1
    cmpwi r30, 0x7f
    addi r31, r31, 0x40
    blt @L_800B4380
    li r3, -0x4
    @L_800B4474:
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
