    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, lbl_8036CC00@ha
    lis r4, lbl_8036C638@ha
    lis r5, lbl_80274EC8@ha
    stw r0, 0x14(r1)
    addi r6, r2, -23040
    addi r3, r3, lbl_8036CC00@l
    addi r4, r4, lbl_8036C638@l
    addi r5, r5, lbl_80274EC8@l
    li r7, 0x3c
    li r8, 0x8
    bl fn_80193B30
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
