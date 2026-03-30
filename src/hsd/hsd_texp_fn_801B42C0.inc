    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    b @L_801B42E4
@L_801B42D4:
    lwz r31, 0x0(r3)
    li r4, 0x88
    bl fn_80193AF0
    mr r3, r31
@L_801B42E4:
    cmplwi r3, 0x0
    bne @L_801B42D4
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
