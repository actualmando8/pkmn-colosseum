    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    addi r5, r13, -20920
    cmpwi r3, 0x0
    stw r0, 0x14(r1)
    stw r3, -20920(r13)
    stw r4, 0x4(r5)
    beq @L_8025F2E8
    cmpwi r3, 0x1
    bne @L_8025F2EC
@L_8025F2E8:
    bl fn_8025EF58
@L_8025F2EC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
