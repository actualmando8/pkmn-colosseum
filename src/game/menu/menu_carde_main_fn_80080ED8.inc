    nofralloc
    cmplwi r4, 0x0
    li r5, 0x0
    bne @L_80080EEC
    li r3, 0x0
    blr
@L_80080EEC:
    cmplwi r3, 0x0
    bne @L_80081010
    b @L_80080F34
@L_80080EF8:
    cmplwi r3, 0x81
    blt @L_80080F0C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x9f
    ble @L_80080F20
@L_80080F0C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0xe0
    blt @L_80080F2C
    cmplwi r0, 0xfc
    bgt @L_80080F2C
@L_80080F20:
    addi r4, r4, 0x2
    addi r5, r5, 0x2
    b @L_80080F34
@L_80080F2C:
    addi r4, r4, 0x1
    addi r5, r5, 0x1
@L_80080F34:
    lbz r3, 0x0(r4)
    cmplwi r3, 0x0
    bne @L_80080EF8
    b @L_80081024
    b @L_80081010
@L_80080F48:
    cmplwi r6, 0x81
    blt @L_80080F5C
    rlwinm r0, r6, 0, 24, 31
    cmplwi r0, 0x9f
    ble @L_80080F70
@L_80080F5C:
    rlwinm r0, r6, 0, 24, 31
    cmplwi r0, 0xe0
    blt @L_80080FDC
    cmplwi r0, 0xfc
    bgt @L_80080FDC
@L_80080F70:
    cmplwi r6, 0x81
    blt @L_80080FAC
    cmplwi r6, 0x9f
    bgt @L_80080FAC
    mulli r7, r6, 0x170
    lbz r0, 0x1(r4)
    lis r6, lbl_80269B68@ha
    rlwinm r0, r0, 1, 0, 30
    addi r6, r6, lbl_80269B68@l
    add r6, r6, r7
    add r6, r6, r0
    subis r6, r6, 0x1
    lhz r0, 0x4610(r6)
    mr r6, r0
    b @L_80080FD4
@L_80080FAC:
    mulli r7, r6, 0x170
    lbz r0, 0x1(r4)
    lis r6, lbl_8026C7F8@ha
    rlwinm r0, r0, 1, 0, 30
    addi r6, r6, lbl_8026C7F8@l
    add r6, r6, r7
    add r6, r6, r0
    subis r6, r6, 0x1
    lhz r0, -0x4280(r6)
    mr r6, r0
@L_80080FD4:
    li r0, 0x2
    b @L_80081000
@L_80080FDC:
    rlwinm r0, r6, 0, 24, 31
    cmplwi r0, 0xa1
    blt @L_80080FFC
    cmplwi r0, 0xdf
    bgt @L_80080FFC
    addis r6, r6, 0x1
    subi r0, r6, 0x140
    rlwinm r6, r0, 0, 16, 31
@L_80080FFC:
    li r0, 0x1
@L_80081000:
    sth r6, 0x0(r3)
    addi r3, r3, 0x2
    add r4, r4, r0
    add r5, r5, r0
@L_80081010:
    lbz r6, 0x0(r4)
    cmplwi r6, 0x0
    bne @L_80080F48
    li r0, 0x0
    sth r0, 0x0(r3)
@L_80081024:
    mr r3, r5
    blr
