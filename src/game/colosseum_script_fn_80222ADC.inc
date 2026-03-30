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
    li r5, 0x31
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 24, 31
    subi r4, r3, 0x1
    extsh. r0, r4
    bge @L_80222B30
    li r4, 0x0
@L_80222B30:
    extsh. r0, r4
    bne @L_80222B44
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    b @L_80222B4C
@L_80222B44:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
@L_80222B4C:
    stw r0, lbl_8047B610(r13)
    mr r3, r31
    extsh r7, r4
    li r4, 0x0
    li r5, 0x31
    li r6, 0x0
    bl fn_8011BBD8
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
