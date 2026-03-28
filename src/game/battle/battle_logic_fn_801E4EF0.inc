    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32697
    mr r5, r3
    stw r0, 0x14(r1)
    addi r3, r4, -15960
    addi r4, r1, 0x8
    bl fn_8009F2F8
    cmpwi r3, 0x1
    bne @L_801E4F20
    lwz r3, 0x8(r1)
    b @L_801E4F24
@L_801E4F20:
    li r3, 0x0
@L_801E4F24:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
