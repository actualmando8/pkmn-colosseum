    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lis r3, lbl_803A1D40@ha
    li r6, 0x0
    addi r3, r3, lbl_803A1D40@l
    lbz r0, 0x14(r3)
    cmpwi r0, 0x2
    beq @L_8001AA58
    bge @L_8001A9C4
    cmpwi r0, 0x0
    beq @L_8001A9D0
    bge @L_8001AA30
    b @L_8001AA64
    @L_8001A9C4:
    cmpwi r0, 0x4
    bge @L_8001AA64
    b @L_8001AA60
    @L_8001A9D0:
    lbz r0, 0x0(r3)
    cmpwi r0, 0x5
    beq @L_8001AA20
    bge @L_8001A9F0
    cmpwi r0, 0x3
    bge @L_8001A9F8
    cmpwi r0, 0x1
    b @L_8001AA28
    @L_8001A9F0:
    cmpwi r0, 0x8
    b @L_8001AA28
    @L_8001A9F8:
    lhz r3, 0x12(r3)
    bl fn_801440A0
    bl fn_80144014
    clrlwi r0, r3, 24
    cmplwi r0, 0x4
    bne @L_8001AA18
    li r6, 0x2b63
    b @L_8001AA64
    @L_8001AA18:
    li r6, 0x2b61
    b @L_8001AA64
    @L_8001AA20:
    li r6, 0x2b62
    b @L_8001AA64
    @L_8001AA28:
    li r6, 0x2b5d
    b @L_8001AA64
    @L_8001AA30:
    lbz r4, 0x6(r3)
    lis r3, lbl_803A1C20@ha
    addi r0, r3, lbl_803A1C20@l
    li r3, 0x32
    extsb r4, r4
    mulli r4, r4, 0x30
    add r4, r0, r4
    bl fn_80132A38
    li r6, 0x2b5e
    b @L_8001AA64
    @L_8001AA58:
    li r6, 0x2b5f
    b @L_8001AA64
    @L_8001AA60:
    li r6, 0x2b60
    @L_8001AA64:
    cmplwi r6, 0x0
    beq @L_8001AA84
    lbz r4, 0x8b(r31)
    li r0, -0x100
    li r3, 0x0
    or r5, r4, r0
    li r4, 0x0
    bl fn_800FB680
    @L_8001AA84:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
