    nofralloc
    fsubs f3, f2, f1
    lfs f0, -22664(r2)
    stwu r1, -0x20(r1)
    li r4, 0x2
    lfs f4, -22688(r2)
    fmuls f3, f3, f0
    fadds f0, f1, f3
    b @L_801B1D94
@L_801B1AF0:
    rlwinm. r0, r4, 0, 31, 31
    bne @L_801B1C44
    fmuls f13, f0, f0
    lfs f6, 0x4(r3)
    lfs f7, 0x0(r3)
    lfs f8, 0x8(r3)
    fmuls f12, f13, f0
    lfs f9, 0xc(r3)
    lfs f10, 0x10(r3)
    lfs f5, -22688(r2)
    fmuls f11, f12, f0
    fmuls f6, f6, f12
    fmadds f6, f7, f11, f6
    fmadds f6, f8, f13, f6
    fmadds f6, f9, f0, f6
    fadds f13, f10, f6
    fcmpo cr0, f13, f5
    bge @L_801B1B48
    lfs f5, -22660(r2)
    fcmpo cr0, f13, f5
    ble @L_801B1B48
    lfs f13, -22688(r2)
@L_801B1B48:
    lfs f5, -22688(r2)
    fcmpo cr0, f13, f5
    ble @L_801B1BAC
    frsqrte f12, f13
    lfd f11, -22656(r2)
    lfd f8, -22648(r2)
    lfd f9, -22656(r2)
    fmul f10, f12, f12
    lfd f6, -22648(r2)
    lfd f7, -22656(r2)
    fmul f11, f11, f12
    lfd f5, -22648(r2)
    fnmsub f8, f13, f10, f8
    fmul f12, f11, f8
    fmul f8, f12, f12
    fmul f9, f9, f12
    fnmsub f6, f13, f8, f6
    fmul f12, f9, f6
    fmul f6, f12, f12
    fmul f7, f7, f12
    fnmsub f5, f13, f6, f5
    fmul f12, f7, f5
    fmul f13, f13, f12
    frsp f13, f13
    b @L_801B1C38
@L_801B1BAC:
    lfd f5, -22640(r2)
    fcmpo cr0, f13, f5
    bge @L_801B1BC4
    lis r5, -32696
    lfs f13, -30016(r5)
    b @L_801B1C38
@L_801B1BC4:
    stfs f13, 0x14(r1)
    lis r0, 0x7f80
    lwz r5, 0x14(r1)
    rlwinm r5, r5, 0, 1, 8
    cmpw r5, r0
    beq @L_801B1BEC
    bge @L_801B1C24
    cmpwi r5, 0x0
    beq @L_801B1C08
    b @L_801B1C24
@L_801B1BEC:
    lwz r0, 0x14(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801B1C00
    li r0, 0x1
    b @L_801B1C28
@L_801B1C00:
    li r0, 0x2
    b @L_801B1C28
@L_801B1C08:
    lwz r0, 0x14(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801B1C1C
    li r0, 0x5
    b @L_801B1C28
@L_801B1C1C:
    li r0, 0x3
    b @L_801B1C28
@L_801B1C24:
    li r0, 0x4
@L_801B1C28:
    cmpwi r0, 0x1
    bne @L_801B1C38
    lis r5, -32696
    lfs f13, -30016(r5)
@L_801B1C38:
    lfs f5, -22632(r2)
    fmadds f4, f5, f13, f4
    b @L_801B1D8C
@L_801B1C44:
    fmuls f13, f0, f0
    lfs f6, 0x4(r3)
    lfs f7, 0x0(r3)
    lfs f8, 0x8(r3)
    fmuls f12, f13, f0
    lfs f9, 0xc(r3)
    lfs f10, 0x10(r3)
    lfs f5, -22688(r2)
    fmuls f11, f12, f0
    fmuls f6, f6, f12
    fmadds f6, f7, f11, f6
    fmadds f6, f8, f13, f6
    fmadds f6, f9, f0, f6
    fadds f13, f10, f6
    fcmpo cr0, f13, f5
    bge @L_801B1C94
    lfs f5, -22660(r2)
    fcmpo cr0, f13, f5
    ble @L_801B1C94
    lfs f13, -22688(r2)
@L_801B1C94:
    lfs f5, -22688(r2)
    fcmpo cr0, f13, f5
    ble @L_801B1CF8
    frsqrte f12, f13
    lfd f11, -22656(r2)
    lfd f8, -22648(r2)
    lfd f9, -22656(r2)
    fmul f10, f12, f12
    lfd f6, -22648(r2)
    lfd f7, -22656(r2)
    fmul f11, f11, f12
    lfd f5, -22648(r2)
    fnmsub f8, f13, f10, f8
    fmul f12, f11, f8
    fmul f8, f12, f12
    fmul f9, f9, f12
    fnmsub f6, f13, f8, f6
    fmul f12, f9, f6
    fmul f6, f12, f12
    fmul f7, f7, f12
    fnmsub f5, f13, f6, f5
    fmul f12, f7, f5
    fmul f13, f13, f12
    frsp f13, f13
    b @L_801B1D84
@L_801B1CF8:
    lfd f5, -22640(r2)
    fcmpo cr0, f13, f5
    bge @L_801B1D10
    lis r5, -32696
    lfs f13, -30016(r5)
    b @L_801B1D84
@L_801B1D10:
    stfs f13, 0x10(r1)
    lis r0, 0x7f80
    lwz r5, 0x10(r1)
    rlwinm r5, r5, 0, 1, 8
    cmpw r5, r0
    beq @L_801B1D38
    bge @L_801B1D70
    cmpwi r5, 0x0
    beq @L_801B1D54
    b @L_801B1D70
@L_801B1D38:
    lwz r0, 0x10(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801B1D4C
    li r0, 0x1
    b @L_801B1D74
@L_801B1D4C:
    li r0, 0x2
    b @L_801B1D74
@L_801B1D54:
    lwz r0, 0x10(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801B1D68
    li r0, 0x5
    b @L_801B1D74
@L_801B1D68:
    li r0, 0x3
    b @L_801B1D74
@L_801B1D70:
    li r0, 0x4
@L_801B1D74:
    cmpwi r0, 0x1
    bne @L_801B1D84
    lis r5, -32696
    lfs f13, -30016(r5)
@L_801B1D84:
    lfs f5, -22628(r2)
    fmadds f4, f5, f13, f4
@L_801B1D8C:
    fadds f0, f0, f3
    addi r4, r4, 0x1
@L_801B1D94:
    cmpwi r4, 0x8
    ble @L_801B1AF0
    fmuls f12, f1, f1
    lfs f0, 0x4(r3)
    lfs f6, 0x0(r3)
    lfs f7, 0x8(r3)
    fmuls f11, f12, f1
    lfs f8, 0xc(r3)
    lfs f9, 0x10(r3)
    lfs f5, -22688(r2)
    fmuls f10, f11, f1
    fmuls f0, f0, f11
    fmadds f0, f6, f10, f0
    fmadds f0, f7, f12, f0
    fmadds f0, f8, f1, f0
    fadds f0, f9, f0
    fcmpo cr0, f0, f5
    bge @L_801B1DEC
    lfs f1, -22660(r2)
    fcmpo cr0, f0, f1
    ble @L_801B1DEC
    lfs f0, -22688(r2)
@L_801B1DEC:
    lfs f1, -22688(r2)
    fcmpo cr0, f0, f1
    ble @L_801B1E50
    frsqrte f11, f0
    lfd f10, -22656(r2)
    lfd f7, -22648(r2)
    lfd f8, -22656(r2)
    fmul f9, f11, f11
    lfd f5, -22648(r2)
    lfd f6, -22656(r2)
    fmul f10, f10, f11
    lfd f1, -22648(r2)
    fnmsub f7, f0, f9, f7
    fmul f11, f10, f7
    fmul f7, f11, f11
    fmul f8, f8, f11
    fnmsub f5, f0, f7, f5
    fmul f11, f8, f5
    fmul f5, f11, f11
    fmul f6, f6, f11
    fnmsub f1, f0, f5, f1
    fmul f11, f6, f1
    fmul f0, f0, f11
    frsp f0, f0
    b @L_801B1EDC
@L_801B1E50:
    lfd f1, -22640(r2)
    fcmpo cr0, f0, f1
    bge @L_801B1E68
    lis r4, -32696
    lfs f0, -30016(r4)
    b @L_801B1EDC
@L_801B1E68:
    stfs f0, 0xc(r1)
    lis r0, 0x7f80
    lwz r4, 0xc(r1)
    rlwinm r4, r4, 0, 1, 8
    cmpw r4, r0
    beq @L_801B1E90
    bge @L_801B1EC8
    cmpwi r4, 0x0
    beq @L_801B1EAC
    b @L_801B1EC8
@L_801B1E90:
    lwz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801B1EA4
    li r0, 0x1
    b @L_801B1ECC
@L_801B1EA4:
    li r0, 0x2
    b @L_801B1ECC
@L_801B1EAC:
    lwz r0, 0xc(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801B1EC0
    li r0, 0x5
    b @L_801B1ECC
@L_801B1EC0:
    li r0, 0x3
    b @L_801B1ECC
@L_801B1EC8:
    li r0, 0x4
@L_801B1ECC:
    cmpwi r0, 0x1
    bne @L_801B1EDC
    lis r4, -32696
    lfs f0, -30016(r4)
@L_801B1EDC:
    fmuls f12, f2, f2
    lfs f5, 0x4(r3)
    lfs f6, 0x0(r3)
    lfs f7, 0x8(r3)
    fmuls f11, f12, f2
    lfs f8, 0xc(r3)
    lfs f9, 0x10(r3)
    lfs f1, -22688(r2)
    fmuls f10, f11, f2
    fmuls f5, f5, f11
    fmadds f5, f6, f10, f5
    fmadds f5, f7, f12, f5
    fmadds f2, f8, f2, f5
    fadds f11, f9, f2
    fcmpo cr0, f11, f1
    bge @L_801B1F2C
    lfs f1, -22660(r2)
    fcmpo cr0, f11, f1
    ble @L_801B1F2C
    lfs f11, -22688(r2)
@L_801B1F2C:
    lfs f1, -22688(r2)
    fcmpo cr0, f11, f1
    ble @L_801B1F90
    frsqrte f10, f11
    lfd f9, -22656(r2)
    lfd f6, -22648(r2)
    lfd f7, -22656(r2)
    fmul f8, f10, f10
    lfd f2, -22648(r2)
    lfd f5, -22656(r2)
    fmul f9, f9, f10
    lfd f1, -22648(r2)
    fnmsub f6, f11, f8, f6
    fmul f10, f9, f6
    fmul f6, f10, f10
    fmul f7, f7, f10
    fnmsub f2, f11, f6, f2
    fmul f10, f7, f2
    fmul f2, f10, f10
    fmul f5, f5, f10
    fnmsub f1, f11, f2, f1
    fmul f10, f5, f1
    fmul f11, f11, f10
    frsp f11, f11
    b @L_801B201C
@L_801B1F90:
    lfd f1, -22640(r2)
    fcmpo cr0, f11, f1
    bge @L_801B1FA8
    lis r3, -32696
    lfs f11, -30016(r3)
    b @L_801B201C
@L_801B1FA8:
    stfs f11, 0x8(r1)
    lis r0, 0x7f80
    lwz r3, 0x8(r1)
    rlwinm r3, r3, 0, 1, 8
    cmpw r3, r0
    beq @L_801B1FD0
    bge @L_801B2008
    cmpwi r3, 0x0
    beq @L_801B1FEC
    b @L_801B2008
@L_801B1FD0:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801B1FE4
    li r0, 0x1
    b @L_801B200C
@L_801B1FE4:
    li r0, 0x2
    b @L_801B200C
@L_801B1FEC:
    lwz r0, 0x8(r1)
    rlwinm. r0, r0, 0, 9, 31
    beq @L_801B2000
    li r0, 0x5
    b @L_801B200C
@L_801B2000:
    li r0, 0x3
    b @L_801B200C
@L_801B2008:
    li r0, 0x4
@L_801B200C:
    cmpwi r0, 0x1
    bne @L_801B201C
    lis r3, -32696
    lfs f11, -30016(r3)
@L_801B201C:
    fadds f0, f4, f0
    lfs f1, -22624(r2)
    fadds f0, f0, f11
    fmuls f0, f3, f0
    fdivs f1, f0, f1
    addi r1, r1, 0x20
    blr
