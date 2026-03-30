    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8025DCE8
    li r4, 0x32
    li r5, 0xff
    bl fn_80165A20
@L_8025DCE8:
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_8025DD00
    li r4, 0x32
    li r5, 0xff
    bl fn_801659FC
@L_8025DD00:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
