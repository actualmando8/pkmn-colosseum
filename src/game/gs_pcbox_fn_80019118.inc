    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    mr r31, r4
    lwz r4, 0x60(r3)
    lis r5, lbl_80266C10@ha
    addi r6, r5, lbl_80266C10@l
    lbz r3, 0x95(r3)
    lwz r0, 0xc(r4)
    lwz r5, 0x0(r6)
    extsb r3, r3
    subfic r0, r0, 0x4
    lwz r4, 0x4(r6)
    add r7, r3, r0
    lwz r3, 0x8(r6)
    lwz r0, 0xc(r6)
    cmpwi r7, 0x0
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r3, 0x10(r1)
    stw r0, 0x14(r1)
    blt @L_80019180
    cmpwi r7, 0x4
    blt @L_80019188
    @L_80019180:
    li r3, 0x0
    b @L_800191EC
    @L_80019188:
    slwi r0, r7, 2
    addi r3, r1, 0x8
    lha r4, 0x6(r31)
    lwzx r0, r3, r0
    cmpw r4, r0
    bne @L_800191A8
    li r30, 0x1
    b @L_800191AC
    @L_800191A8:
    li r30, 0x0
    @L_800191AC:
    mr r3, r31
    mr r4, r30
    bl fn_80109220
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_800191E8
    lfs f1, lbl_8047B7A4(r2)
    lfs f0, lbl_8047A300(r13)
    lfs f2, lbl_8047B7A0(r2)
    fsubs f0, f1, f0
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    stb r0, 0x67(r31)
    @L_800191E8:
    li r3, 0x0
    @L_800191EC:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
