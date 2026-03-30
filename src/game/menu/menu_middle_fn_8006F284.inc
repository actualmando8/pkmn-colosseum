    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    lis r3, lbl_80267EA8@ha
    addi r30, r3, lbl_80267EA8@l
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8006F358
    bge @L_8006F2C0
    cmpwi r0, 0x0
    beq @L_8006F2C8
    b @L_8006F370
    @L_8006F2C0:
    cmpwi r0, 0x6
    b @L_8006F370
    @L_8006F2C8:
    addi r26, r30, 0x4ec
    li r25, 0x0
    @L_8006F2D0:
    lhz r4, 0x0(r26)
    mr r3, r31
    bl fn_801046C8
    lwz r0, 0x4(r26)
    addi r26, r26, 0x8
    addi r25, r25, 0x1
    stw r0, 0x4c(r3)
    cmplwi r25, 0x10
    blt @L_8006F2D0
    bl fn_80077BD0
    clrlwi r0, r3, 24
    li r3, 0x9fc
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r25, r0, 24
    mr r4, r25
    bl fn_8005D8F8
    cmplwi r25, 0x0
    bne @L_8006F370
    lwz r3, 0x4(r31)
    li r4, 0x9fd
    bl fn_80102138
    extsb r3, r3
    li r0, 0x0
    stb r3, 0xd(r1)
    addi r4, r1, 0x8
    stb r0, 0xc(r1)
    lhz r0, 0xc(r1)
    sth r0, 0x8(r1)
    lwz r3, 0x4(r31)
    bl fn_801044D0
    mr r3, r31
    bl fn_80104CA0
    b @L_8006F370
    @L_8006F358:
    lwz r3, 0x4(r31)
    bl fn_801022B8
    cmpwi r3, 0xe35
    bne @L_8006F370
    li r0, 0x1
    stb r0, 0x98(r31)
    @L_8006F370:
    lwz r3, 0x4(r31)
    bl fn_801022B8
    cmpwi r3, 0x9fe
    bge @L_8006F3AC
    cmpwi r3, 0x9f9
    beq @L_8006F3E8
    bge @L_8006F39C
    cmpwi r3, 0x9f7
    beq @L_8006F3D8
    bge @L_8006F3E0
    b @L_8006F400
    @L_8006F39C:
    cmpwi r3, 0x9fb
    beq @L_8006F3F8
    bge @L_8006F400
    b @L_8006F3F0
    @L_8006F3AC:
    cmpwi r3, 0xe33
    bge @L_8006F3C8
    cmpwi r3, 0xa0e
    bge @L_8006F400
    cmpwi r3, 0xa0c
    bge @L_8006F3F0
    b @L_8006F400
    @L_8006F3C8:
    cmpwi r3, 0xe35
    beq @L_8006F400
    bge @L_8006F400
    b @L_8006F3F8
    @L_8006F3D8:
    li r25, 0x0
    b @L_8006F404
    @L_8006F3E0:
    li r25, 0x1
    b @L_8006F404
    @L_8006F3E8:
    li r25, 0x2
    b @L_8006F404
    @L_8006F3F0:
    li r25, 0x3
    b @L_8006F404
    @L_8006F3F8:
    li r25, 0x4
    b @L_8006F404
    @L_8006F400:
    li r25, 0x5
    @L_8006F404:
    addi r27, r30, 0x4e0
    li r28, 0x0
    @L_8006F40C:
    lhz r4, 0x0(r27)
    mr r3, r31
    bl fn_801046C8
    cmpw r25, r28
    mr r29, r3
    li r26, 0x0
    bne @L_8006F43C
    bl fn_80077BD0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8006F43C
    li r26, 0x1
    @L_8006F43C:
    clrlwi r4, r26, 24
    mr r3, r29
    bl fn_80109220
    li r3, 0x191
    bl fn_8005D830
    lwz r0, 0xc(r29)
    cmplw r0, r3
    beq @L_8006F46C
    lhz r4, 0x0(r27)
    mr r3, r31
    li r5, 0x191
    bl fn_801081F8
    @L_8006F46C:
    addi r27, r27, 0x2
    addi r28, r28, 0x1
    cmplwi r28, 0x5
    blt @L_8006F40C
    bl fn_80077BD0
    clrlwi r0, r3, 24
    mr r3, r31
    cntlzw r0, r0
    li r4, 0x0
    srwi r0, r0, 5
    clrlwi r29, r0, 24
    bl fn_801040D0
    lha r5, 0x14(r3)
    li r4, 0xa06
    lha r0, 0x16(r3)
    srwi r5, r5, 31
    lbz r25, 0x11(r3)
    srwi r0, r0, 31
    lbz r26, 0x12(r3)
    xori r5, r5, 0x1
    lbz r24, 0x13(r3)
    xori r0, r0, 0x1
    mr r3, r31
    clrlwi r28, r5, 24
    clrlwi r27, r0, 24
    bl fn_801046C8
    mr r4, r25
    bl fn_80109220
    mr r3, r31
    li r4, 0xa07
    bl fn_801046C8
    cntlzw r0, r25
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r31
    li r4, 0xa08
    bl fn_801046C8
    mr r4, r26
    bl fn_80109220
    mr r3, r31
    li r4, 0xa09
    bl fn_801046C8
    cntlzw r0, r26
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r31
    li r4, 0xa0a
    bl fn_801046C8
    mr r4, r24
    bl fn_80109220
    mr r3, r31
    li r4, 0xa0b
    bl fn_801046C8
    cntlzw r0, r24
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r31
    li r4, 0x11a2
    bl fn_801046C8
    cmplwi r28, 0x0
    mr r25, r3
    li r26, 0x0
    beq @L_8006F598
    lwz r3, 0x4(r31)
    bl fn_801022B8
    cmpwi r3, 0xa0c
    beq @L_8006F598
    lwz r3, 0x4(r31)
    bl fn_801022B8
    cmpwi r3, 0xa0d
    beq @L_8006F598
    li r26, 0x1
    @L_8006F598:
    clrlwi r4, r26, 24
    mr r3, r25
    bl fn_80109220
    mr r3, r31
    li r4, 0x11a3
    bl fn_801046C8
    cntlzw r0, r28
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r31
    li r4, 0x11a4
    bl fn_801046C8
    cmplwi r27, 0x0
    mr r25, r3
    li r26, 0x0
    beq @L_8006F600
    lwz r3, 0x4(r31)
    bl fn_801022B8
    cmpwi r3, 0xe34
    beq @L_8006F600
    lwz r3, 0x4(r31)
    bl fn_801022B8
    cmpwi r3, 0xe33
    beq @L_8006F600
    li r26, 0x1
    @L_8006F600:
    clrlwi r4, r26, 24
    mr r3, r25
    bl fn_80109220
    mr r3, r31
    li r4, 0x11a5
    bl fn_801046C8
    cntlzw r0, r27
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    mr r3, r31
    li r4, 0xa16
    bl fn_801046C8
    mr r4, r29
    bl fn_80109220
    mr r3, r31
    li r4, 0xd9f
    bl fn_801046C8
    mr r4, r29
    bl fn_80109220
    mr r3, r31
    li r4, 0xa15
    bl fn_801046C8
    cmplwi r29, 0x0
    beq @L_8006F66C
    li r0, 0x3d7a
    b @L_8006F670
    @L_8006F66C:
    li r0, 0x3d79
    @L_8006F670:
    stw r0, 0x4c(r3)
    mr r3, r31
    li r4, 0x11a6
    bl fn_801046C8
    cmplwi r29, 0x0
    mr r25, r3
    li r26, 0x0
    beq @L_8006F6AC
    cmplwi r28, 0x0
    beq @L_8006F6AC
    lwz r3, 0x4(r31)
    bl fn_801022B8
    cmpwi r3, 0x9fa
    bne @L_8006F6AC
    li r26, 0x1
    @L_8006F6AC:
    clrlwi r4, r26, 24
    mr r3, r25
    bl fn_80109220
    mr r3, r31
    li r4, 0x11a7
    bl fn_801046C8
    cmplwi r29, 0x0
    mr r26, r3
    li r28, 0x0
    beq @L_8006F6F0
    cmplwi r27, 0x0
    beq @L_8006F6F0
    lwz r3, 0x4(r31)
    bl fn_801022B8
    cmpwi r3, 0x9fb
    bne @L_8006F6F0
    li r28, 0x1
    @L_8006F6F0:
    clrlwi r4, r28, 24
    mr r3, r26
    bl fn_80109220
    mr r3, r31
    addi r4, r30, 0x56c
    li r5, 0x2
    bl fn_80070D84
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
