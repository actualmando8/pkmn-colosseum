    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xe
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    li r3, 0x0
    bl fn_8011BEB4
    rlwinm r31, r3, 0, 24, 31
    mr r3, r30
    li r4, 0x2b
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229C78
    cmplwi r31, 0x1
    bne @L_80229C78
    li r3, 0x1
    b @L_80229C7C
@L_80229C78:
    li r3, 0x0
@L_80229C7C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
