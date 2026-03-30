    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r20, 0x10(r1)
    mr. r31, r3
    mr r30, r4
    mr r29, r5
    mr r28, r6
    beq @L_801A3008
    cmplwi r31, 0x0
    beq @L_801A2D58
    cmplwi r30, 0x0
    beq @L_801A2D14
    lwz r0, 0x7c(r31)
    cmplwi r0, 0x0
    beq @L_801A2BA4
    lwz r3, 0x7c(r31)
    bl fn_801C25E4
@L_801A2BA4:
    lwz r3, 0x8(r30)
    bl fn_801C2670
    stw r3, 0x7c(r31)
    lwz r6, 0x7c(r31)
    cmplwi r6, 0x0
    beq @L_801A2C10
    lwz r0, 0x14(r6)
    cmplwi r0, 0x0
    beq @L_801A2C10
    addi r5, r6, 0x14
    b @L_801A2C04
@L_801A2BD0:
    lwz r3, 0x0(r5)
    lbz r0, 0x13(r3)
    cmplwi r0, 0xc
    bne @L_801A2C00
    lwz r3, 0x0(r5)
    lwz r4, 0x0(r5)
    lwz r0, 0x0(r3)
    stw r0, 0x0(r5)
    lwz r0, 0x14(r6)
    stw r0, 0x0(r4)
    stw r4, 0x14(r6)
    b @L_801A2C10
@L_801A2C00:
    lwz r5, 0x0(r5)
@L_801A2C04:
    lwz r0, 0x0(r5)
    cmplwi r0, 0x0
    bne @L_801A2BD0
@L_801A2C10:
    lwz r3, 0x80(r31)
    lwz r4, 0xc(r30)
    bl fn_801AFE68
    lwz r0, 0x10(r30)
    clrlwi. r0, r0, 31
    beq @L_801A2CA0
    cmplwi r31, 0x0
    beq @L_801A2D14
    lwz r0, 0x14(r31)
    xori r0, r0, 0x8
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801A2C90
    cmplwi r31, 0x0
    beq @L_801A2C90
    cmplwi r31, 0x0
    bne @L_801A2C60
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2C60:
    lwz r0, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2C80
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2C80
    li r3, 0x1
@L_801A2C80:
    cmpwi r3, 0x0
    bne @L_801A2C90
    mr r3, r31
    bl fn_8019D620
@L_801A2C90:
    lwz r0, 0x14(r31)
    ori r0, r0, 0x8
    stw r0, 0x14(r31)
    b @L_801A2D14
@L_801A2CA0:
    cmplwi r31, 0x0
    beq @L_801A2D14
    lwz r0, 0x14(r31)
    xori r0, r0, 0x8
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801A2D08
    cmplwi r31, 0x0
    beq @L_801A2D08
    cmplwi r31, 0x0
    bne @L_801A2CD8
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2CD8:
    lwz r0, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2CF8
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2CF8
    li r3, 0x1
@L_801A2CF8:
    cmpwi r3, 0x0
    bne @L_801A2D08
    mr r3, r31
    bl fn_8019D620
@L_801A2D08:
    lwz r0, 0x14(r31)
    rlwinm r0, r0, 0, 29, 27
    stw r0, 0x14(r31)
@L_801A2D14:
    lwz r0, 0x14(r31)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A2D58
    cmplwi r28, 0x0
    beq @L_801A2D38
    lwz r5, 0x8(r28)
    b @L_801A2D3C
@L_801A2D38:
    li r5, 0x0
@L_801A2D3C:
    cmplwi r29, 0x0
    lwz r3, 0x18(r31)
    beq @L_801A2D50
    lwz r4, 0x8(r29)
    b @L_801A2D54
@L_801A2D50:
    li r4, 0x0
@L_801A2D54:
    bl fn_80199654
@L_801A2D58:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3008
    cmplwi r30, 0x0
    lwz r31, 0x10(r31)
    beq @L_801A2D78
    lwz r30, 0x0(r30)
    b @L_801A2D7C
@L_801A2D78:
    li r30, 0x0
@L_801A2D7C:
    cmplwi r29, 0x0
    beq @L_801A2D8C
    lwz r29, 0x0(r29)
    b @L_801A2D90
@L_801A2D8C:
    li r29, 0x0
@L_801A2D90:
    cmplwi r28, 0x0
    beq @L_801A2DA0
    lwz r28, 0x0(r28)
    b @L_801A3000
@L_801A2DA0:
    li r28, 0x0
    b @L_801A3000
@L_801A2DA8:
    cmplwi r31, 0x0
    beq @L_801A2FC0
    cmplwi r31, 0x0
    beq @L_801A2E60
    cmplwi r30, 0x0
    beq @L_801A2E1C
    lwz r0, 0x7c(r31)
    cmplwi r0, 0x0
    beq @L_801A2DD4
    lwz r3, 0x7c(r31)
    bl fn_801C25E4
@L_801A2DD4:
    lwz r3, 0x8(r30)
    bl fn_801C2670
    stw r3, 0x7c(r31)
    lwz r3, 0x7c(r31)
    bl fn_801A323C
    lwz r3, 0x80(r31)
    lwz r4, 0xc(r30)
    bl fn_801AFE68
    lwz r0, 0x10(r30)
    clrlwi. r0, r0, 31
    beq @L_801A2E10
    mr r3, r31
    li r4, 0x8
    bl fn_8019FE8C
    b @L_801A2E1C
@L_801A2E10:
    mr r3, r31
    li r4, 0x8
    bl fn_8019FAEC
@L_801A2E1C:
    lwz r0, 0x14(r31)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A2E60
    cmplwi r28, 0x0
    beq @L_801A2E40
    lwz r5, 0x8(r28)
    b @L_801A2E44
@L_801A2E40:
    li r5, 0x0
@L_801A2E44:
    cmplwi r29, 0x0
    lwz r3, 0x18(r31)
    beq @L_801A2E58
    lwz r4, 0x8(r29)
    b @L_801A2E5C
@L_801A2E58:
    li r4, 0x0
@L_801A2E5C:
    bl fn_80199654
@L_801A2E60:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A2FC0
    cmplwi r30, 0x0
    lwz r23, 0x10(r31)
    beq @L_801A2E80
    lwz r22, 0x0(r30)
    b @L_801A2E84
@L_801A2E80:
    li r22, 0x0
@L_801A2E84:
    cmplwi r29, 0x0
    beq @L_801A2E94
    lwz r21, 0x0(r29)
    b @L_801A2E98
@L_801A2E94:
    li r21, 0x0
@L_801A2E98:
    cmplwi r28, 0x0
    beq @L_801A2EA8
    lwz r20, 0x0(r28)
    b @L_801A2FB8
@L_801A2EA8:
    li r20, 0x0
    b @L_801A2FB8
@L_801A2EB0:
    cmplwi r23, 0x0
    beq @L_801A2F78
    mr r3, r23
    mr r4, r22
    mr r5, r21
    mr r6, r20
    bl fn_801A301C
    lwz r0, 0x14(r23)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A2F78
    cmplwi r22, 0x0
    lwz r24, 0x10(r23)
    beq @L_801A2EEC
    lwz r25, 0x0(r22)
    b @L_801A2EF0
@L_801A2EEC:
    li r25, 0x0
@L_801A2EF0:
    cmplwi r21, 0x0
    beq @L_801A2F00
    lwz r26, 0x0(r21)
    b @L_801A2F04
@L_801A2F00:
    li r26, 0x0
@L_801A2F04:
    cmplwi r20, 0x0
    beq @L_801A2F14
    lwz r27, 0x0(r20)
    b @L_801A2F70
@L_801A2F14:
    li r27, 0x0
    b @L_801A2F70
@L_801A2F1C:
    mr r3, r24
    mr r4, r25
    mr r5, r26
    mr r6, r27
    bl fn_801A2B5C
    cmplwi r25, 0x0
    lwz r24, 0x8(r24)
    beq @L_801A2F44
    lwz r25, 0x4(r25)
    b @L_801A2F48
@L_801A2F44:
    li r25, 0x0
@L_801A2F48:
    cmplwi r26, 0x0
    beq @L_801A2F58
    lwz r26, 0x4(r26)
    b @L_801A2F5C
@L_801A2F58:
    li r26, 0x0
@L_801A2F5C:
    cmplwi r27, 0x0
    beq @L_801A2F6C
    lwz r27, 0x4(r27)
    b @L_801A2F70
@L_801A2F6C:
    li r27, 0x0
@L_801A2F70:
    cmplwi r24, 0x0
    bne @L_801A2F1C
@L_801A2F78:
    cmplwi r22, 0x0
    lwz r23, 0x8(r23)
    beq @L_801A2F8C
    lwz r22, 0x4(r22)
    b @L_801A2F90
@L_801A2F8C:
    li r22, 0x0
@L_801A2F90:
    cmplwi r21, 0x0
    beq @L_801A2FA0
    lwz r21, 0x4(r21)
    b @L_801A2FA4
@L_801A2FA0:
    li r21, 0x0
@L_801A2FA4:
    cmplwi r20, 0x0
    beq @L_801A2FB4
    lwz r20, 0x4(r20)
    b @L_801A2FB8
@L_801A2FB4:
    li r20, 0x0
@L_801A2FB8:
    cmplwi r23, 0x0
    bne @L_801A2EB0
@L_801A2FC0:
    cmplwi r30, 0x0
    lwz r31, 0x8(r31)
    beq @L_801A2FD4
    lwz r30, 0x4(r30)
    b @L_801A2FD8
@L_801A2FD4:
    li r30, 0x0
@L_801A2FD8:
    cmplwi r29, 0x0
    beq @L_801A2FE8
    lwz r29, 0x4(r29)
    b @L_801A2FEC
@L_801A2FE8:
    li r29, 0x0
@L_801A2FEC:
    cmplwi r28, 0x0
    beq @L_801A2FFC
    lwz r28, 0x4(r28)
    b @L_801A3000
@L_801A2FFC:
    li r28, 0x0
@L_801A3000:
    cmplwi r31, 0x0
    bne @L_801A2DA8
@L_801A3008:
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
