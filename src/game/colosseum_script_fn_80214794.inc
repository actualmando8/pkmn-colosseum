    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801F025C
    mr r30, r3
    bl fn_802040E8
    mr r31, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0xfa
    li r6, 0x0
    bl fn_8012640C
    rlwinm. r29, r3, 0, 16, 31
    beq @L_80214844
    rlwinm. r0, r31, 0, 16, 31
    bne @L_80214844
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    mr r4, r29
    li r5, 0x1
    li r6, 0x1
    bl fn_8020147C
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214834
    mr r3, r30
    li r4, 0x82
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80214834:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80214850
@L_80214844:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80214850:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
