    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0xf
    li r6, 0x0
    stmw r29, 0x14(r1)
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80136428
    mr r0, r3
    mr r3, r30
    rlwinm r31, r0, 0, 24, 31
    mr r4, r31
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80214774
    li r29, 0x0
    b @L_80214740
@L_8021472C:
    mr r3, r30
    mr r4, r29
    mr r5, r31
    bl fn_80207B5C
    addi r29, r29, 0x1
@L_80214740:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_8021472C
    mr r3, r31
    bl fn_8010C4D4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80214780
@L_80214774:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80214780:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
