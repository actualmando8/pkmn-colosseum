    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_80401BF8@ha
    li r5, 0x14
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    addi r3, r4, lbl_80401BF8@l
    li r4, 0x0
    stw r30, 0x8(r1)
    bl memset
    clrlwi r30, r31, 16
    lis r3, lbl_80401BF8@ha
    addi r0, r3, lbl_80401BF8@l
    mulli r3, r30, 0x16c
    stw r0, lbl_80478B00(r13)
    bl fn_800E3534
    lwz r4, lbl_80478B00(r13)
    sth r3, 0x2(r4)
    lwz r3, lbl_80478B00(r13)
    lhz r3, 0x2(r3)
    cmplwi r3, 0x0
    bne @L_800F77CC
    lis r3, lbl_802712E4@ha
    addi r3, r3, lbl_802712E4@l
    crxor 6, 6, 6
    bl fn_800DD38C
    li r3, -0x1
    b @L_800F788C
    @L_800F77CC:
    bl fn_800E27B0
    lwz r4, lbl_80478B00(r13)
    cmpwi r30, 0x0
    li r5, 0x0
    stw r3, 0xc(r4)
    lwz r3, lbl_80478B00(r13)
    lwz r4, 0xc(r3)
    ble @L_800F7880
    cmpwi r30, 0x8
    subi r3, r30, 0x8
    ble @L_800F785C
    addi r0, r3, 0x7
    srwi r0, r0, 3
    mtctr r0
    cmpwi r3, 0x0
    ble @L_800F785C
    li r0, 0x0
    @L_800F7810:
    stw r0, 0x0(r4)
    addi r5, r5, 0x8
    stb r0, 0x4(r4)
    stw r0, 0x16c(r4)
    stb r0, 0x170(r4)
    stw r0, 0x2d8(r4)
    stb r0, 0x2dc(r4)
    stw r0, 0x444(r4)
    stb r0, 0x448(r4)
    stw r0, 0x5b0(r4)
    stb r0, 0x5b4(r4)
    stw r0, 0x71c(r4)
    stb r0, 0x720(r4)
    stw r0, 0x888(r4)
    stb r0, 0x88c(r4)
    stw r0, 0x9f4(r4)
    stb r0, 0x9f8(r4)
    addi r4, r4, 0xb60
    bdnz @L_800F7810
    @L_800F785C:
    subf r0, r5, r30
    li r3, 0x0
    mtctr r0
    cmpw r5, r30
    bge @L_800F7880
    @L_800F7870:
    stw r3, 0x0(r4)
    stb r3, 0x4(r4)
    addi r4, r4, 0x16c
    bdnz @L_800F7870
    @L_800F7880:
    lwz r4, lbl_80478B00(r13)
    li r3, 0x0
    sth r31, 0x0(r4)
    @L_800F788C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
