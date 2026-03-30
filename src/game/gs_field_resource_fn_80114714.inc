    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_800F9318
    mr r31, r3
    bl fn_800EFD3C
    cmplwi r3, 0x0
    bne @L_80114748
    lis r3, -32729
    addi r3, r3, 9000
    crxor 6, 6, 6
    bl fn_800DD970
@L_80114748:
    mr r3, r31
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
