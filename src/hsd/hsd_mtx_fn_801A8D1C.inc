    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stw r31, 0x5c(r1)
    stw r30, 0x58(r1)
    mr r30, r3
    mr r31, r4
    lfs f0, 0x0(r3)
    addi r3, r1, 0x40
    stfs f0, 0x40(r1)
    lfs f0, 0x10(r30)
    stfs f0, 0x44(r1)
    lfs f0, 0x20(r30)
    stfs f0, 0x48(r1)
    bl fn_800A3B20
    lfs f0, -23088(r2)
    fcmpo cr0, f1, f0
    ble @L_801A936C
    lfs f2, -23112(r2)
    lfs f0, -23108(r2)
    fdivs f2, f2, f1
    fcmpo cr0, f2, f0
    ble @L_801A8DE0
    lfs f0, -23112(r2)
    lfd f6, -23080(r2)
    fdivs f8, f0, f1
    lfd f3, -23072(r2)
    lfd f4, -23080(r2)
    lfd f1, -23072(r2)
    lfd f2, -23080(r2)
    lfd f0, -23072(r2)
    frsqrte f7, f8
    fmul f5, f7, f7
    fmul f6, f6, f7
    fnmsub f3, f8, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f8, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f8, f1, f0
    fmul f7, f2, f0
    fmul f1, f8, f7
    frsp f1, f1
    b @L_801A8E88
@L_801A8DE0:
    lfs f2, -23112(r2)
    lfd f0, -23104(r2)
    fdivs f2, f2, f1
    fcmpo cr0, f2, f0
    bge @L_801A8E00
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801A8E88
@L_801A8E00:
    lfs f0, -23112(r2)
    lis r0, 0x7f80
    fdivs f0, f0, f1
    stfs f0, 0x18(r1)
    lwz r3, 0x18(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801A8E30
    bge @L_801A8E68
    cmpwi r3, 0x0
    beq @L_801A8E4C
    b @L_801A8E68
@L_801A8E30:
    lwz r0, 0x18(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A8E44
    li r0, 0x1
    b @L_801A8E6C
@L_801A8E44:
    li r0, 0x2
    b @L_801A8E6C
@L_801A8E4C:
    lwz r0, 0x18(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A8E60
    li r0, 0x5
    b @L_801A8E6C
@L_801A8E60:
    li r0, 0x3
    b @L_801A8E6C
@L_801A8E68:
    li r0, 0x4
@L_801A8E6C:
    cmpwi r0, 0x1
    bne @L_801A8E80
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801A8E88
@L_801A8E80:
    lfs f0, -23112(r2)
    fdivs f1, f0, f1
@L_801A8E88:
    lfs f0, -23112(r2)
    addi r3, r1, 0x40
    addi r4, r1, 0x40
    fdivs f0, f0, f1
    stfs f0, 0x0(r31)
    bl fn_800A3AC0
    lfs f0, 0x4(r30)
    addi r3, r1, 0x40
    addi r4, r1, 0x34
    stfs f0, 0x34(r1)
    lfs f0, 0x14(r30)
    stfs f0, 0x38(r1)
    lfs f0, 0x24(r30)
    stfs f0, 0x3c(r1)
    bl fn_800A3B7C
    addi r3, r1, 0x40
    addi r4, r1, 0x1c
    bl fn_800A3AC0
    addi r3, r1, 0x34
    addi r4, r1, 0x1c
    addi r5, r1, 0x34
    bl fn_800A3A9C
    addi r3, r1, 0x34
    bl fn_800A3B20
    lfs f0, -23088(r2)
    fcmpo cr0, f1, f0
    ble @L_801A920C
    lfs f2, -23112(r2)
    lfs f0, -23108(r2)
    fdivs f2, f2, f1
    fcmpo cr0, f2, f0
    ble @L_801A8F68
    lfs f0, -23112(r2)
    lfd f6, -23080(r2)
    fdivs f8, f0, f1
    lfd f3, -23072(r2)
    lfd f4, -23080(r2)
    lfd f1, -23072(r2)
    lfd f2, -23080(r2)
    lfd f0, -23072(r2)
    frsqrte f7, f8
    fmul f5, f7, f7
    fmul f6, f6, f7
    fnmsub f3, f8, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f8, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f8, f1, f0
    fmul f7, f2, f0
    fmul f1, f8, f7
    frsp f1, f1
    b @L_801A9010
@L_801A8F68:
    lfs f2, -23112(r2)
    lfd f0, -23104(r2)
    fdivs f2, f2, f1
    fcmpo cr0, f2, f0
    bge @L_801A8F88
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801A9010
@L_801A8F88:
    lfs f0, -23112(r2)
    lis r0, 0x7f80
    fdivs f0, f0, f1
    stfs f0, 0x14(r1)
    lwz r3, 0x14(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801A8FB8
    bge @L_801A8FF0
    cmpwi r3, 0x0
    beq @L_801A8FD4
    b @L_801A8FF0
@L_801A8FB8:
    lwz r0, 0x14(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A8FCC
    li r0, 0x1
    b @L_801A8FF4
@L_801A8FCC:
    li r0, 0x2
    b @L_801A8FF4
@L_801A8FD4:
    lwz r0, 0x14(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A8FE8
    li r0, 0x5
    b @L_801A8FF4
@L_801A8FE8:
    li r0, 0x3
    b @L_801A8FF4
@L_801A8FF0:
    li r0, 0x4
@L_801A8FF4:
    cmpwi r0, 0x1
    bne @L_801A9008
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801A9010
@L_801A9008:
    lfs f0, -23112(r2)
    fdivs f1, f0, f1
@L_801A9010:
    lfs f0, -23112(r2)
    addi r3, r1, 0x34
    addi r4, r1, 0x34
    fdivs f0, f0, f1
    stfs f0, 0x4(r31)
    bl fn_800A3AC0
    lfs f0, 0x8(r30)
    addi r3, r1, 0x34
    addi r4, r1, 0x28
    stfs f0, 0x28(r1)
    lfs f0, 0x18(r30)
    stfs f0, 0x2c(r1)
    lfs f0, 0x28(r30)
    stfs f0, 0x30(r1)
    bl fn_800A3B7C
    addi r3, r1, 0x34
    addi r4, r1, 0x1c
    bl fn_800A3AC0
    addi r3, r1, 0x28
    addi r4, r1, 0x1c
    addi r5, r1, 0x28
    bl fn_800A3A9C
    addi r3, r1, 0x40
    addi r4, r1, 0x28
    bl fn_800A3B7C
    addi r3, r1, 0x40
    addi r4, r1, 0x1c
    bl fn_800A3AC0
    addi r3, r1, 0x28
    addi r4, r1, 0x1c
    addi r5, r1, 0x28
    bl fn_800A3A9C
    addi r3, r1, 0x28
    bl fn_800A3B20
    lfs f0, -23088(r2)
    fcmpo cr0, f1, f0
    ble @L_801A9200
    lfs f0, -23108(r2)
    fcmpo cr0, f1, f0
    ble @L_801A9108
    frsqrte f8, f1
    lfd f7, -23080(r2)
    lfd f4, -23072(r2)
    lfd f5, -23080(r2)
    fmul f6, f8, f8
    lfd f2, -23072(r2)
    lfd f3, -23080(r2)
    fmul f7, f7, f8
    lfd f0, -23072(r2)
    fnmsub f4, f1, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f1, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f0, f1, f2, f0
    fmul f8, f3, f0
    fmul f1, f1, f8
    frsp f1, f1
    b @L_801A9194
@L_801A9108:
    lfd f0, -23104(r2)
    fcmpo cr0, f1, f0
    bge @L_801A9120
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801A9194
@L_801A9120:
    stfs f1, 0x10(r1)
    lis r0, 0x7f80
    lwz r3, 0x10(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801A9148
    bge @L_801A9180
    cmpwi r3, 0x0
    beq @L_801A9164
    b @L_801A9180
@L_801A9148:
    lwz r0, 0x10(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A915C
    li r0, 0x1
    b @L_801A9184
@L_801A915C:
    li r0, 0x2
    b @L_801A9184
@L_801A9164:
    lwz r0, 0x10(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A9178
    li r0, 0x5
    b @L_801A9184
@L_801A9178:
    li r0, 0x3
    b @L_801A9184
@L_801A9180:
    li r0, 0x4
@L_801A9184:
    cmpwi r0, 0x1
    bne @L_801A9194
    lis r3, -32696
    lfs f1, -30016(r3)
@L_801A9194:
    stfs f1, 0x8(r31)
    addi r3, r1, 0x34
    addi r4, r1, 0x28
    addi r5, r1, 0x1c
    bl fn_800A3B9C
    addi r3, r1, 0x40
    addi r4, r1, 0x1c
    bl fn_800A3B7C
    lfd f0, -23104(r2)
    fcmpo cr0, f1, f0
    bge @L_801A9550
    lfs f2, 0x0(r31)
    lfd f0, -23096(r2)
    lfd f1, -23096(r2)
    fmul f2, f2, f0
    lfd f0, -23096(r2)
    frsp f2, f2
    stfs f2, 0x0(r31)
    lfs f2, 0x4(r31)
    fmul f1, f2, f1
    frsp f1, f1
    stfs f1, 0x4(r31)
    lfs f1, 0x8(r31)
    fmul f0, f1, f0
    frsp f0, f0
    stfs f0, 0x8(r31)
    b @L_801A9550
@L_801A9200:
    lfs f0, -23108(r2)
    stfs f0, 0x8(r31)
    b @L_801A9550
@L_801A920C:
    lfs f0, -23108(r2)
    addi r3, r1, 0x40
    addi r4, r1, 0x28
    stfs f0, 0x4(r31)
    lfs f0, 0x8(r30)
    stfs f0, 0x28(r1)
    lfs f0, 0x18(r30)
    stfs f0, 0x2c(r1)
    lfs f0, 0x28(r30)
    stfs f0, 0x30(r1)
    bl fn_800A3B7C
    addi r3, r1, 0x40
    addi r4, r1, 0x1c
    bl fn_800A3AC0
    addi r3, r1, 0x28
    addi r4, r1, 0x1c
    addi r5, r1, 0x28
    bl fn_800A3A9C
    addi r3, r1, 0x28
    bl fn_800A3B20
    lfs f0, -23088(r2)
    fcmpo cr0, f1, f0
    ble @L_801A9360
    lfs f0, -23108(r2)
    fcmpo cr0, f1, f0
    ble @L_801A92CC
    frsqrte f8, f1
    lfd f7, -23080(r2)
    lfd f4, -23072(r2)
    lfd f5, -23080(r2)
    fmul f6, f8, f8
    lfd f2, -23072(r2)
    lfd f3, -23080(r2)
    fmul f7, f7, f8
    lfd f0, -23072(r2)
    fnmsub f4, f1, f6, f4
    fmul f8, f7, f4
    fmul f4, f8, f8
    fmul f5, f5, f8
    fnmsub f2, f1, f4, f2
    fmul f8, f5, f2
    fmul f2, f8, f8
    fmul f3, f3, f8
    fnmsub f0, f1, f2, f0
    fmul f8, f3, f0
    fmul f1, f1, f8
    frsp f1, f1
    b @L_801A9358
@L_801A92CC:
    lfd f0, -23104(r2)
    fcmpo cr0, f1, f0
    bge @L_801A92E4
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801A9358
@L_801A92E4:
    stfs f1, 0xc(r1)
    lis r0, 0x7f80
    lwz r3, 0xc(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801A930C
    bge @L_801A9344
    cmpwi r3, 0x0
    beq @L_801A9328
    b @L_801A9344
@L_801A930C:
    lwz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A9320
    li r0, 0x1
    b @L_801A9348
@L_801A9320:
    li r0, 0x2
    b @L_801A9348
@L_801A9328:
    lwz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A933C
    li r0, 0x5
    b @L_801A9348
@L_801A933C:
    li r0, 0x3
    b @L_801A9348
@L_801A9344:
    li r0, 0x4
@L_801A9348:
    cmpwi r0, 0x1
    bne @L_801A9358
    lis r3, -32696
    lfs f1, -30016(r3)
@L_801A9358:
    stfs f1, 0x8(r31)
    b @L_801A9550
@L_801A9360:
    lfs f0, -23108(r2)
    stfs f0, 0x8(r31)
    b @L_801A9550
@L_801A936C:
    lfs f0, -23108(r2)
    addi r3, r1, 0x34
    stfs f0, 0x0(r31)
    lfs f0, 0x4(r30)
    stfs f0, 0x34(r1)
    lfs f0, 0x14(r30)
    stfs f0, 0x38(r1)
    lfs f0, 0x24(r30)
    stfs f0, 0x3c(r1)
    bl fn_800A3B20
    lfs f0, -23088(r2)
    fcmpo cr0, f1, f0
    ble @L_801A9524
    lfs f2, -23112(r2)
    lfs f0, -23108(r2)
    fdivs f2, f2, f1
    fcmpo cr0, f2, f0
    ble @L_801A9414
    lfs f0, -23112(r2)
    lfd f6, -23080(r2)
    fdivs f8, f0, f1
    lfd f3, -23072(r2)
    lfd f4, -23080(r2)
    lfd f1, -23072(r2)
    lfd f2, -23080(r2)
    lfd f0, -23072(r2)
    frsqrte f7, f8
    fmul f5, f7, f7
    fmul f6, f6, f7
    fnmsub f3, f8, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f8, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f8, f1, f0
    fmul f7, f2, f0
    fmul f1, f8, f7
    frsp f1, f1
    b @L_801A94BC
@L_801A9414:
    lfs f2, -23112(r2)
    lfd f0, -23104(r2)
    fdivs f2, f2, f1
    fcmpo cr0, f2, f0
    bge @L_801A9434
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801A94BC
@L_801A9434:
    lfs f0, -23112(r2)
    lis r0, 0x7f80
    fdivs f0, f0, f1
    stfs f0, 0x8(r1)
    lwz r3, 0x8(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801A9464
    bge @L_801A949C
    cmpwi r3, 0x0
    beq @L_801A9480
    b @L_801A949C
@L_801A9464:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A9478
    li r0, 0x1
    b @L_801A94A0
@L_801A9478:
    li r0, 0x2
    b @L_801A94A0
@L_801A9480:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801A9494
    li r0, 0x5
    b @L_801A94A0
@L_801A9494:
    li r0, 0x3
    b @L_801A94A0
@L_801A949C:
    li r0, 0x4
@L_801A94A0:
    cmpwi r0, 0x1
    bne @L_801A94B4
    lis r3, -32696
    lfs f1, -30016(r3)
    b @L_801A94BC
@L_801A94B4:
    lfs f0, -23112(r2)
    fdivs f1, f0, f1
@L_801A94BC:
    lfs f0, -23112(r2)
    addi r3, r1, 0x34
    addi r4, r1, 0x34
    fdivs f0, f0, f1
    stfs f0, 0x4(r31)
    bl fn_800A3AC0
    lfs f0, 0x8(r30)
    addi r3, r1, 0x34
    addi r4, r1, 0x28
    stfs f0, 0x28(r1)
    lfs f0, 0x18(r30)
    stfs f0, 0x2c(r1)
    lfs f0, 0x28(r30)
    stfs f0, 0x30(r1)
    bl fn_800A3B7C
    addi r3, r1, 0x34
    addi r4, r1, 0x1c
    bl fn_800A3AC0
    addi r3, r1, 0x28
    addi r4, r1, 0x1c
    addi r5, r1, 0x28
    bl fn_800A3A9C
    addi r3, r1, 0x28
    bl fn_800A3B38
    stfs f1, 0x8(r31)
    b @L_801A9550
@L_801A9524:
    lfs f0, -23108(r2)
    addi r3, r1, 0x28
    stfs f0, 0x4(r31)
    lfs f0, 0x8(r30)
    stfs f0, 0x28(r1)
    lfs f0, 0x18(r30)
    stfs f0, 0x2c(r1)
    lfs f0, 0x28(r30)
    stfs f0, 0x30(r1)
    bl fn_800A3B38
    stfs f1, 0x8(r31)
@L_801A9550:
    psq_l f31, 0x68(r1), 0, 0
    lwz r0, 0x74(r1)
    lfd f31, 0x60(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
