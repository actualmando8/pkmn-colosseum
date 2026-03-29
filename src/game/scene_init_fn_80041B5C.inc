    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r5, lbl_803A6818@ha
    lfs f1, lbl_8047BCA0(r2)
    addi r5, r5, lbl_803A6818@l
    lfs f0, 0x4c(r5)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r3)
    lha r0, 0x6(r4)
    cmpwi r0, 0x76a
    beq @L_80041BC0
    bge @L_80041BB4
    cmpwi r0, 0x759
    beq @L_80041BC0
    bge @L_80041BBC
    cmpwi r0, 0x331
    beq @L_80041BC0
    b @L_80041BBC
    @L_80041BB4:
    cmpwi r0, 0xfbe
    beq @L_80041BC0
    @L_80041BBC:
    bl fn_800411FC
    @L_80041BC0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
