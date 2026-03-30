    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x0
    li r5, 0x1
    stw r0, 0x14(r1)
    li r6, 0x0
    stw r31, 0xc(r1)
    lwz r31, lbl_8047B64C(r13)
    lhz r4, lbl_8047B61C(r13)
    bl fn_8011BEB4
    bl fn_800FA280
    mr r0, r3
    mr r3, r31
    mr r31, r0
    bl fn_80203848
    mr r0, r3
    li r3, 0xd
    mr r4, r0
    bl fn_80132A38
    mr r4, r31
    li r3, 0xe
    bl fn_80132A38
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
