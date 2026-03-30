    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r23, 0x1c(r1)
    mr r31, r3
    li r0, 0x0
    lis r3, -32698
    stb r0, 0x8(r1)
    addi r28, r3, 28852
    addi r27, r1, 0x8
    li r26, 0x0
    stb r0, 0x9(r1)
    stb r0, 0xa(r1)
    stb r0, 0xb(r1)
    stb r0, 0xc(r1)
    stb r0, 0xd(r1)
@L_801CAB34:
    lwz r0, 0x4(r28)
    cmplwi r0, 0x0
    beq @L_801CABF4
    mr r30, r27
    addi r29, r28, 0x4
    li r25, 0x0
@L_801CAB4C:
    lbz r0, 0x0(r30)
    cmplwi r0, 0x0
    bne @L_801CABC8
    mr r3, r31
    rlwinm r5, r25, 0, 16, 31
    li r4, 0x3
    bl fn_8012A5B0
    mr r23, r3
    cmplwi r23, 0x0
    beq @L_801CABC8
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CABC8
    mr r3, r23
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r24, r3, 0, 16, 31
    mr r3, r23
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    lwz r0, 0x0(r29)
    cmplw r24, r0
    bne @L_801CABC8
    lwz r0, 0x0(r28)
    cmplw r3, r0
    beq @L_801CABD8
@L_801CABC8:
    addi r30, r30, 0x1
    addi r25, r25, 0x1
    cmpwi r25, 0x6
    blt @L_801CAB4C
@L_801CABD8:
    cmpwi r25, 0x6
    blt @L_801CABE8
    li r3, 0x1
    b @L_801CAC58
@L_801CABE8:
    addi r3, r1, 0x8
    li r0, 0x1
    stbx r0, r3, r25
@L_801CABF4:
    addi r28, r28, 0x8
    addi r26, r26, 0x1
    cmpwi r26, 0x6
    blt @L_801CAB34
    li r28, 0x0
@L_801CAC08:
    lbz r0, 0x0(r27)
    cmplwi r0, 0x0
    bne @L_801CAC44
    mr r3, r31
    rlwinm r5, r28, 0, 16, 31
    li r4, 0x3
    bl fn_8012A5B0
    cmplwi r3, 0x0
    beq @L_801CAC44
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CAC44
    li r3, 0x1
    b @L_801CAC58
@L_801CAC44:
    addi r27, r27, 0x1
    addi r28, r28, 0x1
    cmpwi r28, 0x6
    blt @L_801CAC08
    li r3, 0x0
@L_801CAC58:
    lmw r23, 0x1c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
