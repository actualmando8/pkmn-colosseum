    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, -32704
    addi r4, r3, -19584
    lwz r3, 0xc(r4)
    cmplwi r3, 0x0
    beq @L_80093F54
    addi r4, r4, 0x1c
    bl fn_80093F64
@L_80093F54:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
