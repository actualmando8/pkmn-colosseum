    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, 0x0
    li r3, 0x1
    bl fn_800F7BC4
    andi. r0, r3, 0x1f70
    cmplwi r0, 0x0
    beq @L_801CB1AC
    li r31, 0x1
@L_801CB1AC:
    mr r3, r31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
