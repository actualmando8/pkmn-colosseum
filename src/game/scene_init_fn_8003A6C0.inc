    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    lis r3, lbl_80267130@ha
    li r6, 0x0
    addi r3, r3, lbl_80267130@l
    lwz r5, 0x0(r3)
    lwz r4, 0x4(r3)
    lwz r3, 0x8(r3)
    lha r0, 0x6(r31)
    cmpw r0, r5
    beq @L_8003A71C
    li r6, 0x1
    cmpw r0, r4
    beq @L_8003A71C
    li r6, 0x2
    cmpw r0, r3
    beq @L_8003A71C
    li r6, 0x3
    @L_8003A71C:
    cmpwi r6, 0x3
    blt @L_8003A72C
    li r3, 0x0
    b @L_8003A7D8
    @L_8003A72C:
    cmpwi r6, 0x0
    li r7, 0x1
    li r5, 0x0
    ble @L_8003A788
    cmpwi r6, 0x8
    subi r4, r6, 0x8
    ble @L_8003A770
    addi r0, r4, 0x7
    lis r3, 0x5f6
    srwi r0, r0, 3
    subi r3, r3, 0x1f00
    mtctr r0
    cmpwi r4, 0x0
    ble @L_8003A770
    @L_8003A764:
    mullw r7, r7, r3
    addi r5, r5, 0x8
    bdnz @L_8003A764
    @L_8003A770:
    subf r0, r5, r6
    mtctr r0
    cmpw r5, r6
    bge @L_8003A788
    @L_8003A780:
    mulli r7, r7, 0xa
    bdnz @L_8003A780
    @L_8003A788:
    lwz r4, lbl_8047A4C8(r13)
    lis r3, 0x6666
    addi r0, r3, 0x6667
    divw r4, r4, r7
    li r3, 0x34
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
    lwz r7, 0x88(r30)
    li r8, 0xc9
    bl fn_800FB8C8
    li r3, 0x0
    @L_8003A7D8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
