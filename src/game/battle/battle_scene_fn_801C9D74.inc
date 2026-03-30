    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_800F92D4
    cmplwi r3, 0x0
    bne @L_801C9DAC
    lis r3, -32729
    mr r4, r31
    addi r3, r3, 22700
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801C9DB0
@L_801C9DAC:
    bl fn_800DCAA4
@L_801C9DB0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
