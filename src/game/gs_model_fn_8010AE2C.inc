    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r30, r4
    mr r31, r5
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    cmplwi r28, 0x0
    bne @L_8010AE74
    li r29, 0x0
    b @L_8010AF60
@L_8010AE74:
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010AE8C
    li r29, 0x33d
    b @L_8010AF60
@L_8010AE8C:
    mr r3, r28
    bl fn_8011E8DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010AEA8
    li r29, 0x33d
    b @L_8010AF60
@L_8010AEA8:
    mr r3, r28
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    cmplwi r29, 0xc9
    bne @L_8010AF18
    mr r3, r28
    bl fn_8011F5B0
    bl fn_80122334
    rlwinm r29, r3, 0, 24, 31
    mr r3, r28
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010AF00
    lis r3, lbl_8035B478@ha
    rlwinm r0, r29, 2, 0, 29
    addi r3, r3, lbl_8035B478@l
    lhzx r29, r3, r0
    b @L_8010AF60
@L_8010AF00:
    lis r3, lbl_8035B478@ha
    rlwinm r0, r29, 2, 0, 29
    addi r3, r3, lbl_8035B478@l
    add r3, r3, r0
    lhz r29, 0x2(r3)
    b @L_8010AF60
@L_8010AF18:
    mr r3, r28
    bl fn_80125390
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010AF48
    mr r4, r29
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    b @L_8010AF60
@L_8010AF48:
    mr r4, r29
    li r3, 0x0
    li r5, 0x5b
    li r6, 0x1
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
@L_8010AF60:
    mr r3, r29
    mr r4, r30
    mr r5, r31
    bl fn_8010B16C
@L_8010AF70:
    mr r3, r29
    bl fn_8010C3FC
    lwz r6, lbl_8047AD4C(r13)
    li r4, 0x0
    lwz r0, lbl_8047AD48(r13)
    mr r5, r6
    mtctr r0
    cmpwi r0, 0x0
    ble @L_8010AFB0
@L_8010AF94:
    lwz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_8010AFA4
    b @L_8010AFB4
@L_8010AFA4:
    addi r5, r5, 0x10
    addi r4, r4, 0x1
    bdnz @L_8010AF94
@L_8010AFB0:
    li r4, -0x1
@L_8010AFB4:
    cmpwi r4, 0x0
    bge @L_8010AFC4
    li r0, -0x1
    b @L_8010AFE4
@L_8010AFC4:
    rlwinm r3, r4, 4, 0, 27
    addi r0, r3, 0x6
    lbzx r0, r6, r0
    cmplwi r0, 0x2
    bne @L_8010AFE0
    li r0, 0x1
    b @L_8010AFE4
@L_8010AFE0:
    li r0, 0x0
@L_8010AFE4:
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8010AFF8
    bl fn_800F0308
    b @L_8010AF70
@L_8010AFF8:
    li r3, 0x1
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

