    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_800E202C
    mr r31, r3
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_800710D4
    lis r3, -32729
    li r4, 0xde
    addi r3, r3, -30968
    addi r5, r2, -30224
    bl fn_80196E10
@L_800710D4:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_800710F0
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
@L_800710F0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
