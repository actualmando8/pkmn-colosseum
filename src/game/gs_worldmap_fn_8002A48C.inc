    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lis r4, lbl_80266E58@ha
    lwz r6, 0x60(r3)
    addi r4, r4, lbl_80266E58@l
    li r5, 0x0
    lha r3, 0x6(r31)
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8002A4D8
    li r5, 0x1
    addi r4, r4, 0xc
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8002A4D8
    li r5, 0x2
    @L_8002A4D8:
    cmpwi r5, 0x2
    blt @L_8002A4E8
    li r3, 0x0
    b @L_8002A59C
    @L_8002A4E8:
    subfic r5, r5, 0x1
    li r8, 0x1
    cmpwi r5, 0x0
    li r7, 0x0
    ble @L_8002A548
    cmpwi r5, 0x8
    subi r4, r5, 0x8
    ble @L_8002A530
    addi r0, r4, 0x7
    lis r3, 0x5f6
    srwi r0, r0, 3
    subi r3, r3, 0x1f00
    mtctr r0
    cmpwi r4, 0x0
    ble @L_8002A530
    @L_8002A524:
    mullw r8, r8, r3
    addi r7, r7, 0x8
    bdnz @L_8002A524
    @L_8002A530:
    subf r0, r7, r5
    mtctr r0
    cmpw r7, r5
    bge @L_8002A548
    @L_8002A540:
    mulli r8, r8, 0xa
    bdnz @L_8002A540
    @L_8002A548:
    lwz r4, 0xc(r6)
    lis r3, 0x6666
    addi r0, r3, 0x6667
    lwz r4, 0x0(r4)
    li r3, 0x34
    divw r4, r4, r8
    mulhw r0, r0, r4
    srawi r0, r0, 2
    srwi r5, r0, 31
    add r0, r0, r5
    mulli r0, r0, 0xa
    subf r4, r0, r4
    bl fn_80132A38
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    li r4, 0x0
    li r7, -0x1
    li r8, 0xc9
    bl fn_800FB8C8
    li r3, 0x0
    @L_8002A59C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
