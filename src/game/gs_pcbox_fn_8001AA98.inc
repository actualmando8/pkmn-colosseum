    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lis r3, lbl_803A1D40@ha
    li r31, 0x0
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x1(r3)
    cmplwi r0, 0x0
    bne @L_8001AAD4
    li r0, 0x1
    stb r0, 0x98(r30)
    b @L_8001AB58
    @L_8001AAD4:
    bl fn_80105624
    lhz r3, 0x4(r3)
    rlwinm r0, r3, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8001AB04
    lwz r3, 0x4(r30)
    bl fn_801022B8
    clrlwi r0, r3, 16
    cmplwi r0, 0x3b6
    bne @L_8001AB14
    li r31, 0x1
    b @L_8001AB14
    @L_8001AB04:
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_8001AB14
    li r31, 0x1
    @L_8001AB14:
    clrlwi r0, r31, 24
    cmplwi r0, 0x0
    beq @L_8001AB50
    lis r3, lbl_803A1D40@ha
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x15(r3)
    cmplwi r0, 0x0
    bne @L_8001AB40
    li r3, 0x26
    bl fn_80166A28
    b @L_8001AB58
    @L_8001AB40:
    li r0, 0x1
    stb r0, 0x98(r30)
    stb r0, 0x99(r30)
    b @L_8001AB58
    @L_8001AB50:
    mr r3, r30
    bl fn_80102ED4
    @L_8001AB58:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
