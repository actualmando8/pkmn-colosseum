    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr r26, r3
    lhz r0, 0x0(r26)
    li r29, 0x0
    li r3, 0x1
    li r4, 0x0
    sth r0, 0x2(r26)
    bl fn_800F7A08
    mr r27, r3
    li r3, 0x1
    li r4, 0x0
    bl fn_800F7A7C
    extsb r0, r3
    cmpwi r0, 0x0
    bge @L_80043A08
    neg r0, r0
    @L_80043A08:
    cmpwi r0, 0x20
    bgt @L_80043A28
    extsb r0, r27
    cmpwi r0, 0x0
    bge @L_80043A20
    neg r0, r0
    @L_80043A20:
    cmpwi r0, 0x20
    ble @L_80043B2C
    @L_80043A28:
    extsb r4, r3
    extsb r0, r27
    lis r3, 0x4330
    lfd f2, lbl_8047BCB0(r2)
    xoris r4, r4, 0x8000
    xoris r0, r0, 0x8000
    stw r4, 0xc(r1)
    stw r3, 0x8(r1)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsub f1, f0, f2
    stw r3, 0x10(r1)
    lfd f0, 0x10(r1)
    fsub f2, f0, f2
    bl fn_800CE2D8
    frsp f2, f1
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_80043A7C
    fmr f1, f2
    b @L_80043A80
    @L_80043A7C:
    fneg f1, f2
    @L_80043A80:
    lfs f0, lbl_8047BCFC(r2)
    fcmpo cr0, f1, f0
    bge @L_80043A98
    ori r0, r29, 0x2
    clrlwi r29, r0, 16
    b @L_80043AC4
    @L_80043A98:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_80043AAC
    fmr f1, f2
    b @L_80043AB0
    @L_80043AAC:
    fneg f1, f2
    @L_80043AB0:
    lfs f0, lbl_8047BD00(r2)
    fcmpo cr0, f1, f0
    ble @L_80043AC4
    ori r0, r29, 0x1
    clrlwi r29, r0, 16
    @L_80043AC4:
    lfs f0, lbl_8047BC94(r2)
    lfs f1, lbl_8047BD04(r2)
    fcmpo cr0, f2, f0
    ble @L_80043ADC
    fmr f0, f2
    b @L_80043AE0
    @L_80043ADC:
    fneg f0, f2
    @L_80043AE0:
    fcmpo cr0, f1, f0
    bge @L_80043B2C
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_80043AFC
    fmr f1, f2
    b @L_80043B00
    @L_80043AFC:
    fneg f1, f2
    @L_80043B00:
    lfs f0, lbl_8047BD08(r2)
    fcmpo cr0, f1, f0
    bge @L_80043B2C
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    bge @L_80043B24
    ori r0, r29, 0x4
    clrlwi r29, r0, 16
    b @L_80043B2C
    @L_80043B24:
    ori r0, r29, 0x8
    clrlwi r29, r0, 16
    @L_80043B2C:
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm r0, r3, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_80043B48
    ori r0, r29, 0x1
    clrlwi r29, r0, 16
    @L_80043B48:
    rlwinm r0, r3, 0, 29, 29
    cmplwi r0, 0x0
    beq @L_80043B5C
    ori r0, r29, 0x2
    clrlwi r29, r0, 16
    @L_80043B5C:
    clrlwi r0, r3, 31
    cmplwi r0, 0x0
    beq @L_80043B70
    ori r0, r29, 0x4
    clrlwi r29, r0, 16
    @L_80043B70:
    rlwinm r0, r3, 0, 30, 30
    cmplwi r0, 0x0
    beq @L_80043B84
    ori r0, r29, 0x8
    clrlwi r29, r0, 16
    @L_80043B84:
    rlwinm r0, r3, 0, 23, 23
    cmplwi r0, 0x0
    beq @L_80043B98
    ori r0, r29, 0x10
    clrlwi r29, r0, 16
    @L_80043B98:
    rlwinm r0, r3, 0, 22, 22
    cmplwi r0, 0x0
    beq @L_80043BAC
    ori r0, r29, 0x20
    clrlwi r29, r0, 16
    @L_80043BAC:
    rlwinm r0, r3, 0, 21, 21
    cmplwi r0, 0x0
    beq @L_80043BC0
    ori r0, r29, 0x40
    clrlwi r29, r0, 16
    @L_80043BC0:
    rlwinm r0, r3, 0, 20, 20
    cmplwi r0, 0x0
    beq @L_80043BD4
    ori r0, r29, 0x80
    clrlwi r29, r0, 16
    @L_80043BD4:
    rlwinm r0, r3, 0, 27, 27
    cmplwi r0, 0x0
    beq @L_80043BE8
    ori r0, r29, 0x100
    clrlwi r29, r0, 16
    @L_80043BE8:
    rlwinm r0, r3, 0, 25, 25
    cmplwi r0, 0x0
    beq @L_80043BFC
    ori r0, r29, 0x200
    clrlwi r29, r0, 16
    @L_80043BFC:
    rlwinm r0, r3, 0, 26, 26
    cmplwi r0, 0x0
    beq @L_80043C10
    ori r0, r29, 0x400
    clrlwi r29, r0, 16
    @L_80043C10:
    rlwinm r0, r3, 0, 19, 19
    cmplwi r0, 0x0
    beq @L_80043C24
    ori r0, r29, 0x800
    clrlwi r29, r0, 16
    @L_80043C24:
    lhz r0, 0x2(r26)
    clrlwi r31, r29, 16
    li r27, 0x0
    li r30, 0x0
    xori r0, r0, 0xffff
    and r0, r0, r31
    clrlwi r28, r0, 16
    @L_80043C40:
    li r0, 0x1
    slw r0, r0, r30
    clrlwi r25, r0, 16
    and r0, r28, r25
    cmpwi r0, 0x0
    beq @L_80043C6C
    addi r0, r30, 0xa
    li r3, 0xf
    stbx r3, r26, r0
    or r27, r27, r25
    b @L_80043CAC
    @L_80043C6C:
    and r0, r31, r25
    cmpwi r0, 0x0
    beq @L_80043CAC
    bl fn_800D3088
    addi r4, r30, 0xa
    lbzx r0, r26, r4
    subf r0, r3, r0
    extsb r0, r0
    stbx r0, r26, r4
    lbzx r0, r26, r4
    extsb r0, r0
    cmpwi r0, 0x0
    bgt @L_80043CAC
    lbz r0, lbl_804788C4(r13)
    or r27, r27, r25
    stbx r0, r26, r4
    @L_80043CAC:
    addi r30, r30, 0x1
    cmpwi r30, 0x10
    blt @L_80043C40
    sth r29, 0x0(r26)
    sth r28, 0x4(r26)
    sth r27, 0x6(r26)
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
