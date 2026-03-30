    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_801A8594
    lis r5, -32698
    mr r4, r3
    addi r3, r5, 22092
    bl fn_801AA498
@L_801A8594:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
