    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    cmpwi r31, 0x0
    li r0, 0x0
    blt @L_8006B4D8
    cmplwi r31, 0x6
    bge @L_8006B4D8
    li r0, 0x1
    @L_8006B4D8:
    cmpwi r0, 0x0
    bne @L_8006B4F8
    lis r3, lbl_80267DE8@ha
    lis r5, lbl_80267E70@ha
    addi r3, r3, lbl_80267DE8@l
    li r4, 0xb9
    addi r5, r5, lbl_80267E70@l
    bl fn_80196E10
    @L_8006B4F8:
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    stw r31, 0x8(r3)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
