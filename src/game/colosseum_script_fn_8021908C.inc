    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x12
    mr r29, r0
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xef
    li r6, 0x0
    bl fn_8012640C
    lis r5, 0x1
    rlwinm r31, r3, 0, 16, 31
    mr r3, r29
    li r4, 0x0
    subi r7, r5, 0x1
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r30
    li r4, 0x10
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80219160
    cmplwi r31, 0x164
    beq @L_80219148
    subi r0, r31, 0xa5
    rlwinm r0, r0, 0, 16, 31
    cmplwi r0, 0x1
    ble @L_80219148
    cmplwi r31, 0xffff
    beq @L_80219148
    cmplwi r31, 0x0
    beq @L_80219148
    cmplwi r31, 0x165
    bne @L_80219150
@L_80219148:
    li r0, 0x1
    b @L_80219154
@L_80219150:
    li r0, 0x0
@L_80219154:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80219170
@L_80219160:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021925C
@L_80219170:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r28, r0
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r28
    mr r28, r0
    bl fn_80205BE8
    mr r0, r3
    mr r3, r28
    mr r28, r0
    bl fn_80205BE8
    mr r0, r3
    mr r3, r28
    mr r27, r0
    mr r4, r31
    bl fn_80123B5C
    extsb. r0, r3
    bge @L_80219160
    mr r3, r29
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_8011BEB4
    extsb. r29, r3
    blt @L_80219160
    mr r3, r28
    mr r4, r29
    mr r5, r31
    bl fn_80123D58
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021922C
    mr r3, r27
    mr r4, r29
    mr r5, r31
    bl fn_80123D58
@L_8021922C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021925C:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
