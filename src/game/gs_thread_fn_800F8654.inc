    nofralloc
    stwu r1, -0x30(r1)
    extsb r12, r4
    subi r0, r12, 0x2
    stw r31, 0x2c(r1)
    lwz r11, 0x38(r1)
    stw r30, 0x28(r1)
    lbz r30, 0x0(r6)
    extsb r30, r30
    cmpw r30, r0
    blt @L_800F86AC
    addi r0, r12, 0x2
    cmpw r30, r0
    bgt @L_800F86AC
    lbz r30, 0x0(r7)
    extsb r12, r5
    subi r0, r12, 0x2
    extsb r30, r30
    cmpw r30, r0
    blt @L_800F86AC
    addi r0, r12, 0x2
    cmpw r30, r0
    ble @L_800F87E0
@L_800F86AC:
    lwz r0, 0x14(r3)
    cmpwi r0, 0x0
    bne @L_800F8740
    extsb r0, r4
    lis r30, 0x4330
    xoris r0, r0, 0x8000
    lbz r12, 0x10(r3)
    stw r0, 0xc(r1)
    extsb r0, r5
    xoris r0, r0, 0x8000
    lfd f4, lbl_8047CCC8(r2)
    stw r30, 0x8(r1)
    lfs f1, 0x0(r10)
    lfd f0, 0x8(r1)
    stw r12, 0x14(r1)
    fsubs f3, f0, f4
    lfd f2, lbl_8047CCE0(r2)
    stw r30, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f1, f3, f1
    stw r0, 0x1c(r1)
    fsubs f0, f0, f2
    stw r30, 0x18(r1)
    fdivs f1, f1, f0
    lfd f0, 0x18(r1)
    stw r30, 0x20(r1)
    stfs f1, 0x0(r8)
    fsubs f1, f0, f4
    lbz r0, 0x10(r3)
    lfs f0, 0x0(r11)
    stw r0, 0x24(r1)
    fsubs f1, f1, f0
    lfd f0, 0x20(r1)
    fsubs f0, f0, f2
    fdivs f0, f1, f0
    stfs f0, 0x0(r9)
    b @L_800F87D8
@L_800F8740:
    cmpwi r0, 0x1
    bne @L_800F87D8
    extsb r30, r4
    lbz r12, 0x10(r3)
    lis r31, 0x4330
    extsb r0, r5
    xoris r30, r30, 0x8000
    stw r31, 0x20(r1)
    xoris r0, r0, 0x8000
    lfd f6, lbl_8047CCC8(r2)
    stw r30, 0x24(r1)
    lfd f4, lbl_8047CCE0(r2)
    lfd f0, 0x20(r1)
    stw r12, 0x1c(r1)
    fsubs f2, f0, f6
    lfs f1, 0x0(r10)
    stw r31, 0x18(r1)
    lfs f3, lbl_8047CCD4(r2)
    lfd f0, 0x18(r1)
    fsubs f5, f2, f1
    stw r0, 0x14(r1)
    fsubs f0, f0, f4
    stw r31, 0x10(r1)
    fmuls f2, f0, f3
    lfd f0, 0x10(r1)
    stw r31, 0x8(r1)
    fsubs f1, f0, f6
    fdivs f0, f5, f2
    stfs f0, 0x0(r8)
    lbz r0, 0x10(r3)
    lfs f0, 0x0(r11)
    stw r0, 0xc(r1)
    fsubs f1, f1, f0
    lfd f0, 0x8(r1)
    fsubs f0, f0, f4
    fmuls f0, f0, f3
    fdivs f0, f1, f0
    stfs f0, 0x0(r9)
@L_800F87D8:
    li r0, 0x0
    stb r0, 0x11(r3)
@L_800F87E0:
    stb r4, 0x0(r6)
    stb r5, 0x0(r7)
    lfs f1, 0x0(r10)
    lfs f0, 0x0(r8)
    fadds f0, f1, f0
    stfs f0, 0x0(r10)
    lfs f1, 0x0(r11)
    lfs f0, 0x0(r9)
    fadds f0, f1, f0
    stfs f0, 0x0(r11)
    lwz r0, 0x14(r3)
    cmpwi r0, 0x1
    bne @L_800F890C
    lbz r4, 0x11(r3)
    addi r0, r4, 0x1
    stb r0, 0x11(r3)
    lbz r4, 0x11(r3)
    lbz r0, 0x10(r3)
    cmplw r4, r0
    bge @L_800F890C
    lfs f1, 0x0(r8)
    lfs f2, lbl_8047CCD8(r2)
    lfs f0, lbl_8047CCD0(r2)
    fmuls f1, f1, f2
    stfs f1, 0x0(r8)
    lfs f1, 0x0(r9)
    fmuls f1, f1, f2
    stfs f1, 0x0(r9)
    lfs f2, 0x0(r8)
    fcmpo cr0, f2, f0
    ble @L_800F8864
    fmr f1, f2
    b @L_800F8868
@L_800F8864:
    fneg f1, f2
@L_800F8868:
    lfs f0, lbl_8047CCDC(r2)
    fcmpo cr0, f1, f0
    bge @L_800F88AC
    lfs f0, lbl_8047CCD0(r2)
    fcmpo cr0, f2, f0
    ble @L_800F8888
    li r0, 0x1
    b @L_800F888C
@L_800F8888:
    li r0, -0x1
@L_800F888C:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0x24(r1)
    lfd f1, lbl_8047CCC8(r2)
    stw r0, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    stfs f0, 0x0(r8)
@L_800F88AC:
    lfs f1, 0x0(r9)
    lfs f0, lbl_8047CCD0(r2)
    fcmpo cr0, f1, f0
    ble @L_800F88C0
    b @L_800F88C4
@L_800F88C0:
    fneg f1, f1
@L_800F88C4:
    lfs f0, lbl_8047CCDC(r2)
    fcmpo cr0, f1, f0
    bge @L_800F890C
    lfs f1, 0x0(r8)
    lfs f0, lbl_8047CCD0(r2)
    fcmpo cr0, f1, f0
    ble @L_800F88E8
    li r0, 0x1
    b @L_800F88EC
@L_800F88E8:
    li r0, -0x1
@L_800F88EC:
    xoris r3, r0, 0x8000
    lis r0, 0x4330
    stw r3, 0x24(r1)
    lfd f1, lbl_8047CCC8(r2)
    stw r0, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    stfs f0, 0x0(r9)
@L_800F890C:
    lfs f1, 0x0(r8)
    lfs f0, lbl_8047CCD0(r2)
    fcmpo cr0, f1, f0
    bge @L_800F8964
    lbz r3, 0x0(r6)
    lis r0, 0x4330
    stw r0, 0x20(r1)
    extsb r3, r3
    lfd f1, lbl_8047CCC8(r2)
    xoris r3, r3, 0x8000
    lfs f2, 0x0(r10)
    stw r3, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    bge @L_800F89A8
    stw r3, 0x24(r1)
    stw r0, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    stfs f0, 0x0(r10)
    b @L_800F89A8
@L_800F8964:
    lbz r3, 0x0(r6)
    lis r0, 0x4330
    stw r0, 0x20(r1)
    extsb r3, r3
    lfd f1, lbl_8047CCC8(r2)
    xoris r3, r3, 0x8000
    lfs f2, 0x0(r10)
    stw r3, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    ble @L_800F89A8
    stw r3, 0x24(r1)
    stw r0, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    stfs f0, 0x0(r10)
@L_800F89A8:
    lfs f1, 0x0(r9)
    lfs f0, lbl_8047CCD0(r2)
    fcmpo cr0, f1, f0
    bge @L_800F8A00
    lbz r3, 0x0(r7)
    lis r0, 0x4330
    stw r0, 0x20(r1)
    extsb r3, r3
    lfd f1, lbl_8047CCC8(r2)
    xoris r3, r3, 0x8000
    lfs f2, 0x0(r11)
    stw r3, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    bge @L_800F8A44
    stw r3, 0x24(r1)
    stw r0, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    stfs f0, 0x0(r11)
    b @L_800F8A44
@L_800F8A00:
    lbz r3, 0x0(r7)
    lis r0, 0x4330
    stw r0, 0x20(r1)
    extsb r3, r3
    lfd f1, lbl_8047CCC8(r2)
    xoris r3, r3, 0x8000
    lfs f2, 0x0(r11)
    stw r3, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f2, f0
    ble @L_800F8A44
    stw r3, 0x24(r1)
    stw r0, 0x20(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    stfs f0, 0x0(r11)
@L_800F8A44:
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    addi r1, r1, 0x30
    blr
