    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    lwz r3, lbl_8047B64C(r13)
    bl fn_80205BE8
    mr r29, r3
    bl fn_80262308
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021EBE0
@L_8021EB14:
    bl fn_8026246C
    lhz r4, lbl_8047B61C(r13)
    mr r3, r29
    bl fn_80262270
    mr r31, r3
    cmpwi r31, -0x1
    beq @L_8021EBE0
    cmpwi r31, 0x4
    bge @L_8021EBE0
    mr r3, r29
    rlwinm r6, r31, 0, 16, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r30, r3, 0, 16, 31
    li r3, 0x0
    mr r4, r30
    li r5, 0x19
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021EB8C
    li r3, 0x7635
    bl fn_802624CC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021EB14
    bl fn_8026246C
    b @L_8021EB14
@L_8021EB8C:
    lhz r5, lbl_8047B61C(r13)
    mr r3, r29
    rlwinm r4, r31, 0, 16, 31
    bl fn_80123D58
    mr r4, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r0, r3
    li r3, 0xe
    mr r4, r0
    bl fn_80132A38
    li r3, 0x5d
    li r4, 0x468
    bl fn_80132A38
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021EBEC
@L_8021EBE0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021EBEC:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
