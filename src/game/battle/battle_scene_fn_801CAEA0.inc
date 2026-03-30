    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    cmpwi r31, 0x6
    blt @L_801CAEC4
    li r3, 0x0
    b @L_801CAEF8
@L_801CAEC4:
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    rlwinm r4, r31, 0, 16, 31
    bl fn_8012AC08
    cmplwi r3, 0x0
    bne @L_801CAEE8
    li r3, 0x0
    b @L_801CAEF8
@L_801CAEE8:
    li r4, 0x0
    li r5, 0x77
    li r6, 0x0
    bl fn_8012640C
@L_801CAEF8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
