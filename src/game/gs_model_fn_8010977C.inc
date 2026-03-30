    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lfs f0, lbl_8047CE50(r13)
    li r6, 0x0
    li r0, 0x1
    lis r3, fn_80109884@ha
    addi r4, r3, fn_80109884@l
    stb r6, lbl_8047AD24(r13)
    lwz r3, lbl_8047AD28(r13)
    li r5, 0x0
    stb r0, lbl_8047AD21(r13)
    stb r6, lbl_8047AD20(r13)
    stb r6, lbl_8047AD22(r13)
    stfs f0, lbl_8047AD2C(r13)
    stfs f0, lbl_8047AD30(r13)
    stfs f0, lbl_8047AD34(r13)
    stfs f0, lbl_8047AD38(r13)
    stfs f0, lbl_8047AD3C(r13)
    bl fn_800DC390
    rlwinm r31, r31, 0, 24, 31
    b @L_801097EC
@L_801097DC:
    lbz r0, lbl_8047AD20(r13)
    cmplwi r0, 0x0
    bne @L_801097F4
    bl fn_800F0308
@L_801097EC:
    cmplwi r31, 0x0
    bne @L_801097DC
@L_801097F4:
    lbz r0, lbl_8047AD20(r13)
    rlwinm r3, r0, 0, 24, 31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

