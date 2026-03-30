    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047AD28(r13)
    cmplwi r0, 0x0
    bne @L_80109844
    li r3, 0x0
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    li r7, 0x0
    bl fn_800EF5FC
    stw r3, lbl_8047AD28(r13)
@L_80109844:
    lfs f0, lbl_8047CE50(r13)
    li r0, 0x0
    stb r0, lbl_8047AD21(r13)
    stb r0, lbl_8047AD20(r13)
    stb r0, lbl_8047AD22(r13)
    stb r0, lbl_8047AD23(r13)
    stb r0, lbl_8047AD24(r13)
    stfs f0, lbl_8047AD38(r13)
    stfs f0, lbl_8047AD3C(r13)
    stfs f0, lbl_8047AD2C(r13)
    stfs f0, lbl_8047AD30(r13)
    stfs f0, lbl_8047AD34(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

