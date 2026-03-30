    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr. r23, r3
    beq @L_800EC0D4
    lis r4, 0x2
    bl fn_8019FE8C
    lwz r0, 0x14(r23)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EC0D4
    lwz r31, 0x10(r23)
    b @L_800EC0CC
    @L_800EBF20:
    cmplwi r31, 0x0
    beq @L_800EC0C8
    mr r3, r31
    lis r4, 0x2
    bl fn_8019FE8C
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EC0C8
    lwz r30, 0x10(r31)
    b @L_800EC0C0
    @L_800EBF48:
    cmplwi r30, 0x0
    beq @L_800EC0BC
    mr r3, r30
    lis r4, 0x2
    bl fn_8019FE8C
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EC0BC
    lwz r29, 0x10(r30)
    b @L_800EC0B4
    @L_800EBF70:
    cmplwi r29, 0x0
    beq @L_800EC0B0
    mr r3, r29
    lis r4, 0x2
    bl fn_8019FE8C
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EC0B0
    lwz r28, 0x10(r29)
    b @L_800EC0A8
    @L_800EBF98:
    cmplwi r28, 0x0
    beq @L_800EC0A4
    mr r3, r28
    lis r4, 0x2
    bl fn_8019FE8C
    lwz r0, 0x14(r28)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EC0A4
    lwz r27, 0x10(r28)
    b @L_800EC09C
    @L_800EBFC0:
    cmplwi r27, 0x0
    beq @L_800EC098
    mr r3, r27
    lis r4, 0x2
    bl fn_8019FE8C
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EC098
    lwz r26, 0x10(r27)
    b @L_800EC090
    @L_800EBFE8:
    cmplwi r26, 0x0
    beq @L_800EC08C
    mr r3, r26
    lis r4, 0x2
    bl fn_8019FE8C
    lwz r0, 0x14(r26)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EC08C
    lwz r25, 0x10(r26)
    b @L_800EC084
    @L_800EC010:
    cmplwi r25, 0x0
    beq @L_800EC080
    mr r3, r25
    lis r4, 0x2
    bl fn_8019FE8C
    lwz r0, 0x14(r25)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EC080
    lwz r24, 0x10(r25)
    b @L_800EC078
    @L_800EC038:
    cmplwi r24, 0x0
    beq @L_800EC074
    mr r3, r24
    lis r4, 0x2
    bl fn_8019FE8C
    lwz r0, 0x14(r24)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_800EC074
    lwz r23, 0x10(r24)
    b @L_800EC06C
    @L_800EC060:
    mr r3, r23
    bl fn_800EBEEC
    lwz r23, 0x8(r23)
    @L_800EC06C:
    cmplwi r23, 0x0
    bne @L_800EC060
    @L_800EC074:
    lwz r24, 0x8(r24)
    @L_800EC078:
    cmplwi r24, 0x0
    bne @L_800EC038
    @L_800EC080:
    lwz r25, 0x8(r25)
    @L_800EC084:
    cmplwi r25, 0x0
    bne @L_800EC010
    @L_800EC08C:
    lwz r26, 0x8(r26)
    @L_800EC090:
    cmplwi r26, 0x0
    bne @L_800EBFE8
    @L_800EC098:
    lwz r27, 0x8(r27)
    @L_800EC09C:
    cmplwi r27, 0x0
    bne @L_800EBFC0
    @L_800EC0A4:
    lwz r28, 0x8(r28)
    @L_800EC0A8:
    cmplwi r28, 0x0
    bne @L_800EBF98
    @L_800EC0B0:
    lwz r29, 0x8(r29)
    @L_800EC0B4:
    cmplwi r29, 0x0
    bne @L_800EBF70
    @L_800EC0BC:
    lwz r30, 0x8(r30)
    @L_800EC0C0:
    cmplwi r30, 0x0
    bne @L_800EBF48
    @L_800EC0C8:
    lwz r31, 0x8(r31)
    @L_800EC0CC:
    cmplwi r31, 0x0
    bne @L_800EBF20
    @L_800EC0D4:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
