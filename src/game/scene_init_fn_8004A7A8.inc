    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    mr r31, r4
    lha r0, 0x6(r31)
    cmpwi r0, 0x6ed
    beq @L_8004ABF8
    bge @L_8004A83C
    cmpwi r0, 0x6e1
    beq @L_8004B264
    bge @L_8004A810
    cmpwi r0, 0x6dd
    beq @L_8004A8FC
    bge @L_8004A804
    cmpwi r0, 0x437
    bge @L_8004A7F8
    cmpwi r0, 0x432
    bge @L_8004A8FC
    b @L_8004B264
    @L_8004A7F8:
    cmpwi r0, 0x43b
    bge @L_8004B264
    b @L_8004A938
    @L_8004A804:
    cmpwi r0, 0x6e0
    bge @L_8004A898
    b @L_8004B264
    @L_8004A810:
    cmpwi r0, 0x6ea
    beq @L_8004A974
    bge @L_8004A830
    cmpwi r0, 0x6e7
    bge @L_8004B1DC
    cmpwi r0, 0x6e3
    bge @L_8004B218
    b @L_8004A898
    @L_8004A830:
    cmpwi r0, 0x6ec
    bge @L_8004AB24
    b @L_8004AA4C
    @L_8004A83C:
    cmpwi r0, 0x6f7
    bge @L_8004A870
    cmpwi r0, 0x6f1
    beq @L_8004AF58
    bge @L_8004A860
    cmpwi r0, 0x6ef
    beq @L_8004ADA8
    bge @L_8004AE80
    b @L_8004ACD0
    @L_8004A860:
    cmpwi r0, 0x6f3
    beq @L_8004B104
    bge @L_8004B1DC
    b @L_8004B02C
    @L_8004A870:
    cmpwi r0, 0x759
    bge @L_8004A88C
    cmpwi r0, 0x751
    bge @L_8004A898
    cmpwi r0, 0x6f9
    bge @L_8004B264
    b @L_8004B254
    @L_8004A88C:
    cmpwi r0, 0x777
    beq @L_8004A938
    b @L_8004B264
    @L_8004A898:
    lis r4, lbl_803A6A60@ha
    lis r5, lbl_802EF0A8@ha
    addi r4, r4, lbl_803A6A60@l
    lfs f0, 0x30(r4)
    mulli r4, r0, 0x1c
    addi r0, r5, lbl_802EF0A8@l
    fctiwz f0, f0
    add r4, r0, r4
    lha r4, 0x2(r4)
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    add r0, r4, r0
    extsh r0, r0
    sth r0, 0x50(r31)
    lha r6, 0x84(r3)
    lha r5, 0x50(r31)
    lha r4, 0x86(r3)
    lha r0, 0x52(r31)
    add r3, r6, r5
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800FE6D0
    bl fn_800FE4D4
    b @L_8004B264
    @L_8004A8FC:
    lis r3, lbl_803A6A60@ha
    lis r4, lbl_802EF0A8@ha
    addi r3, r3, lbl_803A6A60@l
    lfs f0, 0x34(r3)
    mulli r3, r0, 0x1c
    addi r0, r4, lbl_802EF0A8@l
    fctiwz f0, f0
    add r3, r0, r3
    lha r3, 0x4(r3)
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x52(r31)
    b @L_8004B264
    @L_8004A938:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004A958
    li r0, 0xff
    b @L_8004A964
    @L_8004A958:
    fctiwz f0, f1
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    @L_8004A964:
    subfic r0, r0, 0xff
    clrlwi r0, r0, 24
    stb r0, 0x67(r31)
    b @L_8004B264
    @L_8004A974:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004A994
    li r0, 0xff
    b @L_8004A9A0
    @L_8004A994:
    fctiwz f0, f1
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    @L_8004A9A0:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f3, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f0, lbl_8047BDAC(r2)
    lfs f2, 0x40(r3)
    lfs f1, lbl_804788D8(r13)
    fmuls f2, f3, f2
    lfs f3, lbl_8047BDC8(r2)
    fmuls f0, f1, f0
    fmadds f1, f3, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004AA04
    fmr f3, f0
    @L_8004AA04:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004AA14
    fmr f3, f0
    @L_8004AA14:
    fctiwz f0, f3
    stfd f0, 0x10(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x14(r1)
    stfd f0, 0x20(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x24(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x28(r1)
    stb r0, 0x65(r31)
    lwz r0, 0x2c(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004AA4C:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004AA6C
    li r0, 0xff
    b @L_8004AA78
    @L_8004AA6C:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004AA78:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f3, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, 0x40(r3)
    lfs f1, lbl_804788D8(r13)
    fmuls f2, f3, f2
    lfs f3, lbl_8047BDC8(r2)
    fmuls f0, f1, f0
    fmadds f1, f3, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004AADC
    fmr f3, f0
    @L_8004AADC:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004AAEC
    fmr f3, f0
    @L_8004AAEC:
    fctiwz f0, f3
    stfd f0, 0x20(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x24(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x14(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x8(r1)
    stb r0, 0x65(r31)
    lwz r0, 0xc(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004AB24:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004AB44
    li r0, 0xff
    b @L_8004AB50
    @L_8004AB44:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004AB50:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f2, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f3, lbl_8047BDC8(r2)
    lfs f1, 0x40(r3)
    lfs f0, lbl_804788D8(r13)
    fmuls f1, f2, f1
    fmuls f0, f0, f2
    fmadds f1, f3, f1, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004ABB0
    fmr f3, f0
    @L_8004ABB0:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004ABC0
    fmr f3, f0
    @L_8004ABC0:
    fctiwz f0, f3
    stfd f0, 0x20(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x24(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x14(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x8(r1)
    stb r0, 0x65(r31)
    lwz r0, 0xc(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004ABF8:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004AC18
    li r0, 0xff
    b @L_8004AC24
    @L_8004AC18:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004AC24:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f3, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f0, lbl_8047BDD4(r2)
    lfs f2, 0x40(r3)
    lfs f1, lbl_804788D8(r13)
    fmuls f2, f3, f2
    lfs f3, lbl_8047BDC8(r2)
    fmuls f0, f1, f0
    fmadds f1, f3, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004AC88
    fmr f3, f0
    @L_8004AC88:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004AC98
    fmr f3, f0
    @L_8004AC98:
    fctiwz f0, f3
    stfd f0, 0x20(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x24(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x14(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x8(r1)
    stb r0, 0x65(r31)
    lwz r0, 0xc(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004ACD0:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004ACF0
    li r0, 0xff
    b @L_8004ACFC
    @L_8004ACF0:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004ACFC:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f3, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f0, lbl_8047BDB8(r2)
    lfs f2, 0x40(r3)
    lfs f1, lbl_804788D8(r13)
    fmuls f2, f3, f2
    lfs f3, lbl_8047BDC8(r2)
    fmuls f0, f1, f0
    fmadds f1, f3, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004AD60
    fmr f3, f0
    @L_8004AD60:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004AD70
    fmr f3, f0
    @L_8004AD70:
    fctiwz f0, f3
    stfd f0, 0x20(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x24(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x14(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x8(r1)
    stb r0, 0x65(r31)
    lwz r0, 0xc(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004ADA8:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004ADC8
    li r0, 0xff
    b @L_8004ADD4
    @L_8004ADC8:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004ADD4:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f3, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f0, lbl_8047BDAC(r2)
    lfs f2, 0x40(r3)
    lfs f1, lbl_804788D8(r13)
    fmuls f2, f3, f2
    lfs f3, lbl_8047BDC8(r2)
    fmuls f0, f1, f0
    fmadds f1, f3, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004AE38
    fmr f3, f0
    @L_8004AE38:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004AE48
    fmr f3, f0
    @L_8004AE48:
    fctiwz f0, f3
    stfd f0, 0x20(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x24(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x14(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x8(r1)
    stb r0, 0x65(r31)
    lwz r0, 0xc(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004AE80:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004AEA0
    li r0, 0xff
    b @L_8004AEAC
    @L_8004AEA0:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004AEAC:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f3, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, 0x40(r3)
    lfs f1, lbl_804788D8(r13)
    fmuls f2, f3, f2
    lfs f3, lbl_8047BDC8(r2)
    fmuls f0, f1, f0
    fmadds f1, f3, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004AF10
    fmr f3, f0
    @L_8004AF10:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004AF20
    fmr f3, f0
    @L_8004AF20:
    fctiwz f0, f3
    stfd f0, 0x20(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x24(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x14(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x8(r1)
    stb r0, 0x65(r31)
    lwz r0, 0xc(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004AF58:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004AF78
    li r0, 0xff
    b @L_8004AF84
    @L_8004AF78:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004AF84:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f2, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f3, lbl_8047BDC8(r2)
    lfs f1, 0x40(r3)
    lfs f0, lbl_804788D8(r13)
    fmuls f1, f2, f1
    fmuls f0, f0, f2
    fmadds f1, f3, f1, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004AFE4
    fmr f3, f0
    @L_8004AFE4:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004AFF4
    fmr f3, f0
    @L_8004AFF4:
    fctiwz f0, f3
    stfd f0, 0x20(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x24(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x14(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x8(r1)
    stb r0, 0x65(r31)
    lwz r0, 0xc(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004B02C:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004B04C
    li r0, 0xff
    b @L_8004B058
    @L_8004B04C:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004B058:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f3, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f0, lbl_8047BDD4(r2)
    lfs f2, 0x40(r3)
    lfs f1, lbl_804788D8(r13)
    fmuls f2, f3, f2
    lfs f3, lbl_8047BDC8(r2)
    fmuls f0, f1, f0
    fmadds f1, f3, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004B0BC
    fmr f3, f0
    @L_8004B0BC:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004B0CC
    fmr f3, f0
    @L_8004B0CC:
    fctiwz f0, f3
    stfd f0, 0x20(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x24(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x14(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x8(r1)
    stb r0, 0x65(r31)
    lwz r0, 0xc(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004B104:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004B124
    li r0, 0xff
    b @L_8004B130
    @L_8004B124:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004B130:
    subfic r0, r0, 0xff
    lis r3, lbl_803A6A60@ha
    clrlwi r0, r0, 24
    lfs f3, lbl_8047BDCC(r2)
    stb r0, 0x67(r31)
    addi r3, r3, lbl_803A6A60@l
    lfs f0, lbl_8047BDB8(r2)
    lfs f2, 0x40(r3)
    lfs f1, lbl_804788D8(r13)
    fmuls f2, f3, f2
    lfs f3, lbl_8047BDC8(r2)
    fmuls f0, f1, f0
    fmadds f1, f3, f2, f0
    bl fn_800CE148
    frsp f3, f1
    lfs f0, lbl_8047BDA0(r2)
    lfs f2, lbl_8047BDA8(r2)
    lfs f1, lbl_8047BDD0(r2)
    fadds f3, f3, f0
    lfs f0, lbl_8047BDB0(r2)
    fmuls f3, f3, f2
    fmuls f3, f3, f1
    fcmpo cr0, f3, f0
    ble @L_8004B194
    fmr f3, f0
    @L_8004B194:
    lfs f0, lbl_8047BDAC(r2)
    fcmpo cr0, f3, f0
    bge @L_8004B1A4
    fmr f3, f0
    @L_8004B1A4:
    fctiwz f0, f3
    stfd f0, 0x20(r1)
    stfd f0, 0x18(r1)
    lwz r0, 0x24(r1)
    stfd f0, 0x10(r1)
    lwz r3, 0x1c(r1)
    stb r0, 0x67(r31)
    lwz r0, 0x14(r1)
    stb r3, 0x64(r31)
    stfd f0, 0x8(r1)
    stb r0, 0x65(r31)
    lwz r0, 0xc(r1)
    stb r0, 0x66(r31)
    b @L_8004B264
    @L_8004B1DC:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004B1FC
    li r0, 0xff
    b @L_8004B208
    @L_8004B1FC:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004B208:
    subfic r0, r0, 0xff
    clrlwi r0, r0, 24
    stb r0, 0x67(r31)
    b @L_8004B264
    @L_8004B218:
    lis r3, lbl_803A6A60@ha
    lfs f0, lbl_8047BDB0(r2)
    addi r3, r3, lbl_803A6A60@l
    lfs f1, 0x30(r3)
    fcmpo cr0, f1, f0
    ble @L_8004B238
    li r0, 0xff
    b @L_8004B244
    @L_8004B238:
    fctiwz f0, f1
    stfd f0, 0x28(r1)
    lwz r0, 0x2c(r1)
    @L_8004B244:
    subfic r0, r0, 0xff
    clrlwi r0, r0, 24
    stb r0, 0x67(r31)
    b @L_8004B264
    @L_8004B254:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    @L_8004B264:
    lwz r0, 0x44(r1)
    lwz r31, 0x3c(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
