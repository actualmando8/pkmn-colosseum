    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r27, r3
    beq @L_8020EDA8
    rlwinm r31, r4, 0, 16, 31
    li r28, 0x0
    b @L_8020ED9C
@L_8020ED04:
    rlwinm r0, r28, 0, 16, 31
    li r4, -0x1
    mulli r0, r0, 0x14
    add r29, r27, r0
    mr r3, r29
    bl fn_801FBE18
    li r30, 0x0
    b @L_8020ED38
@L_8020ED24:
    mr r3, r29
    mr r4, r30
    li r5, 0x0
    bl fn_801FBDF4
    addi r30, r30, 0x1
@L_8020ED38:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8020ED24
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDE4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDD4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDC4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDB4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBDA4
    mr r3, r29
    li r4, 0x0
    bl fn_801FBD94
    mr r3, r29
    li r4, 0x0
    bl fn_801FBD84
    addi r28, r28, 0x1
@L_8020ED9C:
    rlwinm r0, r28, 0, 16, 31
    cmplw r0, r31
    blt @L_8020ED04
@L_8020EDA8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
