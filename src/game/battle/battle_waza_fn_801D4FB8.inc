    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stfd f30, 0x40(r1)
    psq_st f30, 0x48(r1), 0, 0
    stfd f29, 0x30(r1)
    psq_st f29, 0x38(r1), 0, 0
    stmw r27, 0x1c(r1)
    mr r27, r3
    mr r28, r4
    lwz r30, 0x24(r3)
    mr r29, r5
    mr r3, r30
    bl fn_800E3C5C
    mr r31, r3
    mr r3, r30
    addi r4, r1, 0x8
    bl fn_800E3D6C
    lfs f0, 0xc(r1)
    stfs f0, 0x14(r29)
    lwz r3, 0x10(r27)
    cmpwi r3, 0x1
    beq @L_801D5060
    bge @L_801D5038
    cmpwi r3, -0x1
    beq @L_801D5054
    bge @L_801D5084
    cmpwi r3, -0x2
    bge @L_801D5048
    b @L_801D5084
@L_801D5038:
    cmpwi r3, 0x3
    beq @L_801D5078
    bge @L_801D5084
    b @L_801D506C
@L_801D5048:
    lfs f31, -21528(r2)
    lfs f2, -21600(r2)
    b @L_801D508C
@L_801D5054:
    lfs f31, -21528(r2)
    lfs f2, -21600(r2)
    b @L_801D508C
@L_801D5060:
    lfs f31, -21524(r2)
    lfs f2, -21596(r2)
    b @L_801D508C
@L_801D506C:
    lfs f31, -21520(r2)
    lfs f2, -21592(r2)
    b @L_801D508C
@L_801D5078:
    lfs f31, -21516(r2)
    lfs f2, -21588(r2)
    b @L_801D508C
@L_801D5084:
    lfs f31, -21512(r2)
    lfs f2, -21676(r2)
@L_801D508C:
    rlwinm. r0, r28, 0, 31, 31
    beq @L_801D50A8
    lfs f1, -21668(r2)
    lfs f0, -21508(r2)
    stfs f1, 0xc(r29)
    stfs f0, 0x10(r29)
    b @L_801D517C
@L_801D50A8:
    rlwinm. r0, r28, 0, 30, 30
    beq @L_801D50C4
    lfs f1, -21504(r2)
    lfs f0, -21500(r2)
    stfs f1, 0xc(r29)
    stfs f0, 0x10(r29)
    b @L_801D517C
@L_801D50C4:
    rlwinm. r0, r28, 0, 29, 29
    beq @L_801D50FC
    cmpwi r3, 0x0
    ble @L_801D50E8
    lfs f1, -21504(r2)
    lfs f0, -21500(r2)
    stfs f1, 0xc(r29)
    stfs f0, 0x10(r29)
    b @L_801D517C
@L_801D50E8:
    lfs f1, -21500(r2)
    lfs f0, -21496(r2)
    stfs f1, 0xc(r29)
    stfs f0, 0x10(r29)
    b @L_801D517C
@L_801D50FC:
    rlwinm. r0, r28, 0, 28, 28
    beq @L_801D5134
    cmpwi r3, 0x0
    ble @L_801D5120
    lfs f1, -21500(r2)
    lfs f0, -21496(r2)
    stfs f1, 0xc(r29)
    stfs f0, 0x10(r29)
    b @L_801D517C
@L_801D5120:
    lfs f1, -21496(r2)
    lfs f0, -21492(r2)
    stfs f1, 0xc(r29)
    stfs f0, 0x10(r29)
    b @L_801D517C
@L_801D5134:
    rlwinm. r0, r28, 0, 27, 27
    beq @L_801D5150
    lfs f1, -21492(r2)
    lfs f0, -21488(r2)
    stfs f1, 0xc(r29)
    stfs f0, 0x10(r29)
    b @L_801D517C
@L_801D5150:
    cmpwi r3, 0x0
    ble @L_801D516C
    lfs f1, -21504(r2)
    lfs f0, -21496(r2)
    stfs f1, 0xc(r29)
    stfs f0, 0x10(r29)
    b @L_801D517C
@L_801D516C:
    lfs f1, -21504(r2)
    lfs f0, -21484(r2)
    stfs f1, 0xc(r29)
    stfs f0, 0x10(r29)
@L_801D517C:
    rlwinm. r0, r28, 0, 26, 26
    beq @L_801D51A0
    lfs f1, -21664(r2)
    lfs f0, -21472(r2)
    stfs f1, 0x20(r29)
    lfs f30, -21480(r2)
    stfs f0, 0x24(r29)
    lfs f29, -21476(r2)
    b @L_801D51FC
@L_801D51A0:
    rlwinm. r0, r28, 0, 25, 25
    beq @L_801D51C4
    lfs f1, -21472(r2)
    lfs f0, -21584(r2)
    stfs f1, 0x20(r29)
    lfs f30, -21480(r2)
    stfs f0, 0x24(r29)
    lfs f29, -21468(r2)
    b @L_801D51FC
@L_801D51C4:
    rlwinm. r0, r28, 0, 24, 24
    beq @L_801D51E8
    lfs f1, -21584(r2)
    lfs f0, -21464(r2)
    stfs f1, 0x20(r29)
    lfs f30, -21480(r2)
    stfs f0, 0x24(r29)
    lfs f29, -21652(r2)
    b @L_801D51FC
@L_801D51E8:
    lfs f29, -21652(r2)
    lfs f0, -21464(r2)
    stfs f29, 0x20(r29)
    lfs f30, -21480(r2)
    stfs f0, 0x24(r29)
@L_801D51FC:
    lfs f0, 0x20(r29)
    addi r30, r31, 0x28
    mr r3, r30
    fmuls f0, f0, f2
    stfs f0, 0x20(r29)
    lfs f0, 0x24(r29)
    fmuls f0, f0, f2
    stfs f0, 0x24(r29)
    bl fn_800E008C
    lfs f0, -21460(r2)
    lfs f2, 0x4(r30)
    fmuls f0, f0, f1
    stfs f0, 0x4(r29)
    stfs f2, 0x8(r29)
    lfs f0, 0x14(r31)
    stfs f0, 0x18(r29)
    lfs f0, 0x20(r31)
    stfs f0, 0x1c(r29)
    lfs f0, 0x18(r29)
    fcmpo cr0, f0, f30
    bge @L_801D526C
    stfs f30, 0x18(r29)
    lfs f0, 0x1c(r29)
    fcmpo cr0, f0, f30
    bge @L_801D526C
    lfs f0, -21516(r2)
    fmuls f0, f0, f30
    stfs f0, 0x1c(r29)
@L_801D526C:
    lfs f0, 0x1c(r29)
    fcmpo cr0, f0, f29
    ble @L_801D5294
    stfs f29, 0x1c(r29)
    lfs f0, 0x18(r29)
    fcmpo cr0, f0, f29
    ble @L_801D5294
    lfs f0, -21456(r2)
    fmuls f0, f0, f29
    stfs f0, 0x18(r29)
@L_801D5294:
    lfs f1, 0x4(r29)
    lfs f0, -21652(r2)
    fmuls f1, f1, f31
    stfs f1, 0x4(r29)
    lfs f1, 0x8(r29)
    fmuls f1, f1, f31
    stfs f1, 0x8(r29)
    lfs f1, 0x20(r29)
    fcmpo cr0, f1, f0
    bge @L_801D52C0
    stfs f0, 0x20(r29)
@L_801D52C0:
    lfs f1, 0x24(r29)
    lfs f0, -21452(r2)
    fcmpo cr0, f1, f0
    bge @L_801D52D4
    stfs f0, 0x24(r29)
@L_801D52D4:
    lfs f1, 0x20(r29)
    lfs f0, -21448(r2)
    fcmpo cr0, f1, f0
    ble @L_801D52E8
    stfs f0, 0x20(r29)
@L_801D52E8:
    lfs f1, 0x24(r29)
    lfs f0, -21464(r2)
    fcmpo cr0, f1, f0
    ble @L_801D52FC
    stfs f0, 0x24(r29)
@L_801D52FC:
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    psq_l f30, 0x48(r1), 0, 0
    lfd f30, 0x40(r1)
    psq_l f29, 0x38(r1), 0, 0
    lfd f29, 0x30(r1)
    lmw r27, 0x1c(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
