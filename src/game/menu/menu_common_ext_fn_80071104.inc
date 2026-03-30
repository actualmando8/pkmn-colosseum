    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    addi r0, r3, 0x1f
    li r4, 0x20
    rlwinm r3, r0, 0, 0, 26
    bl fn_800E2C04
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x0
    beq @L_80071134
    bl fn_800E27B0
    b @L_80071150
@L_80071134:
    bne @L_8007114C
    lis r3, -32729
    li r4, 0xd5
    addi r3, r3, -30968
    addi r5, r2, -30224
    bl fn_80196E10
@L_8007114C:
    li r3, 0x0
@L_80071150:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
