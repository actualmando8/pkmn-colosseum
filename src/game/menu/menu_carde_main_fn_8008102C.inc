    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, 0x0(r4)
    lis r4, lbl_80268DC0@ha
    addi r4, r4, lbl_80268DC0@l
    cmplwi r0, 0x4a
    bgt @L_80082624
    lis r9, jumptable_802EEA4C@ha
    rlwinm r0, r0, 2, 0, 29
    addi r9, r9, jumptable_802EEA4C@l
    lwzx r0, r9, r0
    mtctr r0
    bctr
    lwz r4, 0x0(r3)
    stw r6, 0x0(r4)
    lwz r3, 0x0(r3)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x2
    bge @L_80081084
    cmpwi r0, 0x0
    bge @L_8008263C
@L_80081084:
    li r3, 0x0
    b @L_80082640
    lwz r4, 0x0(r3)
    rlwinm r0, r6, 0, 24, 31
    stb r0, 0x4(r4)
    lwz r3, 0x0(r3)
    lbz r0, 0x4(r3)
    cmpwi r0, 0x0
    beq @L_800810B8
    blt @L_800810B8
    cmpwi r0, 0x6
    bge @L_800810B8
    b @L_8008263C
@L_800810B8:
    li r3, 0x0
    b @L_80082640
    lwz r4, 0x0(r3)
    rlwinm r0, r6, 0, 24, 31
    stb r0, 0x5(r4)
    lwz r3, 0x0(r3)
    lbz r0, 0x5(r3)
    cmpwi r0, 0x0
    beq @L_800810EC
    blt @L_800810EC
    cmpwi r0, 0x4
    bge @L_800810EC
    b @L_8008263C
@L_800810EC:
    li r3, 0x0
    b @L_80082640
    lwz r4, 0x0(r3)
    rlwinm r0, r6, 0, 24, 31
    stb r0, 0x6(r4)
    lwz r3, 0x0(r3)
    lbz r0, 0x6(r3)
    cmpwi r0, 0x0
    beq @L_80081120
    blt @L_80081120
    cmpwi r0, 0xa
    bge @L_80081120
    b @L_8008263C
@L_80081120:
    li r3, 0x0
    b @L_80082640
    lwz r4, 0x0(r3)
    rlwinm r0, r6, 0, 24, 31
    stb r0, 0x7(r4)
    lwz r3, 0x0(r3)
    lbz r0, 0x7(r3)
    cmpwi r0, 0x0
    beq @L_80081150
    blt @L_8008263C
    cmpwi r0, 0xc
    b @L_8008263C
@L_80081150:
    li r3, 0x0
    b @L_80082640
    lwz r3, 0x0(r3)
    rlwinm r0, r6, 0, 24, 31
    stb r0, 0x8(r3)
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0xa
    bl fn_800F9E70
    b @L_8008263C
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x24(r3)
    blt @L_8008119C
    cmpwi r6, 0x6
    blt @L_8008263C
@L_8008119C:
    li r3, 0x0
    b @L_80082640
    lwz r3, 0x0(r3)
    rlwinm r0, r6, 0, 24, 31
    stb r0, 0x25(r3)
    b @L_8008263C
    subi r0, r6, 0x1
    lwz r4, 0x0(r3)
    extsb r0, r0
    stb r0, 0x26(r4)
    lwz r3, 0x0(r3)
    lbz r0, 0x26(r3)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_800811E0
    cmpwi r0, 0x5
    blt @L_8008263C
@L_800811E0:
    li r3, 0x0
    b @L_80082640
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x28
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x38
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x48
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    extsb r0, r6
    cmpwi r6, 0x1
    stb r0, 0x58(r3)
    blt @L_80081240
    cmpwi r6, 0x3
    ble @L_8008263C
@L_80081240:
    li r3, 0x0
    b @L_80082640
    lwz r3, 0x0(r3)
    extsb r0, r6
    cmpwi r6, 0x1
    stb r0, 0x59(r3)
    blt @L_80081264
    cmpwi r6, 0x6
    ble @L_8008263C
@L_80081264:
    li r3, 0x0
    b @L_80082640
    lwz r3, 0x0(r3)
    extsb r0, r6
    cmpwi r6, 0x1
    stb r0, 0x5a(r3)
    blt @L_80081288
    cmpwi r6, 0x5
    ble @L_8008263C
@L_80081288:
    li r3, 0x0
    b @L_80082640
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x5b(r3)
    blt @L_800812B0
    cmpwi r6, 0x9
    ble @L_8008263C
@L_800812B0:
    li r3, 0x0
    b @L_80082640
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x5c(r3)
    blt @L_800812D8
    cmpwi r6, 0x9
    ble @L_8008263C
@L_800812D8:
    li r3, 0x0
    b @L_80082640
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x5d(r3)
    blt @L_80081300
    cmpwi r6, 0x9
    ble @L_8008263C
@L_80081300:
    li r3, 0x0
    b @L_80082640
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x5e(r3)
    blt @L_80081328
    cmpwi r6, 0x9
    ble @L_8008263C
@L_80081328:
    li r3, 0x0
    b @L_80082640
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x5f(r3)
    blt @L_80081350
    cmpwi r6, 0x9
    ble @L_8008263C
@L_80081350:
    li r3, 0x0
    b @L_80082640
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x60(r3)
    blt @L_80081378
    cmpwi r6, 0x9
    ble @L_8008263C
@L_80081378:
    li r3, 0x0
    b @L_80082640
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x61(r3)
    blt @L_800813A0
    cmpwi r6, 0x9
    ble @L_8008263C
@L_800813A0:
    li r3, 0x0
    b @L_80082640
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x62(r3)
    blt @L_800813C8
    cmpwi r6, 0x9
    ble @L_8008263C
@L_800813C8:
    li r3, 0x0
    b @L_80082640
    subi r0, r6, 0x1
    lwz r3, 0x0(r3)
    extsb r0, r0
    cmpwi r6, 0x0
    stb r0, 0x63(r3)
    blt @L_800813F0
    cmpwi r6, 0x9
    ble @L_8008263C
@L_800813F0:
    li r3, 0x0
    b @L_80082640
    lwz r5, 0x0(r3)
    rlwinm r0, r6, 0, 16, 31
    addi r6, r4, 0x384
    li r4, 0x0
    sth r0, 0x64(r5)
    lwz r3, 0x0(r3)
    lhz r3, 0x64(r3)
    li r0, 0x2f
    mtctr r0
@L_8008141C:
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081430
    li r0, 0x1
    b @L_800814D0
@L_80081430:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081448
    li r0, 0x1
    b @L_800814D0
@L_80081448:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081460
    li r0, 0x1
    b @L_800814D0
@L_80081460:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081478
    li r0, 0x1
    b @L_800814D0
@L_80081478:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081490
    li r0, 0x1
    b @L_800814D0
@L_80081490:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_800814A8
    li r0, 0x1
    b @L_800814D0
@L_800814A8:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_800814C0
    li r0, 0x1
    b @L_800814D0
@L_800814C0:
    addi r6, r6, 0x2
    addi r4, r4, 0x6
    bdnz @L_8008141C
    li r0, 0x0
@L_800814D0:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    lwz r5, 0x0(r3)
    rlwinm r0, r6, 0, 16, 31
    addi r6, r4, 0x384
    li r4, 0x0
    sth r0, 0x66(r5)
    lwz r3, 0x0(r3)
    lhz r3, 0x66(r3)
    li r0, 0x2f
    mtctr r0
@L_80081508:
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_8008151C
    li r0, 0x1
    b @L_800815BC
@L_8008151C:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081534
    li r0, 0x1
    b @L_800815BC
@L_80081534:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_8008154C
    li r0, 0x1
    b @L_800815BC
@L_8008154C:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081564
    li r0, 0x1
    b @L_800815BC
@L_80081564:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_8008157C
    li r0, 0x1
    b @L_800815BC
@L_8008157C:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081594
    li r0, 0x1
    b @L_800815BC
@L_80081594:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_800815AC
    li r0, 0x1
    b @L_800815BC
@L_800815AC:
    addi r6, r6, 0x2
    addi r4, r4, 0x6
    bdnz @L_80081508
    li r0, 0x0
@L_800815BC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    lwz r5, 0x0(r3)
    rlwinm r0, r6, 0, 16, 31
    addi r6, r4, 0x384
    li r4, 0x0
    sth r0, 0x68(r5)
    lwz r3, 0x0(r3)
    lhz r3, 0x68(r3)
    li r0, 0x2f
    mtctr r0
@L_800815F4:
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081608
    li r0, 0x1
    b @L_800816A8
@L_80081608:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081620
    li r0, 0x1
    b @L_800816A8
@L_80081620:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081638
    li r0, 0x1
    b @L_800816A8
@L_80081638:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081650
    li r0, 0x1
    b @L_800816A8
@L_80081650:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081668
    li r0, 0x1
    b @L_800816A8
@L_80081668:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081680
    li r0, 0x1
    b @L_800816A8
@L_80081680:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r3, r0
    bne @L_80081698
    li r0, 0x1
    b @L_800816A8
@L_80081698:
    addi r6, r6, 0x2
    addi r4, r4, 0x6
    bdnz @L_800815F4
    li r0, 0x0
@L_800816A8:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    lwz r4, 0x0(r3)
    rlwinm r0, r6, 0, 24, 31
    stb r0, 0x6a(r4)
    lwz r3, 0x0(r3)
    lbz r0, 0x6a(r3)
    cmplwi r0, 0x24
    bgt @L_800816F8
    lis r3, jumptable_802EE9B8@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_802EE9B8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r0, 0x1
    b @L_800816FC
@L_800816F8:
    li r0, 0x0
@L_800816FC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    lwz r4, 0x0(r3)
    rlwinm r0, r6, 0, 24, 31
    stb r0, 0x6b(r4)
    lwz r3, 0x0(r3)
    lbz r0, 0x6b(r3)
    cmplwi r0, 0x24
    bgt @L_8008174C
    lis r3, jumptable_802EE924@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_802EE924@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r0, 0x1
    b @L_80081750
@L_8008174C:
    li r0, 0x0
@L_80081750:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    lwz r4, 0x0(r3)
    rlwinm r0, r6, 0, 24, 31
    stb r0, 0x6c(r4)
    lwz r3, 0x0(r3)
    lbz r0, 0x6c(r3)
    cmplwi r0, 0x24
    bgt @L_800817A0
    lis r3, jumptable_802EE890@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_802EE890@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r0, 0x1
    b @L_800817A4
@L_800817A0:
    li r0, 0x0
@L_800817A4:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x6e
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x182
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x296
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0xca
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x1de
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x2f2
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x126
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x23a
    bl fn_800F9E70
    b @L_8008263C
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0x34e
    bl fn_800F9E70
    b @L_8008263C
    mulli r5, r5, 0x28
    lwz r0, 0x0(r3)
    mr r4, r7
    addi r3, r5, 0x3ac
    add r3, r0, r3
    bl fn_800F9E70
    b @L_8008263C
    cmpwi r6, 0x1
    beq @L_800818B8
    bge @L_800818D0
    cmpwi r6, 0x0
    bge @L_800818A0
    b @L_800818D0
@L_800818A0:
    mulli r0, r5, 0x28
    lwz r3, 0x0(r3)
    li r4, 0x1
    add r3, r3, r0
    stb r4, 0x3b8(r3)
    b @L_8008263C
@L_800818B8:
    mulli r0, r5, 0x28
    lwz r3, 0x0(r3)
    li r4, 0x0
    add r3, r3, r0
    stb r4, 0x3b8(r3)
    b @L_8008263C
@L_800818D0:
    mulli r0, r5, 0x28
    lwz r4, 0x0(r3)
    li r5, 0x0
    li r3, 0x0
    add r4, r4, r0
    stb r5, 0x3b8(r4)
    b @L_80082640
    mulli r0, r5, 0x28
    lwz r3, 0x0(r3)
    subi r4, r6, 0x1
    cmpwi r6, 0x0
    add r0, r3, r0
    extsb r4, r4
    add r3, r0, r8
    stb r4, 0x3b9(r3)
    blt @L_80081918
    cmpwi r6, 0x24
    ble @L_8008263C
@L_80081918:
    li r3, 0x0
    b @L_80082640
    mulli r5, r5, 0x28
    lwz r3, 0x0(r3)
    rlwinm r0, r8, 1, 0, 30
    rlwinm r6, r6, 0, 16, 31
    add r3, r3, r5
    addi r5, r4, 0x384
    add r3, r3, r0
    li r4, 0x0
    sth r6, 0x3be(r3)
    li r0, 0x2f
    mtctr r0
@L_8008194C:
    lhz r0, 0x0(r5)
    cmplw r6, r0
    bne @L_80081960
    li r0, 0x1
    b @L_80081A00
@L_80081960:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r6, r0
    bne @L_80081978
    li r0, 0x1
    b @L_80081A00
@L_80081978:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r6, r0
    bne @L_80081990
    li r0, 0x1
    b @L_80081A00
@L_80081990:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r6, r0
    bne @L_800819A8
    li r0, 0x1
    b @L_80081A00
@L_800819A8:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r6, r0
    bne @L_800819C0
    li r0, 0x1
    b @L_80081A00
@L_800819C0:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r6, r0
    bne @L_800819D8
    li r0, 0x1
    b @L_80081A00
@L_800819D8:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r6, r0
    bne @L_800819F0
    li r0, 0x1
    b @L_80081A00
@L_800819F0:
    addi r5, r5, 0x2
    addi r4, r4, 0x6
    bdnz @L_8008194C
    li r0, 0x0
@L_80081A00:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    mulli r0, r5, 0x28
    lwz r3, 0x0(r3)
    add r3, r3, r0
    stw r6, 0x3c8(r3)
    b @L_8008263C
    mulli r0, r5, 0x28
    lwz r3, 0x0(r3)
    rlwinm r5, r6, 0, 16, 31
    addi r6, r4, 0x618
    add r3, r3, r0
    li r4, 0x0
    sth r5, 0x3cc(r3)
    li r0, 0x13
    mtctr r0
@L_80081A4C:
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081A60
    li r0, 0x1
    b @L_80081AB8
@L_80081A60:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081A78
    li r0, 0x1
    b @L_80081AB8
@L_80081A78:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081A90
    li r0, 0x1
    b @L_80081AB8
@L_80081A90:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081AA8
    li r0, 0x1
    b @L_80081AB8
@L_80081AA8:
    addi r6, r6, 0x2
    addi r4, r4, 0x3
    bdnz @L_80081A4C
    li r0, 0x0
@L_80081AB8:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    mulli r0, r5, 0x28
    lwz r4, 0x0(r3)
    rlwinm r7, r6, 0, 16, 31
    add r4, r4, r0
    sth r7, 0x3ce(r4)
    lwz r3, 0x0(r3)
    lbz r0, 0x5b(r3)
    extsb r0, r0
    cmpw r0, r5
    bne @L_80081AFC
    li r0, 0x1
    b @L_80081B30
@L_80081AFC:
    lbz r0, 0x5c(r3)
    extsb r0, r0
    cmpw r0, r5
    bne @L_80081B14
    li r0, 0x1
    b @L_80081B30
@L_80081B14:
    lbz r0, 0x5d(r3)
    extsb r0, r0
    cmpw r0, r5
    bne @L_80081B2C
    li r0, 0x1
    b @L_80081B30
@L_80081B2C:
    li r0, 0x0
@L_80081B30:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008263C
    rlwinm r0, r6, 0, 16, 31
    cmplwi r0, 0x3e7
    ble @L_8008263C
    li r3, 0x0
    b @L_80082640
    mulli r0, r5, 0x28
    lwz r3, 0x0(r3)
    rlwinm r5, r6, 0, 24, 31
    addi r6, r4, 0x6b0
    add r3, r3, r0
    li r4, 0x0
    stb r5, 0x3d0(r3)
    li r0, 0x4
    mtctr r0
@L_80081B74:
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081B88
    li r0, 0x1
    b @L_80081C40
@L_80081B88:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081BA0
    li r0, 0x1
    b @L_80081C40
@L_80081BA0:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081BB8
    li r0, 0x1
    b @L_80081C40
@L_80081BB8:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081BD0
    li r0, 0x1
    b @L_80081C40
@L_80081BD0:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081BE8
    li r0, 0x1
    b @L_80081C40
@L_80081BE8:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081C00
    li r0, 0x1
    b @L_80081C40
@L_80081C00:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081C18
    li r0, 0x1
    b @L_80081C40
@L_80081C18:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081C30
    li r0, 0x1
    b @L_80081C40
@L_80081C30:
    addi r6, r6, 0x2
    addi r4, r4, 0x7
    bdnz @L_80081B74
    li r0, 0x0
@L_80081C40:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    rlwinm r5, r6, 0, 16, 31
    addi r7, r4, 0x6f0
    add r4, r3, r0
    rlwinm r3, r6, 0, 24, 31
    sth r5, 0x514(r4)
    li r4, 0x0
    li r0, 0x2b
    mtctr r0
@L_80081C7C:
    lhz r0, 0x0(r7)
    cmplw r3, r0
    bne @L_80081C90
    li r0, 0x1
    b @L_80081D60
@L_80081C90:
    addi r7, r7, 0x2
    lhz r0, 0x0(r7)
    cmplw r3, r0
    bne @L_80081CA8
    li r0, 0x1
    b @L_80081D60
@L_80081CA8:
    addi r7, r7, 0x2
    lhz r0, 0x0(r7)
    cmplw r3, r0
    bne @L_80081CC0
    li r0, 0x1
    b @L_80081D60
@L_80081CC0:
    addi r7, r7, 0x2
    lhz r0, 0x0(r7)
    cmplw r3, r0
    bne @L_80081CD8
    li r0, 0x1
    b @L_80081D60
@L_80081CD8:
    addi r7, r7, 0x2
    lhz r0, 0x0(r7)
    cmplw r3, r0
    bne @L_80081CF0
    li r0, 0x1
    b @L_80081D60
@L_80081CF0:
    addi r7, r7, 0x2
    lhz r0, 0x0(r7)
    cmplw r3, r0
    bne @L_80081D08
    li r0, 0x1
    b @L_80081D60
@L_80081D08:
    addi r7, r7, 0x2
    lhz r0, 0x0(r7)
    cmplw r3, r0
    bne @L_80081D20
    li r0, 0x1
    b @L_80081D60
@L_80081D20:
    addi r7, r7, 0x2
    lhz r0, 0x0(r7)
    cmplw r3, r0
    bne @L_80081D38
    li r0, 0x1
    b @L_80081D60
@L_80081D38:
    addi r7, r7, 0x2
    lhz r0, 0x0(r7)
    cmplw r3, r0
    bne @L_80081D50
    li r0, 0x1
    b @L_80081D60
@L_80081D50:
    addi r7, r7, 0x2
    addi r4, r4, 0x8
    bdnz @L_80081C7C
    li r0, 0x0
@L_80081D60:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    rlwinm r5, r6, 0, 24, 31
    addi r6, r4, 0x9f8
    add r3, r3, r0
    li r4, 0x0
    stb r5, 0x516(r3)
    li r0, 0x3
    mtctr r0
@L_80081D98:
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081DAC
    li r0, 0x1
    b @L_80081E94
@L_80081DAC:
    addi r6, r6, 0x1
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081DC4
    li r0, 0x1
    b @L_80081E94
@L_80081DC4:
    addi r6, r6, 0x1
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081DDC
    li r0, 0x1
    b @L_80081E94
@L_80081DDC:
    addi r6, r6, 0x1
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081DF4
    li r0, 0x1
    b @L_80081E94
@L_80081DF4:
    addi r6, r6, 0x1
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081E0C
    li r0, 0x1
    b @L_80081E94
@L_80081E0C:
    addi r6, r6, 0x1
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081E24
    li r0, 0x1
    b @L_80081E94
@L_80081E24:
    addi r6, r6, 0x1
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081E3C
    li r0, 0x1
    b @L_80081E94
@L_80081E3C:
    addi r6, r6, 0x1
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081E54
    li r0, 0x1
    b @L_80081E94
@L_80081E54:
    addi r6, r6, 0x1
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081E6C
    li r0, 0x1
    b @L_80081E94
@L_80081E6C:
    addi r6, r6, 0x1
    lbz r0, 0x0(r6)
    cmplw r0, r5
    bne @L_80081E84
    li r0, 0x1
    b @L_80081E94
@L_80081E84:
    addi r6, r6, 0x1
    addi r4, r4, 0x9
    bdnz @L_80081D98
    li r0, 0x0
@L_80081E94:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    rlwinm r4, r6, 0, 24, 31
    add r3, r3, r0
    stb r4, 0x517(r3)
    b @L_8008263C
    mulli r5, r5, 0x2a
    lwz r3, 0x0(r3)
    rlwinm r0, r8, 1, 0, 30
    rlwinm r7, r6, 0, 16, 31
    add r3, r3, r5
    addi r5, r4, 0xa18
    add r4, r3, r0
    rlwinm r3, r6, 0, 24, 31
    sth r7, 0x518(r4)
    li r4, 0x0
    li r0, 0x47
    mtctr r0
@L_80081EF0:
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_80081F04
    li r0, 0x1
    b @L_80081F74
@L_80081F04:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_80081F1C
    li r0, 0x1
    b @L_80081F74
@L_80081F1C:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_80081F34
    li r0, 0x1
    b @L_80081F74
@L_80081F34:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_80081F4C
    li r0, 0x1
    b @L_80081F74
@L_80081F4C:
    addi r5, r5, 0x2
    lhz r0, 0x0(r5)
    cmplw r3, r0
    bne @L_80081F64
    li r0, 0x1
    b @L_80081F74
@L_80081F64:
    addi r5, r5, 0x2
    addi r4, r4, 0x4
    bdnz @L_80081EF0
    li r0, 0x0
@L_80081F74:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    rlwinm r5, r6, 0, 16, 31
    addi r6, r4, 0x384
    add r3, r3, r0
    li r4, 0x0
    sth r5, 0x520(r3)
    li r0, 0x2f
    mtctr r0
@L_80081FAC:
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081FC0
    li r0, 0x1
    b @L_80082060
@L_80081FC0:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081FD8
    li r0, 0x1
    b @L_80082060
@L_80081FD8:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80081FF0
    li r0, 0x1
    b @L_80082060
@L_80081FF0:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80082008
    li r0, 0x1
    b @L_80082060
@L_80082008:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80082020
    li r0, 0x1
    b @L_80082060
@L_80082020:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80082038
    li r0, 0x1
    b @L_80082060
@L_80082038:
    addi r6, r6, 0x2
    lhz r0, 0x0(r6)
    cmplw r5, r0
    bne @L_80082050
    li r0, 0x1
    b @L_80082060
@L_80082050:
    addi r6, r6, 0x2
    addi r4, r4, 0x6
    bdnz @L_80081FAC
    li r0, 0x0
@L_80082060:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    cmpwi r6, 0x2
    bge @L_800820A0
    cmpwi r6, 0x0
    bge @L_80082088
    b @L_800820A0
@L_80082088:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    extsb r4, r6
    add r3, r3, r0
    stb r4, 0x522(r3)
    b @L_8008263C
@L_800820A0:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    li r4, -0x1
    add r3, r3, r0
    stb r4, 0x522(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_800820D0
    cmpwi r6, 0x1f
    bgt @L_800820D0
    li r0, 0x1
@L_800820D0:
    cmpwi r0, 0x0
    beq @L_800820E0
    extsb r4, r6
    b @L_800820E4
@L_800820E0:
    li r4, -0x1
@L_800820E4:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    stb r4, 0x523(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_80082110
    cmpwi r6, 0x1f
    bgt @L_80082110
    li r0, 0x1
@L_80082110:
    cmpwi r0, 0x0
    beq @L_80082120
    extsb r4, r6
    b @L_80082124
@L_80082120:
    li r4, -0x1
@L_80082124:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    stb r4, 0x524(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_80082150
    cmpwi r6, 0x1f
    bgt @L_80082150
    li r0, 0x1
@L_80082150:
    cmpwi r0, 0x0
    beq @L_80082160
    extsb r4, r6
    b @L_80082164
@L_80082160:
    li r4, -0x1
@L_80082164:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    stb r4, 0x525(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_80082190
    cmpwi r6, 0x1f
    bgt @L_80082190
    li r0, 0x1
@L_80082190:
    cmpwi r0, 0x0
    beq @L_800821A0
    extsb r4, r6
    b @L_800821A4
@L_800821A0:
    li r4, -0x1
@L_800821A4:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    stb r4, 0x526(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_800821D0
    cmpwi r6, 0x1f
    bgt @L_800821D0
    li r0, 0x1
@L_800821D0:
    cmpwi r0, 0x0
    beq @L_800821E0
    extsb r4, r6
    b @L_800821E4
@L_800821E0:
    li r4, -0x1
@L_800821E4:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    stb r4, 0x527(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_80082210
    cmpwi r6, 0x1f
    bgt @L_80082210
    li r0, 0x1
@L_80082210:
    cmpwi r0, 0x0
    beq @L_80082220
    extsb r4, r6
    b @L_80082224
@L_80082220:
    li r4, -0x1
@L_80082224:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    stb r4, 0x528(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_80082250
    cmpwi r6, 0xff
    bgt @L_80082250
    li r0, 0x1
@L_80082250:
    cmpwi r0, 0x0
    beq @L_80082260
    extsh r4, r6
    b @L_80082264
@L_80082260:
    li r4, -0x1
@L_80082264:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    sth r4, 0x52a(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_80082290
    cmpwi r6, 0xff
    bgt @L_80082290
    li r0, 0x1
@L_80082290:
    cmpwi r0, 0x0
    beq @L_800822A0
    extsh r4, r6
    b @L_800822A4
@L_800822A0:
    li r4, -0x1
@L_800822A4:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    sth r4, 0x52c(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_800822D0
    cmpwi r6, 0xff
    bgt @L_800822D0
    li r0, 0x1
@L_800822D0:
    cmpwi r0, 0x0
    beq @L_800822E0
    extsh r4, r6
    b @L_800822E4
@L_800822E0:
    li r4, -0x1
@L_800822E4:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    sth r4, 0x52e(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_80082310
    cmpwi r6, 0xff
    bgt @L_80082310
    li r0, 0x1
@L_80082310:
    cmpwi r0, 0x0
    beq @L_80082320
    extsh r4, r6
    b @L_80082324
@L_80082320:
    li r4, -0x1
@L_80082324:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    sth r4, 0x530(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_80082350
    cmpwi r6, 0xff
    bgt @L_80082350
    li r0, 0x1
@L_80082350:
    cmpwi r0, 0x0
    beq @L_80082360
    extsh r4, r6
    b @L_80082364
@L_80082360:
    li r4, -0x1
@L_80082364:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    sth r4, 0x532(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_80082390
    cmpwi r6, 0xff
    bgt @L_80082390
    li r0, 0x1
@L_80082390:
    cmpwi r0, 0x0
    beq @L_800823A0
    extsh r4, r6
    b @L_800823A4
@L_800823A0:
    li r4, -0x1
@L_800823A4:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    sth r4, 0x534(r3)
    b @L_8008263C
    cmpwi r6, 0x0
    li r0, 0x0
    blt @L_800823D0
    cmpwi r6, 0xff
    bgt @L_800823D0
    li r0, 0x1
@L_800823D0:
    cmpwi r0, 0x0
    beq @L_800823E0
    extsh r4, r6
    b @L_800823E4
@L_800823E0:
    li r4, -0x1
@L_800823E4:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    add r3, r3, r0
    sth r4, 0x536(r3)
    b @L_8008263C
    cmpwi r6, 0x2
    beq @L_80082450
    bge @L_80082414
    cmpwi r6, 0x0
    beq @L_80082420
    bge @L_80082438
    b @L_80082420
@L_80082414:
    cmpwi r6, 0x4
    bge @L_80082420
    b @L_80082468
@L_80082420:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    li r4, 0x0
    add r3, r3, r0
    stb r4, 0x538(r3)
    b @L_8008263C
@L_80082438:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    li r4, 0x0
    add r3, r3, r0
    stb r4, 0x538(r3)
    b @L_8008263C
@L_80082450:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    li r4, 0x1
    add r3, r3, r0
    stb r4, 0x538(r3)
    b @L_8008263C
@L_80082468:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    li r4, 0x2
    add r3, r3, r0
    stb r4, 0x538(r3)
    b @L_8008263C
    rlwinm r0, r6, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_800824A4
    mulli r0, r5, 0x2a
    lwz r6, 0x0(r3)
    li r7, -0x1
    add r6, r6, r0
    stb r7, 0x539(r6)
    b @L_800824BC
@L_800824A4:
    mulli r0, r5, 0x2a
    lwz r7, 0x0(r3)
    subi r6, r6, 0x1
    extsb r8, r6
    add r6, r7, r0
    stb r8, 0x539(r6)
@L_800824BC:
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    addi r5, r4, 0xce0
    li r4, 0x0
    add r3, r3, r0
    lbz r3, 0x539(r3)
    li r0, 0xd
    extsb r3, r3
    mtctr r0
@L_800824E0:
    lbz r0, 0x0(r5)
    extsb r0, r0
    cmpw r0, r3
    bne @L_800824F8
    li r0, 0x1
    b @L_80082524
@L_800824F8:
    addi r5, r5, 0x1
    lbz r0, 0x0(r5)
    extsb r0, r0
    cmpw r0, r3
    bne @L_80082514
    li r0, 0x1
    b @L_80082524
@L_80082514:
    addi r5, r5, 0x1
    addi r4, r4, 0x1
    bdnz @L_800824E0
    li r0, 0x0
@L_80082524:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    mulli r7, r5, 0x2a
    lwz r0, 0x0(r3)
    rlwinm r5, r6, 0, 24, 31
    add r4, r0, r7
    stb r5, 0x53a(r4)
    lwz r0, 0x0(r3)
    add r3, r0, r7
    lbz r0, 0x53a(r3)
    cmpwi r0, 0x4
    bge @L_80082568
    cmpwi r0, 0x0
    bge @L_8008263C
@L_80082568:
    li r3, 0x0
    b @L_80082640
    mulli r7, r5, 0x2a
    lwz r0, 0x0(r3)
    rlwinm r5, r6, 0, 24, 31
    add r4, r0, r7
    stb r5, 0x53b(r4)
    lwz r0, 0x0(r3)
    add r4, r0, r7
    lbz r0, 0x53b(r4)
    cmpwi r0, 0x5
    bge @L_800825A0
    cmpwi r0, 0x0
    bge @L_8008263C
@L_800825A0:
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x53b(r4)
    b @L_80082640
    mulli r0, r5, 0x2a
    lwz r3, 0x0(r3)
    rlwinm r4, r6, 0, 24, 31
    add r3, r3, r0
    stb r4, 0x53c(r3)
    b @L_8008263C
    lwz r3, 0x0(r3)
    addi r4, r4, 0xcfc
    stw r6, 0xafc(r3)
    li r0, 0x2b
    mtctr r0
@L_800825DC:
    lwz r0, 0x0(r4)
    cmplw r6, r0
    bne @L_800825F0
    li r0, 0x1
    b @L_800825FC
@L_800825F0:
    addi r4, r4, 0x4
    bdnz @L_800825DC
    li r0, 0x0
@L_800825FC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8008263C
    li r3, 0x0
    b @L_80082640
    lwz r3, 0x0(r3)
    mr r4, r7
    addi r3, r3, 0xb00
    bl fn_800F9E70
    b @L_8008263C
@L_80082624:
    addi r3, r4, 0x63e8
    li r4, 0x8c5
    la r5, lbl_8047C178(r2)
    bl fn_80196E10
    li r3, 0x0
    b @L_80082640
@L_8008263C:
    li r3, 0x1
@L_80082640:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

