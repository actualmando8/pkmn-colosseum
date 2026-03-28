    nofralloc
    lis r3, lbl_803A9A60@ha
    li r5, 0x0
    addi r6, r3, lbl_803A9A60@l
    mr r4, r6
    lfs f3, lbl_8047BF60(r2)
@L_80060A3C:
    addi r3, r4, 0x58
    li r7, 0x0
    li r0, 0x6
    mtctr r0
@L_80060A4C:
    lfs f1, 0x24(r3)
    fcmpu cr0, f3, f1
    beq @L_80060A78
    lfs f0, 0x3c(r6)
    fsubs f0, f1, f0
    stfs f0, 0x24(r3)
    lfs f0, 0x24(r3)
    fcmpo cr0, f0, f3
    bge @L_80060B34
    stfs f3, 0x24(r3)
    b @L_80060B34
@L_80060A78:
    lfs f0, 0x3c(r3)
    lfs f1, 0x54(r3)
    fcmpu cr0, f0, f1
    beq @L_80060B34
    fsubs f4, f1, f0
    lfs f2, lbl_8047BF94(r2)
    lfs f1, 0x3c(r6)
    lfs f0, lbl_8047BF98(r2)
    fmuls f2, f2, f4
    fmuls f4, f2, f1
    fcmpo cr0, f4, f0
    ble @L_80060AAC
    fmr f4, f0
@L_80060AAC:
    lfs f0, lbl_8047BF9C(r2)
    fcmpo cr0, f4, f0
    cror eq, lt, eq
    bne @L_80060AC0
    fmr f4, f0
@L_80060AC0:
    lfs f1, 0x3c(r3)
    lfs f0, lbl_8047BF60(r2)
    fadds f1, f1, f4
    fcmpo cr0, f4, f0
    stfs f1, 0x3c(r3)
    lfs f2, 0x54(r3)
    lfs f0, 0x3c(r3)
    fsubs f1, f2, f0
    ble @L_80060AE8
    b @L_80060AEC
@L_80060AE8:
    fneg f4, f4
@L_80060AEC:
    lfs f0, lbl_8047BF60(r2)
    fcmpo cr0, f1, f0
    ble @L_80060B00
    fmr f0, f1
    b @L_80060B04
@L_80060B00:
    fneg f0, f1
@L_80060B04:
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    beq @L_80060B30
    lfs f0, lbl_8047BF60(r2)
    fcmpo cr0, f1, f0
    ble @L_80060B20
    b @L_80060B24
@L_80060B20:
    fneg f1, f1
@L_80060B24:
    lfs f0, lbl_8047BF90(r2)
    fcmpo cr0, f1, f0
    bge @L_80060B34
@L_80060B30:
    stfs f2, 0x3c(r3)
@L_80060B34:
    addi r3, r3, 0x4
    addi r7, r7, 0x1
    bdnz @L_80060A4C
    addi r4, r4, 0xb4
    addi r5, r5, 0x1
    cmpwi r5, 0x4
    blt @L_80060A3C
    mr r3, r6
    li r0, 0x4
    lfs f3, lbl_8047BF60(r2)
    mtctr r0
@L_80060B60:
    addi r4, r3, 0x328
    lfs f1, 0x0(r4)
    fcmpu cr0, f3, f1
    beq @L_80060B90
    lfs f0, 0x3c(r6)
    fsubs f0, f1, f0
    stfs f0, 0x0(r4)
    lfs f0, 0x0(r4)
    fcmpo cr0, f0, f3
    bge @L_80060C4C
    stfs f3, 0x0(r4)
    b @L_80060C4C
@L_80060B90:
    lfs f0, 0x4(r4)
    lfs f1, 0x8(r4)
    fcmpu cr0, f0, f1
    beq @L_80060C4C
    fsubs f4, f1, f0
    lfs f2, lbl_8047BF94(r2)
    lfs f1, 0x3c(r6)
    lfs f0, lbl_8047BF98(r2)
    fmuls f2, f2, f4
    fmuls f4, f2, f1
    fcmpo cr0, f4, f0
    ble @L_80060BC4
    fmr f4, f0
@L_80060BC4:
    lfs f0, lbl_8047BF9C(r2)
    fcmpo cr0, f4, f0
    cror eq, lt, eq
    bne @L_80060BD8
    fmr f4, f0
@L_80060BD8:
    lfs f1, 0x4(r4)
    lfs f0, lbl_8047BF60(r2)
    fadds f1, f1, f4
    fcmpo cr0, f4, f0
    stfs f1, 0x4(r4)
    lfs f2, 0x8(r4)
    lfs f0, 0x4(r4)
    fsubs f1, f2, f0
    ble @L_80060C00
    b @L_80060C04
@L_80060C00:
    fneg f4, f4
@L_80060C04:
    lfs f0, lbl_8047BF60(r2)
    fcmpo cr0, f1, f0
    ble @L_80060C18
    fmr f0, f1
    b @L_80060C1C
@L_80060C18:
    fneg f0, f1
@L_80060C1C:
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    beq @L_80060C48
    lfs f0, lbl_8047BF60(r2)
    fcmpo cr0, f1, f0
    ble @L_80060C38
    b @L_80060C3C
@L_80060C38:
    fneg f1, f1
@L_80060C3C:
    lfs f0, lbl_8047BF90(r2)
    fcmpo cr0, f1, f0
    bge @L_80060C4C
@L_80060C48:
    stfs f2, 0x4(r4)
@L_80060C4C:
    addi r3, r3, 0xc
    bdnz @L_80060B60
    lis r3, lbl_803A9A60@ha
    slwi r4, r7, 2
    addi r0, r3, lbl_803A9A60@l
    mr r5, r6
    add r3, r0, r4
    addi r3, r3, 0x40
    li r0, 0x2
    lfs f3, lbl_8047BF60(r2)
    mtctr r0
@L_80060C78:
    lfs f0, 0x40(r5)
    fcmpu cr0, f3, f0
    beq @L_80060CA8
    lfs f1, 0x0(r3)
    lfs f0, 0x3c(r6)
    fsubs f0, f1, f0
    stfs f0, 0x0(r3)
    lfs f0, 0x0(r3)
    fcmpo cr0, f0, f3
    bge @L_80060D64
    stfs f3, 0x0(r3)
    b @L_80060D64
@L_80060CA8:
    lfs f0, 0x48(r5)
    lfs f1, 0x50(r5)
    fcmpu cr0, f0, f1
    beq @L_80060D64
    fsubs f4, f1, f0
    lfs f2, lbl_8047BF94(r2)
    lfs f1, 0x3c(r6)
    lfs f0, lbl_8047BF98(r2)
    fmuls f2, f2, f4
    fmuls f4, f2, f1
    fcmpo cr0, f4, f0
    ble @L_80060CDC
    fmr f4, f0
@L_80060CDC:
    lfs f0, lbl_8047BF9C(r2)
    fcmpo cr0, f4, f0
    cror eq, lt, eq
    bne @L_80060CF0
    fmr f4, f0
@L_80060CF0:
    lfs f1, 0x48(r5)
    lfs f0, lbl_8047BF60(r2)
    fadds f1, f1, f4
    fcmpo cr0, f4, f0
    stfs f1, 0x48(r5)
    lfs f2, 0x50(r5)
    lfs f0, 0x48(r5)
    fsubs f1, f2, f0
    ble @L_80060D18
    b @L_80060D1C
@L_80060D18:
    fneg f4, f4
@L_80060D1C:
    lfs f0, lbl_8047BF60(r2)
    fcmpo cr0, f1, f0
    ble @L_80060D30
    fmr f0, f1
    b @L_80060D34
@L_80060D30:
    fneg f0, f1
@L_80060D34:
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    beq @L_80060D60
    lfs f0, lbl_8047BF60(r2)
    fcmpo cr0, f1, f0
    ble @L_80060D50
    b @L_80060D54
@L_80060D50:
    fneg f1, f1
@L_80060D54:
    lfs f0, lbl_8047BF90(r2)
    fcmpo cr0, f1, f0
    bge @L_80060D64
@L_80060D60:
    stfs f2, 0x48(r5)
@L_80060D64:
    addi r5, r5, 0x4
    bdnz @L_80060C78
    blr
