    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    lwz r0, -21432(r13)
    cmpwi r0, 0x0
    beq @L_801E38C4
    lis r3, -32697
    addi r31, r3, -23404
@L_801E3890:
    mr r3, r31
    addi r4, r1, 0x8
    li r5, 0x0
    bl fn_8009F2F8
    cmpwi r3, 0x1
    bne @L_801E38B0
    lwz r3, 0x8(r1)
    b @L_801E38B4
@L_801E38B0:
    li r3, 0x0
@L_801E38B4:
    cmplwi r3, 0x0
    beq @L_801E38C4
    bl fn_801E4F34
    b @L_801E3890
@L_801E38C4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
