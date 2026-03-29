    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lis r4, lbl_80266B58@ha
    lwz r3, 0x60(r3)
    addi r4, r4, lbl_80266B58@l
    li r5, 0x0
    lwz r3, 0x4(r3)
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8001440C
    addi r4, r4, 0xc
    li r5, 0x1
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8001440C
    addi r4, r4, 0xc
    li r5, 0x2
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8001440C
    addi r4, r4, 0xc
    li r5, 0x3
    lwz r0, 0x0(r4)
    cmpw r3, r0
    beq @L_8001440C
    li r5, 0x4
    @L_8001440C:
    cmpwi r5, 0x4
    blt @L_8001441C
    li r3, 0x0
    b @L_8001453C
    @L_8001441C:
    mulli r4, r5, 0xc
    lis r3, lbl_80266B58@ha
    li r7, 0x0
    addi r0, r3, lbl_80266B58@l
    add r3, r0, r4
    lwz r6, 0x4(r3)
    lwz r5, 0x8(r3)
    mtctr r5
    cmpwi r5, 0x0
    ble @L_80014460
    @L_80014444:
    lha r3, 0x6(r31)
    lwz r0, 0x0(r6)
    cmpw r3, r0
    beq @L_80014460
    addi r7, r7, 0x1
    addi r6, r6, 0xc
    bdnz @L_80014444
    @L_80014460:
    lis r3, lbl_80266B58@ha
    addi r0, r3, lbl_80266B58@l
    add r3, r0, r4
    lwz r0, 0x8(r3)
    cmpw r7, r0
    blt @L_80014480
    li r3, 0x0
    b @L_8001453C
    @L_80014480:
    subf r3, r7, r0
    li r8, 0x1
    subi r0, r3, 0x1
    li r6, 0x0
    cmpwi r0, 0x0
    ble @L_800144EC
    cmpwi r0, 0x8
    subi r4, r3, 0x9
    ble @L_800144CC
    addi r0, r4, 0x7
    lis r3, 0x5f6
    srwi r0, r0, 3
    subi r3, r3, 0x1f00
    mtctr r0
    cmpwi r4, 0x0
    ble @L_800144CC
    @L_800144C0:
    mullw r8, r8, r3
    addi r6, r6, 0x8
    bdnz @L_800144C0
    @L_800144CC:
    subf r3, r7, r5
    subi r3, r3, 0x1
    subf r0, r6, r3
    mtctr r0
    cmpw r6, r3
    bge @L_800144EC
    @L_800144E4:
    mulli r8, r8, 0xa
    bdnz @L_800144E4
    @L_800144EC:
    lwz r4, lbl_8047A2FC(r13)
    lis r3, 0x6666
    addi r0, r3, 0x6667
    divw r4, r4, r8
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
    li r7, -0x1
    li r8, 0xc9
    bl fn_800FB8C8
    li r3, 0x0
    @L_8001453C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
