    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lhz r3, -22990(r13)
    bl fn_801440A0
    bl fn_80144088
    cmplwi r3, 0x0
    bne @L_801323B8
    li r3, 0x2b6e
@L_801323B8:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
