    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r3, 0x8046
    li r0, 0x258
    addi r4, r3, 29584
    lwz r3, 0x8(r4)
    stw r0, 0x4(r4)
    cmplwi r3, 0x0
    beq @L_801D23F4
    li r4, 0x0
    li r5, 0x0
    bl fn_801669E4
@L_801D23F4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
