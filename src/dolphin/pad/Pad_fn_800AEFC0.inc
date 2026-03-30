    nofralloc
    mflr r0
    lis r4, lbl_80312668@ha
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    addi r31, r4, lbl_80312668@l
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    @L_800AEFE0:
    bl fn_800AE7A4
    cmplwi r3, 0x0
    beq @L_800AEFE0
    bl fn_800AE7B4
    stw r3, 0xc(r1)
    lis r3, 0x80f4
    subi r3, r3, 0x5fff
    bl fn_800AE7CC
    @L_800AF000:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF000
    lwz r3, 0xc(r30)
    bl fn_800AE7CC
    @L_800AF014:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF014
    lis r3, 0x80f4
    subi r3, r3, 0x3ffe
    bl fn_800AE7CC
    @L_800AF02C:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF02C
    lwz r0, 0x14(r30)
    clrlwi r3, r0, 16
    bl fn_800AE7CC
    @L_800AF044:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF044
    lis r3, 0x80f4
    subi r3, r3, 0x5ffe
    bl fn_800AE7CC
    @L_800AF05C:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF05C
    lwz r3, 0x10(r30)
    bl fn_800AE7CC
    @L_800AF070:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF070
    lis r3, 0x80f4
    subi r3, r3, 0x4ffe
    bl fn_800AE7CC
    @L_800AF088:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF088
    li r3, 0x0
    bl fn_800AE7CC
    @L_800AF09C:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF09C
    lis r3, 0x80f4
    subi r3, r3, 0x2fff
    bl fn_800AE7CC
    @L_800AF0B4:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF0B4
    lhz r3, 0x24(r30)
    bl fn_800AE7CC
    @L_800AF0C8:
    bl fn_800AE794
    cmplwi r3, 0x0
    bne @L_800AF0C8
    addi r3, r31, 0x0
    crxor 6, 6, 6
    addi r4, r30, 0x0
    bl fn_800AE9AC
    lwz r4, 0xc(r30)
    addi r3, r31, 0x20
    crxor 6, 6, 6
    bl fn_800AE9AC
    lwz r4, 0x14(r30)
    addi r3, r31, 0x50
    crxor 6, 6, 6
    bl fn_800AE9AC
    lwz r4, 0x10(r30)
    addi r3, r31, 0x80
    crxor 6, 6, 6
    bl fn_800AE9AC
    lwz r4, 0x1c(r30)
    addi r3, r31, 0xb0
    crxor 6, 6, 6
    bl fn_800AE9AC
    lhz r4, 0x24(r30)
    addi r3, r31, 0xe0
    crxor 6, 6, 6
    bl fn_800AE9AC
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
