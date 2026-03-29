    nofralloc
    stwu r1, -0x40(r1)
    lwz r0, 0x8(r3)
    cmpwi r0, 0x1
    beq @L_800F9154
    bge @L_800F8A74
    cmpwi r0, 0x0
    bge @L_800F8A80
    b @L_800F9154
@L_800F8A74:
    cmpwi r0, 0x3
    bge @L_800F9154
    b @L_800F8AD4
@L_800F8A80:
    lfs f0, 0x48(r3)
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    stb r0, 0x58(r3)
    lfs f0, 0x4c(r3)
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r0, 0x24(r1)
    stb r0, 0x59(r3)
    lfs f0, 0x50(r3)
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    stb r0, 0x5a(r3)
    lfs f0, 0x54(r3)
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    stb r0, 0x5b(r3)
    b @L_800F9154
@L_800F8AD4:
    lbz r6, 0x26(r3)
    lbz r5, 0x27(r3)
    extsb r6, r6
    cmpwi r6, -0xa
    extsb r5, r5
    ble @L_800F8AFC
    cmpwi r6, 0xa
    bge @L_800F8AFC
    li r6, 0x0
    b @L_800F8B10
@L_800F8AFC:
    cmpwi r6, 0x0
    ble @L_800F8B0C
    subi r6, r6, 0xa
    b @L_800F8B10
@L_800F8B0C:
    addi r6, r6, 0xa
@L_800F8B10:
    cmpwi r5, -0xa
    ble @L_800F8B28
    cmpwi r5, 0xa
    bge @L_800F8B28
    li r5, 0x0
    b @L_800F8B3C
@L_800F8B28:
    cmpwi r5, 0x0
    ble @L_800F8B38
    subi r5, r5, 0xa
    b @L_800F8B3C
@L_800F8B38:
    addi r5, r5, 0xa
@L_800F8B3C:
    mullw r4, r6, r6
    mullw r0, r5, r5
    add r0, r4, r0
    cmpwi r0, 0xc40
    ble @L_800F8C60
    xoris r4, r0, 0x8000
    lis r0, 0x4330
    stw r4, 0x34(r1)
    lfd f2, lbl_8047CCC8(r2)
    stw r0, 0x30(r1)
    lfs f0, lbl_8047CCD0(r2)
    lfd f1, 0x30(r1)
    fsubs f4, f1, f2
    fcmpo cr0, f4, f0
    ble @L_800F8BC0
    frsqrte f1, f4
    lfd f3, lbl_8047CCE8(r2)
    lfd f2, lbl_8047CCF0(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_800F8C44
@L_800F8BC0:
    lfd f0, lbl_8047CCF8(r2)
    fcmpo cr0, f4, f0
    bge @L_800F8BD8
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
    b @L_800F8C44
@L_800F8BD8:
    stfs f4, 0x14(r1)
    lis r0, 0x7f80
    lwz r7, 0x14(r1)
    rlwinm r4, r7, 0, 1, 8
    cmpw r4, r0
    beq @L_800F8C00
    bge @L_800F8C30
    cmpwi r4, 0x0
    beq @L_800F8C18
    b @L_800F8C30
@L_800F8C00:
    clrlwi. r0, r7, 9
    beq @L_800F8C10
    li r0, 0x1
    b @L_800F8C34
@L_800F8C10:
    li r0, 0x2
    b @L_800F8C34
@L_800F8C18:
    clrlwi. r0, r7, 9
    beq @L_800F8C28
    li r0, 0x5
    b @L_800F8C34
@L_800F8C28:
    li r0, 0x3
    b @L_800F8C34
@L_800F8C30:
    li r0, 0x4
@L_800F8C34:
    cmpwi r0, 0x1
    bne @L_800F8C44
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
@L_800F8C44:
    fctiwz f0, f4
    mulli r4, r6, 0x38
    stfd f0, 0x30(r1)
    mulli r0, r5, 0x38
    lwz r5, 0x34(r1)
    divw r6, r4, r5
    divw r5, r0, r5
@L_800F8C60:
    stb r6, 0x26(r3)
    stb r5, 0x27(r3)
    lbz r6, 0x28(r3)
    lbz r5, 0x29(r3)
    extsb r6, r6
    cmpwi r6, -0xa
    extsb r5, r5
    ble @L_800F8C90
    cmpwi r6, 0xa
    bge @L_800F8C90
    li r6, 0x0
    b @L_800F8CA4
@L_800F8C90:
    cmpwi r6, 0x0
    ble @L_800F8CA0
    subi r6, r6, 0xa
    b @L_800F8CA4
@L_800F8CA0:
    addi r6, r6, 0xa
@L_800F8CA4:
    cmpwi r5, -0xa
    ble @L_800F8CBC
    cmpwi r5, 0xa
    bge @L_800F8CBC
    li r5, 0x0
    b @L_800F8CD0
@L_800F8CBC:
    cmpwi r5, 0x0
    ble @L_800F8CCC
    subi r5, r5, 0xa
    b @L_800F8CD0
@L_800F8CCC:
    addi r5, r5, 0xa
@L_800F8CD0:
    mullw r4, r6, r6
    mullw r0, r5, r5
    add r0, r4, r0
    cmpwi r0, 0x790
    ble @L_800F8DF4
    xoris r4, r0, 0x8000
    lis r0, 0x4330
    stw r4, 0x34(r1)
    lfd f2, lbl_8047CCC8(r2)
    stw r0, 0x30(r1)
    lfs f0, lbl_8047CCD0(r2)
    lfd f1, 0x30(r1)
    fsubs f4, f1, f2
    fcmpo cr0, f4, f0
    ble @L_800F8D54
    frsqrte f1, f4
    lfd f3, lbl_8047CCE8(r2)
    lfd f2, lbl_8047CCF0(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_800F8DD8
@L_800F8D54:
    lfd f0, lbl_8047CCF8(r2)
    fcmpo cr0, f4, f0
    bge @L_800F8D6C
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
    b @L_800F8DD8
@L_800F8D6C:
    stfs f4, 0x10(r1)
    lis r0, 0x7f80
    lwz r7, 0x10(r1)
    rlwinm r4, r7, 0, 1, 8
    cmpw r4, r0
    beq @L_800F8D94
    bge @L_800F8DC4
    cmpwi r4, 0x0
    beq @L_800F8DAC
    b @L_800F8DC4
@L_800F8D94:
    clrlwi. r0, r7, 9
    beq @L_800F8DA4
    li r0, 0x1
    b @L_800F8DC8
@L_800F8DA4:
    li r0, 0x2
    b @L_800F8DC8
@L_800F8DAC:
    clrlwi. r0, r7, 9
    beq @L_800F8DBC
    li r0, 0x5
    b @L_800F8DC8
@L_800F8DBC:
    li r0, 0x3
    b @L_800F8DC8
@L_800F8DC4:
    li r0, 0x4
@L_800F8DC8:
    cmpwi r0, 0x1
    bne @L_800F8DD8
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
@L_800F8DD8:
    fctiwz f0, f4
    mulli r4, r6, 0x2c
    stfd f0, 0x30(r1)
    mulli r0, r5, 0x2c
    lwz r5, 0x34(r1)
    divw r6, r4, r5
    divw r5, r0, r5
@L_800F8DF4:
    stb r6, 0x28(r3)
    stb r5, 0x29(r3)
    lfs f1, 0x48(r3)
    lfs f0, 0x4c(r3)
    fctiwz f1, f1
    fctiwz f0, f0
    stfd f1, 0x30(r1)
    lwz r0, 0x34(r1)
    stfd f0, 0x28(r1)
    extsb r6, r0
    lwz r0, 0x2c(r1)
    cmpwi r6, -0xa
    extsb r5, r0
    ble @L_800F8E3C
    cmpwi r6, 0xa
    bge @L_800F8E3C
    li r6, 0x0
    b @L_800F8E50
@L_800F8E3C:
    cmpwi r6, 0x0
    ble @L_800F8E4C
    subi r6, r6, 0xa
    b @L_800F8E50
@L_800F8E4C:
    addi r6, r6, 0xa
@L_800F8E50:
    cmpwi r5, -0xa
    ble @L_800F8E68
    cmpwi r5, 0xa
    bge @L_800F8E68
    li r5, 0x0
    b @L_800F8E7C
@L_800F8E68:
    cmpwi r5, 0x0
    ble @L_800F8E78
    subi r5, r5, 0xa
    b @L_800F8E7C
@L_800F8E78:
    addi r5, r5, 0xa
@L_800F8E7C:
    mullw r4, r6, r6
    mullw r0, r5, r5
    add r0, r4, r0
    cmpwi r0, 0xc40
    ble @L_800F8FA0
    xoris r4, r0, 0x8000
    lis r0, 0x4330
    stw r4, 0x34(r1)
    lfd f2, lbl_8047CCC8(r2)
    stw r0, 0x30(r1)
    lfs f0, lbl_8047CCD0(r2)
    lfd f1, 0x30(r1)
    fsubs f4, f1, f2
    fcmpo cr0, f4, f0
    ble @L_800F8F00
    frsqrte f1, f4
    lfd f3, lbl_8047CCE8(r2)
    lfd f2, lbl_8047CCF0(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_800F8F84
@L_800F8F00:
    lfd f0, lbl_8047CCF8(r2)
    fcmpo cr0, f4, f0
    bge @L_800F8F18
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
    b @L_800F8F84
@L_800F8F18:
    stfs f4, 0xc(r1)
    lis r0, 0x7f80
    lwz r7, 0xc(r1)
    rlwinm r4, r7, 0, 1, 8
    cmpw r4, r0
    beq @L_800F8F40
    bge @L_800F8F70
    cmpwi r4, 0x0
    beq @L_800F8F58
    b @L_800F8F70
@L_800F8F40:
    clrlwi. r0, r7, 9
    beq @L_800F8F50
    li r0, 0x1
    b @L_800F8F74
@L_800F8F50:
    li r0, 0x2
    b @L_800F8F74
@L_800F8F58:
    clrlwi. r0, r7, 9
    beq @L_800F8F68
    li r0, 0x5
    b @L_800F8F74
@L_800F8F68:
    li r0, 0x3
    b @L_800F8F74
@L_800F8F70:
    li r0, 0x4
@L_800F8F74:
    cmpwi r0, 0x1
    bne @L_800F8F84
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
@L_800F8F84:
    fctiwz f0, f4
    mulli r4, r6, 0x38
    stfd f0, 0x30(r1)
    mulli r0, r5, 0x38
    lwz r5, 0x34(r1)
    divw r6, r4, r5
    divw r5, r0, r5
@L_800F8FA0:
    stb r6, 0x58(r3)
    stb r5, 0x59(r3)
    lfs f1, 0x50(r3)
    lfs f0, 0x54(r3)
    fctiwz f1, f1
    fctiwz f0, f0
    stfd f1, 0x30(r1)
    lwz r0, 0x34(r1)
    stfd f0, 0x28(r1)
    extsb r6, r0
    lwz r0, 0x2c(r1)
    cmpwi r6, -0xa
    extsb r5, r0
    ble @L_800F8FE8
    cmpwi r6, 0xa
    bge @L_800F8FE8
    li r6, 0x0
    b @L_800F8FFC
@L_800F8FE8:
    cmpwi r6, 0x0
    ble @L_800F8FF8
    subi r6, r6, 0xa
    b @L_800F8FFC
@L_800F8FF8:
    addi r6, r6, 0xa
@L_800F8FFC:
    cmpwi r5, -0xa
    ble @L_800F9014
    cmpwi r5, 0xa
    bge @L_800F9014
    li r5, 0x0
    b @L_800F9028
@L_800F9014:
    cmpwi r5, 0x0
    ble @L_800F9024
    subi r5, r5, 0xa
    b @L_800F9028
@L_800F9024:
    addi r5, r5, 0xa
@L_800F9028:
    mullw r4, r6, r6
    mullw r0, r5, r5
    add r0, r4, r0
    cmpwi r0, 0x790
    ble @L_800F914C
    xoris r4, r0, 0x8000
    lis r0, 0x4330
    stw r4, 0x34(r1)
    lfd f2, lbl_8047CCC8(r2)
    stw r0, 0x30(r1)
    lfs f0, lbl_8047CCD0(r2)
    lfd f1, 0x30(r1)
    fsubs f4, f1, f2
    fcmpo cr0, f4, f0
    ble @L_800F90AC
    frsqrte f1, f4
    lfd f3, lbl_8047CCE8(r2)
    lfd f2, lbl_8047CCF0(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_800F9130
@L_800F90AC:
    lfd f0, lbl_8047CCF8(r2)
    fcmpo cr0, f4, f0
    bge @L_800F90C4
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
    b @L_800F9130
@L_800F90C4:
    stfs f4, 0x8(r1)
    lis r0, 0x7f80
    lwz r7, 0x8(r1)
    rlwinm r4, r7, 0, 1, 8
    cmpw r4, r0
    beq @L_800F90EC
    bge @L_800F911C
    cmpwi r4, 0x0
    beq @L_800F9104
    b @L_800F911C
@L_800F90EC:
    clrlwi. r0, r7, 9
    beq @L_800F90FC
    li r0, 0x1
    b @L_800F9120
@L_800F90FC:
    li r0, 0x2
    b @L_800F9120
@L_800F9104:
    clrlwi. r0, r7, 9
    beq @L_800F9114
    li r0, 0x5
    b @L_800F9120
@L_800F9114:
    li r0, 0x3
    b @L_800F9120
@L_800F911C:
    li r0, 0x4
@L_800F9120:
    cmpwi r0, 0x1
    bne @L_800F9130
    lis r4, lbl_80478AC0@ha
    lfs f4, lbl_80478AC0@l(r4)
@L_800F9130:
    fctiwz f0, f4
    mulli r4, r6, 0x2c
    stfd f0, 0x30(r1)
    mulli r0, r5, 0x2c
    lwz r5, 0x34(r1)
    divw r6, r4, r5
    divw r5, r0, r5
@L_800F914C:
    stb r6, 0x5a(r3)
    stb r5, 0x5b(r3)
@L_800F9154:
    addi r1, r1, 0x40
    blr
