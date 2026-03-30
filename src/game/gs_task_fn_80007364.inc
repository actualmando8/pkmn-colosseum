    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    mr r30, r3
    li r0, 0x0
    li r3, 0x0
    stb r0, 0xb(r1)
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    stb r0, 0xa(r1)
    stb r0, 0x9(r1)
    stb r0, 0x8(r1)
    bl fn_801F54A4
    clrlwi r31, r3, 16
    mr r3, r30
    bl fn_80205BE8
    mr r27, r3
    cmplwi r27, 0x0
    bne @L_800073C0
    li r3, -0x1
    b @L_80007648
    @L_800073C0:
    mr r4, r30
    li r3, 0x0
    bl fn_801F4460
    mr r28, r3
    cmplwi r28, 0x0
    bne @L_800073E0
    li r3, -0x1
    b @L_80007648
    @L_800073E0:
    mr r3, r30
    bl fn_80203E7C
    mr r29, r3
    mr r3, r28
    mr r4, r30
    bl fn_801F8C00
    clrlwi r0, r3, 24
    cmplwi r0, 0x2
    bne @L_800075E8
    mr r3, r28
    mr r4, r30
    bl fn_801F8D80
    mr r25, r3
    bl fn_801FECD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000742C
    li r3, -0x1
    b @L_80007648
    @L_8000742C:
    cmplwi r25, 0x0
    beq @L_80007460
    mr r3, r25
    bl fn_80207BF4
    li r22, 0x0
    b @L_80007454
    @L_80007444:
    mr r3, r25
    li r4, 0x0
    bl fn_80207B8C
    addi r22, r22, 0x1
    @L_80007454:
    clrlwi r0, r22, 16
    cmplwi r0, 0x2
    blt @L_80007444
    @L_80007460:
    mr r3, r27
    addi r5, r1, 0xb
    addi r6, r1, 0xa
    addi r7, r1, 0x9
    addi r8, r1, 0x8
    li r4, 0x1
    bl fn_800096B4
    mr r26, r3
    cmpwi r26, 0x1
    bne @L_80007608
    mr r3, r27
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    cmplwi r25, 0x0
    clrlwi r24, r3, 16
    beq @L_80007608
    mr r3, r25
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr r23, r3
    cmplwi r23, 0x0
    beq @L_80007550
    mr r3, r27
    li r4, 0x0
    li r5, 0x0
    bl fn_8010B01C
    b @L_800074E0
    @L_800074DC:
    bl fn_800F0308
    @L_800074E0:
    mr r3, r27
    bl fn_8010BBB8
    bl fn_8010BCE4
    extsb r0, r3
    cmpwi r0, 0x0
    beq @L_800074DC
    mr r3, r27
    bl fn_80121C18
    mr r0, r3
    mr r3, r23
    mr r22, r0
    mr r4, r22
    bl fn_801C3D64
    bl fn_801C3430
    mr r3, r25
    mr r7, r22
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_801254B4
    mr r3, r23
    bl fn_801DB100
    mr r3, r22
    li r4, 0x1
    bl fn_801DA4E8
    mr r3, r25
    li r4, 0x14
    bl fn_80202810
    @L_80007550:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r4, r3, 16
    mr r3, r25
    li r5, 0x1
    bl fn_802653FC
    mr r3, r25
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    mr r3, r27
    bl fn_80122040
    mr r3, r27
    bl fn_801248C4
    mr r4, r3
    mr r3, r25
    bl fn_80207BC0
    li r27, 0x0
    b @L_800075D8
    @L_800075B0:
    mr r4, r24
    mr r6, r27
    li r3, 0x0
    li r5, 0x16
    bl fn_8012640C
    clrlwi r5, r3, 16
    mr r3, r25
    clrlwi r4, r27, 24
    bl fn_80207B5C
    addi r27, r27, 0x1
    @L_800075D8:
    clrlwi r0, r27, 16
    cmplwi r0, 0x2
    blt @L_800075B0
    b @L_80007608
    @L_800075E8:
    mr r3, r27
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_800096B4
    mr r26, r3
    @L_80007608:
    mr r3, r30
    bl fn_80203E7C
    clrlwi r3, r3, 24
    clrlwi r0, r29, 24
    cmplw r3, r0
    ble @L_80007638
    mr r3, r30
    li r4, 0x0
    li r5, 0xd0
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    @L_80007638:
    mr r3, r28
    mr r4, r31
    bl fn_80265754
    mr r3, r26
    @L_80007648:
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
