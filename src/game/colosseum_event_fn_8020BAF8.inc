    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x14(r1)
    li r6, 0x0
    stmw r30, 0x8(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r31
    bl fn_8020D908
    mr r31, r3
    bl fn_80212840
    mr r3, r31
    mr r4, r30
    bl fn_801F0058
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020BB5C
    li r3, 0x0
    li r4, 0x4
    bl fn_801F3984
    b @L_8020BB68
@L_8020BB5C:
    li r3, 0x0
    li r4, 0x5
    bl fn_801F3984
@L_8020BB68:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020BB8C
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801F4C14
@L_8020BB8C:
    lmw r30, 0x8(r1)
    li r3, 0x1
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
