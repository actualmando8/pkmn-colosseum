    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r4, 0x0
    stw r0, 0x14(r1)
    bne @L_800EE074
    lis r4, 0x2000
    bl fn_801C2A74
    b @L_800EE07C
    @L_800EE074:
    lis r4, 0x2000
    bl fn_801C2A90
    @L_800EE07C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
