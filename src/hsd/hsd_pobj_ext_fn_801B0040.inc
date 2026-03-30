    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_801B0088
    mr r31, r3
    b @L_801B0080
@L_801B0060:
    cmplwi r31, 0x0
    beq @L_801B007C
    lis r4, -32741
    lwz r3, 0x18(r31)
    addi r5, r4, 156
    mr r4, r31
    bl fn_801C27F4
@L_801B007C:
    lwz r31, 0x0(r31)
@L_801B0080:
    cmplwi r31, 0x0
    bne @L_801B0060
@L_801B0088:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
