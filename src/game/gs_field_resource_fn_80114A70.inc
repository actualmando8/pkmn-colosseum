    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    addi r0, r5, 0x1f
    mr r5, r3
    rlwinm r3, r0, 0, 0, 26
    mr r6, r4
    addi r31, r3, 0x60
    li r4, 0x20
    mr r3, r31
    li r7, 0x0
    bl fn_800F9418
    cmplwi r3, 0x0
    bne @L_80114AC8
    lis r3, -32729
    mr r4, r31
    addi r3, r3, 9312
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_80114ACC
@L_80114AC8:
    addi r3, r3, 0x60
@L_80114ACC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
