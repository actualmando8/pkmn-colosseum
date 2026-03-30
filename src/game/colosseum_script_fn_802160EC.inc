    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0xe
    mr r31, r0
    mr r4, r31
    bl fn_801F025C
    mr. r30, r3
    beq @L_802161D0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x19
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_802161D0
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802161D0
    mr r3, r31
    li r4, 0x32
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802161D0
    mr r3, r30
    li r4, 0x32
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802161D0
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x32
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802161C0
    mr r3, r30
    li r4, 0x32
    li r5, 0x0
    bl fn_8020248C
@L_802161C0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_802161DC
@L_802161D0:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_802161DC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
