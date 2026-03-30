    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lfs f0, lbl_8047CBC8(r2)
    li r4, 0x180
    stw r0, 0x14(r1)
    li r5, 0x80
    li r0, 0x0
    lis r3, lbl_80401490@ha
    stw r31, 0xc(r1)
    addi r31, r3, lbl_80401490@l
    stw r30, 0x8(r1)
    li r30, 0x0
    stb r5, lbl_8047AB94(r13)
    stw r4, lbl_8047AB90(r13)
    stw r4, lbl_8047AB8C(r13)
    stfs f0, lbl_8047AB88(r13)
    stw r0, lbl_8047AB84(r13)
    @L_800E931C:
    bl fn_801B1730
    stw r3, 0x54(r31)
    li r4, 0x0
    lwz r3, 0x54(r31)
    bl fn_801B0880
    addi r30, r30, 0x1
    addi r31, r31, 0x58
    cmplwi r30, 0x6
    blt @L_800E931C
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
