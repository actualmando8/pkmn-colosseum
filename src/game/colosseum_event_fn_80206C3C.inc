    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr. r29, r3
    beq @L_80206C80
    rlwinm r31, r4, 0, 16, 31
    li r30, 0x0
    b @L_80206C74
@L_80206C60:
    rlwinm r0, r30, 0, 16, 31
    mulli r0, r0, 0x6e0
    add r3, r29, r0
    bl fn_80206C94
    addi r30, r30, 0x1
@L_80206C74:
    rlwinm r0, r30, 0, 16, 31
    cmplw r0, r31
    blt @L_80206C60
@L_80206C80:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
