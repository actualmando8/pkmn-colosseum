    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    cmplwi r30, 0x0
    beq @L_80113874
    bl fn_800FF56C
    lis r4, lbl_80408378@ha
    li r0, 0x1
    addi r4, r4, lbl_80408378@l
    stw r3, 0x0(r4)
    mr r3, r30
    stw r30, 0x4(r4)
    stb r0, 0x8(r4)
    stw r31, 0xc(r4)
    bl fn_800FF58C
@L_80113874:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
