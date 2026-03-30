    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, lbl_80401490@ha
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    addi r30, r3, lbl_80401490@l
    stw r29, 0x14(r1)
    li r29, 0x0
    @L_800E8F24:
    lwz r3, 0x54(r30)
    bl fn_801B06DC
    lwz r3, 0x54(r30)
    li r4, 0x0
    bl fn_801B0880
    addi r29, r29, 0x1
    stb r31, 0x50(r30)
    cmplwi r29, 0x6
    addi r30, r30, 0x58
    blt @L_800E8F24
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
