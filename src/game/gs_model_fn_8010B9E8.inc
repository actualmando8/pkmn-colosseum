    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    mr r3, r30
    bl fn_8010C3FC
    lwz r6, lbl_8047AD4C(r13)
    li r4, 0x0
    lwz r0, lbl_8047AD48(r13)
    mr r5, r6
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010BA44
@L_8010BA28:
    lwz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8010BA38
    b @L_8010BA48
@L_8010BA38:
    addi r5, r5, 0x10
    addi r4, r4, 0x1
    bdnz @L_8010BA28
@L_8010BA44:
    li r4, -0x1
@L_8010BA48:
    cmpwi r4, 0x0
    bge @L_8010BA58
    li r0, -0x1
    b @L_8010BA78
@L_8010BA58:
    rlwinm r0, r4, 4, 0, 27
    add r3, r6, r0
    lbz r0, 0x6(r3)
    cmplwi r0, 0x2
    bne @L_8010BA74
    li r0, 0x1
    b @L_8010BA78
@L_8010BA74:
    li r0, 0x0
@L_8010BA78:
    extsb r0, r0
    cmpwi r0, 0x1
    beq @L_8010BA8C
    li r3, 0x0
    b @L_8010BBA4
@L_8010BA8C:
    mr r3, r30
    bl fn_8010C3FC
    mr r26, r3
    cmplwi r26, 0x0
    bne @L_8010BAA8
    li r3, 0x0
    b @L_8010BBA4
@L_8010BAA8:
    bl fn_800F92D4
    mr r31, r3
    cmplwi r31, 0x0
    bne @L_8010BAC0
    li r3, 0x0
    b @L_8010BBA4
@L_8010BAC0:
    lis r3, lbl_80404BF0@ha
    li r0, 0xf
    addi r3, r3, lbl_80404BF0@l
    subi r4, r29, 0x4
    subi r5, r3, 0x4
    mtctr r0
@L_8010BAD8:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_8010BAD8
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
    beq @L_8010BB48
    mr r3, r31
    bl fn_800EF4F4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    ble @L_8010BB48
    li r0, 0x2a
    sth r0, 0x5e(r27)
@L_8010BB48:
    lis r3, lbl_80404BF0@ha
    addi r29, r3, lbl_80404BF0@l
    lha r0, 0x54(r29)
    cmpwi r0, 0x0
    bge @L_8010BB90
    mr r3, r31
    bl fn_800EF4FC
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    ble @L_8010BB90
    lha r3, 0x54(r29)
    li r0, 0x2a
    sth r0, 0x5c(r27)
    cmpwi r3, 0x0
    bge @L_8010BB88
    neg r3, r3
@L_8010BB88:
    extsh r0, r3
    sth r0, 0x54(r29)
@L_8010BB90:
    lis r4, lbl_80404BF0@ha
    mr r3, r28
    addi r4, r4, lbl_80404BF0@l
    bl fn_80108580
    li r3, 0x1
@L_8010BBA4:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

