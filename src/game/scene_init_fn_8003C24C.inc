    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lha r0, 0x6(r4)
    cmpwi r0, 0x2e3
    beq @L_8003C2A4
    bge @L_8003C274
    cmpwi r0, 0x2e2
    bge @L_8003C27C
    b @L_8003C2A4
    @L_8003C274:
    cmpwi r0, 0x2e5
    bge @L_8003C2A4
    @L_8003C27C:
    lis r4, lbl_803A6748@ha
    lfs f1, lbl_8047BAC0(r2)
    addi r4, r4, lbl_803A6748@l
    lfs f0, 0x44(r4)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r3)
    b @L_8003C2A8
    @L_8003C2A4:
    bl fn_8003C2B8
    @L_8003C2A8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
