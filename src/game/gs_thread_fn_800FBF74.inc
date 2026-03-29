    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r27, 0x2c(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    lwz r4, lbl_80478B08(r13)
    li r6, 0x0
    li r3, 0x0
    lhz r5, 0x0(r4)
    mtctr r5
    cmpwi r5, 0x0
    ble @L_800FBFCC
@L_800FBFAC:
    lwz r0, 0x20(r4)
    add r31, r0, r3
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_800FBFCC
    addi r3, r3, 0x68
    addi r6, r6, 0x1
    bdnz @L_800FBFAC
@L_800FBFCC:
    cmpw r6, r5
    bne @L_800FBFF0
    lis r3, lbl_80271730@ha
    mr r4, r28
    addi r3, r3, lbl_80271730@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, -0x1
    b @L_800FC1BC
@L_800FBFF0:
    cmplwi r28, 0x0
    bne @L_800FC000
    li r27, 0x0
    b @L_800FC08C
@L_800FC000:
    lwz r9, 0x8(r4)
    srwi r3, r28, 20
    clrlwi r4, r28, 12
    b @L_800FC080
@L_800FC010:
    lhz r0, 0x0(r9)
    cmplw r0, r3
    bne @L_800FC07C
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FC074
@L_800FC02C:
    add r0, r7, r6
    srwi r5, r0, 1
    slwi r10, r5, 3
    lwzx r0, r8, r10
    cmplw r0, r4
    bne @L_800FC064
    addi r0, r1, 0x8
    cmplwi r0, 0x0
    beq @L_800FC054
    stw r9, 0x8(r1)
@L_800FC054:
    add r3, r8, r10
    lwz r0, 0x4(r3)
    add r27, r9, r0
    b @L_800FC08C
@L_800FC064:
    bge @L_800FC070
    addi r7, r5, 0x1
    b @L_800FC074
@L_800FC070:
    mr r6, r5
@L_800FC074:
    cmplw r7, r6
    blt @L_800FC02C
@L_800FC07C:
    lwz r9, 0x8(r9)
@L_800FC080:
    cmplwi r9, 0x0
    bne @L_800FC010
    li r27, 0x0
@L_800FC08C:
    cmplwi r27, 0x0
    bne @L_800FC0B0
    lis r3, lbl_80271754@ha
    mr r4, r28
    addi r3, r3, lbl_80271754@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, -0x1
    b @L_800FC1BC
@L_800FC0B0:
    mr r3, r31
    li r4, 0x0
    li r5, 0x68
    bl memset
    li r0, 0x1
    lfs f0, lbl_8047CD08(r2)
    stb r0, 0x0(r31)
    li r0, -0x1
    li r5, 0x0
    stfs f0, 0x60(r31)
    stfs f0, 0x64(r31)
    stw r0, 0x24(r31)
    stw r27, 0x28(r31)
    stw r27, 0x2c(r31)
    stw r27, 0x30(r31)
    lwz r3, 0x8(r1)
    lbz r0, 0x3(r3)
    sth r0, 0x20(r31)
    stw r28, 0x1c(r31)
    stb r29, 0x44(r31)
    stb r30, 0x3(r31)
    lwz r6, lbl_80478B08(r13)
    lhz r0, 0x4(r6)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_800FC1B8
@L_800FC118:
    lwz r3, 0x24(r6)
    lhz r0, 0x20(r31)
    add r4, r3, r5
    lhz r3, 0x0(r4)
    cmplw r3, r0
    bne @L_800FC1B0
    lbz r0, 0x2(r4)
    stb r0, 0x22(r31)
    lbz r0, 0x3(r4)
    stb r0, 0x23(r31)
    lhz r0, 0x20(r31)
    cmplwi r0, 0x0
    bne @L_800FC158
    li r0, 0xb
    stb r0, 0x42(r31)
    b @L_800FC1B8
@L_800FC158:
    cmplwi r0, 0x1
    beq @L_800FC164
    bne @L_800FC170
@L_800FC164:
    li r0, 0x6
    stb r0, 0x42(r31)
    b @L_800FC1B8
@L_800FC170:
    lbz r3, 0x23(r31)
    lis r0, 0x4330
    stw r0, 0x10(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r3, 0x14(r1)
    lfd f3, lbl_8047CD20(r2)
    lfd f1, 0x10(r1)
    lfd f0, lbl_8047CD18(r2)
    fsub f1, f1, f2
    fmadd f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    extsb r0, r0
    stb r0, 0x42(r31)
    b @L_800FC1B8
@L_800FC1B0:
    addi r5, r5, 0x8
    bdnz @L_800FC118
@L_800FC1B8:
    li r3, 0x0
@L_800FC1BC:
    lmw r27, 0x2c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
