    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x0
    bl fn_80083CFC
    cmplwi r3, 0x0
    beq @L_80089BB0
    lbz r4, 0x4136(r3)
    b @L_80089BB4
@L_80089BB0:
    li r4, 0x0
@L_80089BB4:
    lis r3, -32721
    li r5, 0x0
    addi r3, r3, -5224
    li r0, 0x10
    mtctr r0
@L_80089BC8:
    lbz r0, 0x1(r3)
    cmplw r4, r0
    bne @L_80089BE8
    lis r3, -32721
    rlwinm r0, r5, 1, 0, 30
    addi r3, r3, -5224
    lbzx r3, r3, r0
    b @L_80089C00
@L_80089BE8:
    addi r3, r3, 0x2
    addi r5, r5, 0x1
    bdnz @L_80089BC8
    lis r3, -32721
    addi r3, r3, -5224
    lbz r3, 0x0(r3)
@L_80089C00:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
