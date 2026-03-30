    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r3
    mr r29, r4
    mr r27, r5
    cmplwi r27, 0x0
    bne @L_8010B744
    li r30, 0x0
    b @L_8010B834
@L_8010B744:
    mr r3, r27
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010B760
    li r30, 0x33d
    b @L_8010B834
@L_8010B760:
    mr r3, r27
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010B77C
    li r30, 0x33d
    b @L_8010B834
@L_8010B77C:
    mr r3, r27
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r26, r3, 0, 16, 31
    cmplwi r26, 0xc9
    bne @L_8010B7EC
    mr r3, r27
    bl fn_8011F5B0
    bl fn_80122334
    rlwinm r26, r3, 0, 24, 31
    mr r3, r27
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010B7D4
    lis r3, lbl_8035B478@ha
    rlwinm r0, r26, 2, 0, 29
    addi r3, r3, lbl_8035B478@l
    lhzx r30, r3, r0
    b @L_8010B834
@L_8010B7D4:
    lis r3, lbl_8035B478@ha
    rlwinm r0, r26, 2, 0, 29
    addi r3, r3, lbl_8035B478@l
    add r3, r3, r0
    lhz r30, 0x2(r3)
    b @L_8010B834
@L_8010B7EC:
    mr r3, r27
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010B81C
    mr r4, r26
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x0
    bl fn_8012640C
    rlwinm r30, r3, 0, 16, 31
    b @L_8010B834
@L_8010B81C:
    mr r4, r26
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x1
    bl fn_8012640C
    rlwinm r30, r3, 0, 16, 31
@L_8010B834:
    mr r3, r30
    bl fn_8010C3FC
    lwz r6, lbl_8047AD4C(r13)
    li r4, 0x0
    lwz r0, lbl_8047AD48(r13)
    mr r5, r6
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010B874
@L_8010B858:
    lwz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8010B868
    b @L_8010B878
@L_8010B868:
    addi r5, r5, 0x10
    addi r4, r4, 0x1
    bdnz @L_8010B858
@L_8010B874:
    li r4, -0x1
@L_8010B878:
    cmpwi r4, 0x0
    bge @L_8010B888
    li r0, -0x1
    b @L_8010B8A8
@L_8010B888:
    rlwinm r0, r4, 4, 0, 27
    add r3, r6, r0
    lbz r0, 0x6(r3)
    cmplwi r0, 0x2
    bne @L_8010B8A4
    li r0, 0x1
    b @L_8010B8A8
@L_8010B8A4:
    li r0, 0x0
@L_8010B8A8:
    extsb r0, r0
    cmpwi r0, 0x1
    beq @L_8010B8BC
    li r3, 0x0
    b @L_8010B9D4
@L_8010B8BC:
    mr r3, r30
    bl fn_8010C3FC
    mr r26, r3
    cmplwi r26, 0x0
    bne @L_8010B8D8
    li r3, 0x0
    b @L_8010B9D4
@L_8010B8D8:
    bl fn_800F92D4
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_8010B8F0
    li r3, 0x0
    b @L_8010B9D4
@L_8010B8F0:
    lis r3, lbl_80404BF0@ha
    li r0, 0xf
    addi r3, r3, lbl_80404BF0@l
    subi r4, r29, 0x4
    subi r5, r3, 0x4
    mtctr r0
@L_8010B908:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8010B908
    lis r3, lbl_80404BF0@ha
    li r4, 0x0
    addi r27, r3, lbl_80404BF0@l
    li r0, 0x2a
    stw r26, 0x58(r27)
    mr r3, r30
    sth r4, 0x50(r27)
    sth r4, 0x52(r27)
    sth r4, 0x5c(r27)
    sth r4, 0x5e(r27)
    sth r0, 0x60(r27)
    sth r0, 0x62(r27)
    bl fn_8010C388
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010B978
    mr r3, r31
    bl fn_800EF4F4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    ble @L_8010B978
    li r0, 0x2a
    sth r0, 0x5e(r27)
@L_8010B978:
    lis r3, lbl_80404BF0@ha
    addi r29, r3, lbl_80404BF0@l
    lha r0, 0x54(r29)
    cmpwi r0, 0x0
    bge @L_8010B9C0
    mr r3, r31
    bl fn_800EF4FC
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    ble @L_8010B9C0
    lha r3, 0x54(r29)
    li r0, 0x2a
    sth r0, 0x5c(r27)
    cmpwi r3, 0x0
    bge @L_8010B9B8
    neg r3, r3
@L_8010B9B8:
    extsh r0, r3
    sth r0, 0x54(r29)
@L_8010B9C0:
    lis r4, lbl_80404BF0@ha
    mr r3, r28
    addi r4, r4, lbl_80404BF0@l
    bl fn_80108580
    li r3, 0x1
@L_8010B9D4:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

