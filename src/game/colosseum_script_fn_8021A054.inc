    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x12
    mr r28, r0
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xf0
    li r6, 0x0
    bl fn_8012640C
    lis r5, 0x1
    rlwinm r30, r3, 0, 16, 31
    mr r3, r28
    li r4, 0x0
    subi r7, r5, 0x1
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x10
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021A17C
    cmplwi r30, 0x0
    bne @L_8021A0F0
    li r0, 0x1
    b @L_8021A154
@L_8021A0F0:
    cmplwi r30, 0x165
    bne @L_8021A100
    li r0, 0x1
    b @L_8021A154
@L_8021A100:
    cmplwi r30, 0x163
    bne @L_8021A110
    li r0, 0x1
    b @L_8021A154
@L_8021A110:
    lis r4, lbl_80279FA0@ha
    li r3, 0x0
    addi r4, r4, lbl_80279FA0@l
    b @L_8021A12C
@L_8021A120:
    cmplw r30, r5
    beq @L_8021A138
    addi r3, r3, 0x2
@L_8021A12C:
    lhzx r5, r4, r3
    cmplwi r5, 0xfffe
    bne @L_8021A120
@L_8021A138:
    lis r3, 0x1
    subi r0, r3, 0x2
    rlwinm r0, r0, 0, 16, 31
    subf r3, r5, r0
    subic r0, r3, 0x1
    subfe r0, r0, r3
    rlwinm r0, r0, 0, 24, 31
@L_8021A154:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_8021A17C
    cmplwi r30, 0x0
    beq @L_8021A17C
    cmplwi r30, 0xffff
    beq @L_8021A17C
    cmplwi r30, 0x165
    beq @L_8021A17C
    cmplwi r30, 0x163
    bne @L_8021A18C
@L_8021A17C:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021A2AC
@L_8021A18C:
    mr r3, r31
    bl fn_80205B8C
    mr r4, r30
    bl fn_80123B5C
    extsb. r0, r3
    bge @L_8021A17C
    mr r3, r28
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_8011BEB4
    extsb. r29, r3
    blt @L_8021A17C
    mr r3, r31
    bl fn_801FECD4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021A1E0
    mr r3, r31
    bl fn_801FEC10
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021A17C
@L_8021A1E0:
    mr r3, r31
    li r4, 0x31
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021A204
    mr r3, r31
    li r4, 0x31
    li r5, 0x0
    bl fn_8020248C
@L_8021A204:
    mr r3, r31
    bl fn_80205B8C
    mr r28, r3
    mr r4, r29
    mr r5, r30
    bl fn_80123D58
    mr r4, r30
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x5
    ble @L_8021A240
    li r0, 0x5
@L_8021A240:
    mr r3, r28
    mr r6, r29
    rlwinm r7, r0, 0, 24, 31
    li r4, 0x0
    li r5, 0x80
    bl fn_801254B4
    mr r4, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r31
    li r4, 0x31
    bl fn_80201890
    li r0, 0x1
    li r4, 0x31
    slw r0, r0, r29
    or r5, r3, r0
    mr r3, r31
    bl fn_80201764
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021A2AC:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
