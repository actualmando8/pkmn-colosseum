    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_801CAF0C
    clrlwi. r0, r3, 16
    beq @L_8025E630
    bl fn_800FF560
    lis r5, 0x8026
    mr r4, r3
    addi r8, r5, -6860
    li r3, 0x1
    li r5, 0x4000
    li r6, 0x1
    li r7, 0x1
    bl fn_800F07A8
@L_8025E630:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
