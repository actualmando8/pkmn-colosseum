    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_801DB140
    lbz r0, 0x74(r31)
    cmplwi r0, 0x0
    beq @L_801DB130
    bl fn_801DD3E4
    mr r3, r31
    bl fn_801DD23C
@L_801DB130:
    mr r3, r31
    li r4, 0x0
    li r5, 0x8c
    bl memset
@L_801DB140:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
