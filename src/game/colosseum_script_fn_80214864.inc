    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x1
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F453C
    rlwinm r30, r3, 0, 24, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    cmplwi r30, 0x2
    mr r31, r3
    bne @L_802148DC
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    li r7, 0xb
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x2c
    li r6, 0x0
    li r7, 0x2
    bl fn_8011BBD8
    b @L_80214998
@L_802148DC:
    cmplwi r30, 0x3
    bne @L_80214914
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    li r7, 0x5
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x2c
    li r6, 0x0
    li r7, 0x2
    bl fn_8011BBD8
    b @L_80214998
@L_80214914:
    cmplwi r30, 0x1
    bne @L_8021494C
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    li r7, 0xa
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x2c
    li r6, 0x0
    li r7, 0x2
    bl fn_8011BBD8
    b @L_80214998
@L_8021494C:
    cmplwi r30, 0x4
    bne @L_80214984
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    li r7, 0xf
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x2c
    li r6, 0x0
    li r7, 0x2
    bl fn_8011BBD8
    b @L_80214998
@L_80214984:
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
@L_80214998:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
