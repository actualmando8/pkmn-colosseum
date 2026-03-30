    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    mr r31, r3
    li r5, 0x2b
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802267C8
    mr r3, r31
    bl fn_802096E8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802267C8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x52
    li r6, 0x0
    li r7, 0x7631
    bl fn_801F4C14
    li r3, 0x7631
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802267C8
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x7(r3)
@L_802267C8:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
