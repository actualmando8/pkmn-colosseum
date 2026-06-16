    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, lbl_8036C678@ha
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    addi r3, r4, lbl_8036C678@l
    lwz r4, 0x14(r3)
    mr r3, r31
    lwz r12, 0x2c(r4)
    mtctr r12
    bctrl
    cmpwi r3, 0x0
    bge @L_80193F04
    b @L_80193F30
@L_80193F04:
    cmplwi r31, 0x0
    beq @L_80193F18
    lwz r0, 0x8(r31)
    oris r0, r0, 0xc000
    stw r0, 0x8(r31)
@L_80193F18:
    bl HSD_WObjAlloc
    stw r3, 0x24(r31)
    bl HSD_WObjAlloc
    mr r0, r3
    li r3, 0x0
    stw r0, 0x28(r31)
@L_80193F30:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
