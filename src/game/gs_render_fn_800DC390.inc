    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r4
    stw r28, 0x10(r1)
    mr r28, r3
    bl fn_800EF4DC
    cmpwi r3, 0x90
    beq @L_800DC3E0
    bge @L_800DC3D8
    cmpwi r3, 0x46
    bge @L_800DC3D8
    cmpwi r3, 0x40
    bge @L_800DC3E0
    @L_800DC3D8:
    li r3, 0x0
    b @L_800DC520
    @L_800DC3E0:
    lis r3, lbl_80400EE0@ha
    lbzu r0, lbl_80400EE0@l(r3)
    cmplwi r0, 0x1
    bne @L_800DC400
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_800DC400
    b @L_800DC458
    @L_800DC400:
    lbzu r0, 0x14(r3)
    cmplwi r0, 0x1
    bne @L_800DC41C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_800DC41C
    b @L_800DC458
    @L_800DC41C:
    lbzu r0, 0x14(r3)
    cmplwi r0, 0x1
    bne @L_800DC438
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_800DC438
    b @L_800DC458
    @L_800DC438:
    lbzu r0, 0x14(r3)
    cmplwi r0, 0x1
    bne @L_800DC454
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_800DC454
    b @L_800DC458
    @L_800DC454:
    li r3, 0x0
    @L_800DC458:
    cmplwi r3, 0x0
    beq @L_800DC468
    li r3, 0x0
    b @L_800DC520
    @L_800DC468:
    lis r3, lbl_80400EE0@ha
    li r4, 0x0
    addi r31, r3, lbl_80400EE0@l
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_800DC484
    b @L_800DC4B8
    @L_800DC484:
    lbzu r0, 0x14(r31)
    cmplwi r0, 0x0
    bne @L_800DC494
    b @L_800DC4B8
    @L_800DC494:
    lbzu r0, 0x14(r31)
    cmplwi r0, 0x0
    bne @L_800DC4A4
    b @L_800DC4B8
    @L_800DC4A4:
    lbzu r0, 0x14(r31)
    cmplwi r0, 0x0
    bne @L_800DC4B4
    b @L_800DC4B8
    @L_800DC4B4:
    mr r31, r4
    @L_800DC4B8:
    cmplwi r31, 0x0
    bne @L_800DC4C8
    li r3, 0x0
    b @L_800DC520
    @L_800DC4C8:
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x0(r31)
    li r4, 0x0
    li r5, 0x0
    stw r28, 0x4(r31)
    stw r29, 0x8(r31)
    stw r30, 0xc(r31)
    stw r0, 0x10(r31)
    lwz r3, 0x4(r31)
    bl fn_800EF590
    lwz r3, 0x4(r31)
    li r4, 0x2
    li r5, 0x2
    li r6, 0x0
    bl fn_800EF578
    lwz r3, 0x4(r31)
    li r4, 0x0
    bl fn_800EF548
    li r0, 0x1
    li r3, 0x1
    stb r0, lbl_8047AAE0(r13)
    @L_800DC520:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
