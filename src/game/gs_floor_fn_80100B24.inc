    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r31, r3
    lwz r0, 0x10(r3)
    cmpwi r0, 0x4
    beq @L_8010100C
    bge @L_80100B60
    cmpwi r0, 0x2
    beq @L_80100CA4
    bge @L_80100F3C
    cmpwi r0, 0x1
    bge @L_80100B70
    b @L_8010122C
@L_80100B60:
    cmpwi r0, 0x6
    beq @L_80101138
    bge @L_8010122C
    b @L_80101070
@L_80100B70:
    lbz r0, 0xb(r31)
    cmplwi r0, 0x0
    bne @L_80100C38
    lwz r29, 0x0(r31)
    lwz r27, -23380(r13)
    b @L_80100C2C
@L_80100B88:
    lwz r0, 0x8(r27)
    lwz r30, 0x4(r27)
    cmpwi r0, 0x1
    bne @L_80100C1C
    lbz r0, 0x15(r27)
    cmplwi r0, 0x0
    bne @L_80100C1C
    lwz r0, 0xc(r27)
    cmpwi r0, 0x0
    bne @L_80100BC4
    lwz r12, 0x18(r27)
    mr r3, r29
    lwz r4, 0x10(r27)
    mtctr r12
    bctrl
@L_80100BC4:
    lwz r0, 0xc(r27)
    cmpwi r0, 0x1
    bne @L_80100C1C
    mr r3, r29
    bl fn_80115A80
    li r0, 0x0
    mr r9, r3
    stw r0, 0x8(r1)
    li r5, 0x4000
    li r6, 0x0
    li r7, 0x0
    stw r0, 0xc(r1)
    li r8, 0x4
    li r10, 0x0
    lbz r3, 0x14(r27)
    lwz r4, 0x18(r27)
    crxor 6, 6, 6
    bl fn_800F7318
    sth r3, 0x20(r27)
    lhz r3, 0x20(r27)
    bl fn_800F7108
    stw r3, 0x1c(r27)
@L_80100C1C:
    lwz r27, 0x4(r27)
    cmplwi r27, 0x0
    bne @L_80100C2C
    mr r27, r30
@L_80100C2C:
    cmplwi r27, 0x0
    bne @L_80100B88
    b @L_80100C98
@L_80100C38:
    lwz r30, 0x0(r31)
    lwz r27, -23380(r13)
    b @L_80100C90
@L_80100C44:
    lwz r0, 0x8(r27)
    lwz r29, 0x4(r27)
    cmpwi r0, 0x1
    bne @L_80100C80
    lbz r0, 0x15(r27)
    cmplwi r0, 0x0
    bne @L_80100C80
    lwz r0, 0xc(r27)
    cmpwi r0, 0x0
    bne @L_80100C80
    lwz r12, 0x18(r27)
    mr r3, r30
    lwz r4, 0x10(r27)
    mtctr r12
    bctrl
@L_80100C80:
    lwz r27, 0x4(r27)
    cmplwi r27, 0x0
    bne @L_80100C90
    mr r27, r29
@L_80100C90:
    cmplwi r27, 0x0
    bne @L_80100C44
@L_80100C98:
    li r0, 0x2
    stw r0, 0x10(r31)
    b @L_8010122C
@L_80100CA4:
    lwz r29, -23408(r13)
    lwz r30, -23404(r13)
    b @L_80100CF8
@L_80100CB0:
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    beq @L_80100CF4
    lwz r0, 0xc(r29)
    cmpwi r0, 0x1
    bne @L_80100CF4
    lbz r0, 0x15(r29)
    cmplwi r0, 0x0
    bne @L_80100CF4
    lwz r3, 0x1c(r29)
    cmplwi r3, 0x0
    beq @L_80100CF4
    bl fn_800F04BC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80100CF4
    li r0, 0x0
    b @L_80100D60
@L_80100CF4:
    addi r29, r29, 0x24
@L_80100CF8:
    cmplwi r30, 0x0
    subi r30, r30, 0x1
    bne @L_80100CB0
    lwz r27, -23408(r13)
    li r30, 0x0
    lwz r28, -23404(r13)
    b @L_80100D50
@L_80100D14:
    lwz r0, 0x8(r27)
    cmpwi r0, 0x0
    beq @L_80100D4C
    lwz r0, 0xc(r27)
    cmpwi r0, 0x1
    bne @L_80100D4C
    lbz r0, 0x15(r27)
    cmplwi r0, 0x0
    bne @L_80100D4C
    lwz r3, 0x1c(r27)
    cmplwi r3, 0x0
    beq @L_80100D4C
    bl fn_800F0494
    stw r30, 0x1c(r27)
@L_80100D4C:
    addi r27, r27, 0x24
@L_80100D50:
    cmplwi r28, 0x0
    subi r28, r28, 0x1
    bne @L_80100D14
    li r0, 0x1
@L_80100D60:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_8010122C
    li r0, 0x3
    stw r0, 0x10(r31)
    lbz r0, 0xb(r31)
    cmplwi r0, 0x0
    beq @L_80100F34
    lwz r0, -23404(r13)
    li r30, 0x0
    lwz r3, -23408(r13)
    mulli r0, r0, 0x24
    lwz r27, 0x4(r31)
    lwz r28, -23400(r13)
    add r29, r3, r0
    b @L_80100DD8
@L_80100D9C:
    lwz r0, 0xc(r29)
    cmpwi r0, 0x0
    bne @L_80100DD4
    lwz r0, 0x10(r29)
    cmplw r0, r27
    bne @L_80100DD4
    stb r30, 0x15(r29)
    lwz r0, 0xc(r29)
    cmpwi r0, 0x1
    bne @L_80100DD4
    lwz r3, 0x1c(r29)
    cmplwi r3, 0x0
    beq @L_80100DD4
    bl fn_800F0424
@L_80100DD4:
    addi r29, r29, 0x24
@L_80100DD8:
    cmplwi r28, 0x0
    subi r28, r28, 0x1
    bne @L_80100D9C
    lwz r3, -23404(r13)
    li r30, 0x0
    lwz r0, -23400(r13)
    lwz r4, -23408(r13)
    add r0, r3, r0
    lwz r29, 0x4(r31)
    mulli r0, r0, 0x24
    lwz r28, -23396(r13)
    add r27, r4, r0
    b @L_80100E48
@L_80100E0C:
    lwz r0, 0xc(r27)
    cmpwi r0, 0x0
    bne @L_80100E44
    lwz r0, 0x10(r27)
    cmplw r0, r29
    bne @L_80100E44
    stb r30, 0x15(r27)
    lwz r0, 0xc(r27)
    cmpwi r0, 0x1
    bne @L_80100E44
    lwz r3, 0x1c(r27)
    cmplwi r3, 0x0
    beq @L_80100E44
    bl fn_800F0424
@L_80100E44:
    addi r27, r27, 0x24
@L_80100E48:
    cmplwi r28, 0x0
    subi r28, r28, 0x1
    bne @L_80100E0C
    lwz r0, -23404(r13)
    li r30, 0x0
    lwz r3, -23408(r13)
    mulli r0, r0, 0x24
    lwz r29, 0x4(r31)
    lwz r28, -23400(r13)
    add r27, r3, r0
    b @L_80100EB0
@L_80100E74:
    lwz r0, 0xc(r27)
    cmpwi r0, 0x1
    bne @L_80100EAC
    lwz r0, 0x10(r27)
    cmplw r0, r29
    bne @L_80100EAC
    stb r30, 0x15(r27)
    lwz r0, 0xc(r27)
    cmpwi r0, 0x1
    bne @L_80100EAC
    lwz r3, 0x1c(r27)
    cmplwi r3, 0x0
    beq @L_80100EAC
    bl fn_800F0424
@L_80100EAC:
    addi r27, r27, 0x24
@L_80100EB0:
    cmplwi r28, 0x0
    subi r28, r28, 0x1
    bne @L_80100E74
    lwz r3, -23404(r13)
    li r30, 0x0
    lwz r0, -23400(r13)
    lwz r4, -23408(r13)
    add r0, r3, r0
    lwz r29, 0x4(r31)
    mulli r0, r0, 0x24
    lwz r28, -23396(r13)
    add r27, r4, r0
    b @L_80100F20
@L_80100EE4:
    lwz r0, 0xc(r27)
    cmpwi r0, 0x1
    bne @L_80100F1C
    lwz r0, 0x10(r27)
    cmplw r0, r29
    bne @L_80100F1C
    stb r30, 0x15(r27)
    lwz r0, 0xc(r27)
    cmpwi r0, 0x1
    bne @L_80100F1C
    lwz r3, 0x1c(r27)
    cmplwi r3, 0x0
    beq @L_80100F1C
    bl fn_800F0424
@L_80100F1C:
    addi r27, r27, 0x24
@L_80100F20:
    cmplwi r28, 0x0
    subi r28, r28, 0x1
    bne @L_80100EE4
    li r0, 0x4
    stw r0, 0x10(r31)
@L_80100F34:
    bl fn_801127BC
    b @L_8010122C
@L_80100F3C:
    lwz r28, 0x0(r31)
    lwz r29, -23380(r13)
    b @L_80100FEC
@L_80100F48:
    lwz r0, 0x8(r29)
    lwz r27, 0x4(r29)
    cmpwi r0, 0x3
    bne @L_80100FDC
    lbz r0, 0x15(r29)
    cmplwi r0, 0x0
    bne @L_80100FDC
    lwz r0, 0xc(r29)
    cmpwi r0, 0x0
    bne @L_80100F84
    lwz r12, 0x18(r29)
    mr r3, r28
    lwz r4, 0x10(r29)
    mtctr r12
    bctrl
@L_80100F84:
    lwz r0, 0xc(r29)
    cmpwi r0, 0x1
    bne @L_80100FDC
    mr r3, r28
    bl fn_80115A80
    li r0, 0x0
    mr r9, r3
    stw r0, 0x8(r1)
    li r5, 0x4000
    li r6, 0x0
    li r7, 0x0
    stw r0, 0xc(r1)
    li r8, 0x4
    li r10, 0x0
    lbz r3, 0x14(r29)
    lwz r4, 0x18(r29)
    crxor 6, 6, 6
    bl fn_800F7318
    sth r3, 0x20(r29)
    lhz r3, 0x20(r29)
    bl fn_800F7108
    stw r3, 0x1c(r29)
@L_80100FDC:
    lwz r29, 0x4(r29)
    cmplwi r29, 0x0
    bne @L_80100FEC
    mr r29, r27
@L_80100FEC:
    cmplwi r29, 0x0
    bne @L_80100F48
    lwz r0, 0x10(r31)
    cmpwi r0, 0x3
    bne @L_8010122C
    li r0, 0x4
    stw r0, 0x10(r31)
    b @L_8010122C
@L_8010100C:
    lwz r28, 0x0(r31)
    lwz r29, -23380(r13)
    b @L_80101064
@L_80101018:
    lwz r0, 0x8(r29)
    lwz r27, 0x4(r29)
    cmpwi r0, 0x3
    bne @L_80101054
    lbz r0, 0x15(r29)
    cmplwi r0, 0x0
    bne @L_80101054
    lwz r0, 0xc(r29)
    cmpwi r0, 0x0
    bne @L_80101054
    lwz r12, 0x18(r29)
    mr r3, r28
    lwz r4, 0x10(r29)
    mtctr r12
    bctrl
@L_80101054:
    lwz r29, 0x4(r29)
    cmplwi r29, 0x0
    bne @L_80101064
    mr r29, r27
@L_80101064:
    cmplwi r29, 0x0
    bne @L_80101018
    b @L_8010122C
@L_80101070:
    bl fn_80112780
    lwz r28, 0x0(r31)
    lwz r29, -23380(r13)
    b @L_80101124
@L_80101080:
    lwz r0, 0x8(r29)
    lwz r27, 0x4(r29)
    cmpwi r0, 0x5
    bne @L_80101114
    lbz r0, 0x15(r29)
    cmplwi r0, 0x0
    bne @L_80101114
    lwz r0, 0xc(r29)
    cmpwi r0, 0x0
    bne @L_801010BC
    lwz r12, 0x18(r29)
    mr r3, r28
    lwz r4, 0x10(r29)
    mtctr r12
    bctrl
@L_801010BC:
    lwz r0, 0xc(r29)
    cmpwi r0, 0x1
    bne @L_80101114
    mr r3, r28
    bl fn_80115A80
    li r0, 0x0
    mr r9, r3
    stw r0, 0x8(r1)
    li r5, 0x4000
    li r6, 0x0
    li r7, 0x0
    stw r0, 0xc(r1)
    li r8, 0x4
    li r10, 0x0
    lbz r3, 0x14(r29)
    lwz r4, 0x18(r29)
    crxor 6, 6, 6
    bl fn_800F7318
    sth r3, 0x20(r29)
    lhz r3, 0x20(r29)
    bl fn_800F7108
    stw r3, 0x1c(r29)
@L_80101114:
    lwz r29, 0x4(r29)
    cmplwi r29, 0x0
    bne @L_80101124
    mr r29, r27
@L_80101124:
    cmplwi r29, 0x0
    bne @L_80101080
    li r0, 0x6
    stw r0, 0x10(r31)
    b @L_8010122C
@L_80101138:
    lwz r3, -23404(r13)
    lwz r0, -23400(r13)
    lwz r4, -23408(r13)
    add r0, r3, r0
    lwz r28, -23396(r13)
    mulli r0, r0, 0x24
    add r27, r4, r0
    b @L_801011A0
@L_80101158:
    lwz r0, 0x8(r27)
    cmpwi r0, 0x0
    beq @L_8010119C
    lwz r0, 0xc(r27)
    cmpwi r0, 0x1
    bne @L_8010119C
    lbz r0, 0x15(r27)
    cmplwi r0, 0x0
    bne @L_8010119C
    lwz r3, 0x1c(r27)
    cmplwi r3, 0x0
    beq @L_8010119C
    bl fn_800F04BC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8010119C
    li r0, 0x0
    b @L_8010121C
@L_8010119C:
    addi r27, r27, 0x24
@L_801011A0:
    cmplwi r28, 0x0
    subi r28, r28, 0x1
    bne @L_80101158
    lwz r3, -23404(r13)
    li r31, 0x0
    lwz r0, -23400(r13)
    lwz r4, -23408(r13)
    add r0, r3, r0
    lwz r28, -23396(r13)
    mulli r0, r0, 0x24
    add r27, r4, r0
    b @L_8010120C
@L_801011D0:
    lwz r0, 0x8(r27)
    cmpwi r0, 0x0
    beq @L_80101208
    lwz r0, 0xc(r27)
    cmpwi r0, 0x1
    bne @L_80101208
    lbz r0, 0x15(r27)
    cmplwi r0, 0x0
    bne @L_80101208
    lwz r3, 0x1c(r27)
    cmplwi r3, 0x0
    beq @L_80101208
    bl fn_800F0494
    stw r31, 0x1c(r27)
@L_80101208:
    addi r27, r27, 0x24
@L_8010120C:
    cmplwi r28, 0x0
    subi r28, r28, 0x1
    bne @L_801011D0
    li r0, 0x1
@L_8010121C:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_8010122C
    li r3, 0x0
    b @L_80101230
@L_8010122C:
    li r3, 0x1
@L_80101230:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
