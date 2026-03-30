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
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BEB4
    mr r3, r31
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_8011BEB4
    srawi r0, r3, 1
    addze r0, r0
    neg. r7, r0
    bne @L_8021C15C
    li r7, -0x1
@L_8021C15C:
    mr r3, r31
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
