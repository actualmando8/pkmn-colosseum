    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_801AD664
    mr r31, r3
    b @L_801AD65C
@L_801AD63C:
    cmplwi r31, 0x0
    beq @L_801AD658
    lwz r5, 0x0(r31)
    mr r4, r31
    lwz r3, 0x18(r31)
    lwz r5, 0x48(r5)
    bl fn_801C27F4
@L_801AD658:
    lwz r31, 0x4(r31)
@L_801AD65C:
    cmplwi r31, 0x0
    bne @L_801AD63C
@L_801AD664:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
