    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x78(r1)
    stmw r24, 0x58(r1)
    addi r26, r3, 0x0
    addi r27, r4, 0x0
    bl OSDisableInterrupts
    addi r28, r3, 0x0
    lis r25, 0xcc00
    lis r31, 0x4000
@L_800CE804:
    lwz r0, 0x6828(r25)
    addi r30, r25, 0x6800
    addi r3, r1, 0x50
    andi. r0, r0, 0x405
    li r4, 0x2
    li r5, 0x1
    ori r0, r0, 0xc0
    stwu r0, 0x28(r30)
    stw r31, 0x50(r1)
    bl fn_800CEFBC
    cntlzw r0, r3
    srwi r24, r0, 5
@L_800CE834:
    addi r29, r25, 0x6800
    lwzu r0, 0x34(r29)
    clrlwi. r0, r0, 31
    bne @L_800CE834
    addi r3, r1, 0x54
    li r4, 0x4
    li r5, 0x0
    bl fn_800CEFBC
@L_800CE854:
    lwz r0, 0x0(r29)
    clrlwi. r0, r0, 31
    bne @L_800CE854
    lwz r0, 0x0(r30)
    andi. r0, r0, 0x405
    stw r0, 0x0(r30)
    lwz r0, 0x54(r1)
    rlwinm. r0, r0, 0, 30, 30
    bne @L_800CE804
    lbz r3, -32080(r13)
    addi r0, r3, 0x1
    stb r0, -32080(r13)
    lbz r0, -32080(r13)
    clrlwi. r0, r0, 31
    beq @L_800CE898
    li r3, 0x1000
    b @L_800CE89C
@L_800CE898:
    li r3, 0x0
@L_800CE89C:
    addi r0, r27, 0x3
    oris r25, r3, 0x1
    clrrwi r24, r0, 2
    ori r25, r25, 0xc000
@L_800CE8AC:
    addi r3, r25, 0x0
    addi r4, r26, 0x0
    addi r5, r24, 0x0
    bl fn_800CED58
    cmpwi r3, 0x0
    beq @L_800CE8AC
    lis r31, 0x4000
@L_800CE8C8:
    lwz r0, 0x0(r30)
    addi r3, r1, 0x4c
    li r4, 0x2
    andi. r0, r0, 0x405
    li r5, 0x1
    ori r0, r0, 0xc0
    stw r0, 0x0(r30)
    stw r31, 0x4c(r1)
    bl fn_800CEFBC
    cntlzw r0, r3
    srwi r26, r0, 5
@L_800CE8F4:
    lwz r0, 0x0(r29)
    clrlwi. r0, r0, 31
    bne @L_800CE8F4
    addi r3, r1, 0x54
    li r4, 0x4
    li r5, 0x0
    bl fn_800CEFBC
@L_800CE910:
    lwz r0, 0x0(r29)
    clrlwi. r0, r0, 31
    bne @L_800CE910
    lwz r0, 0x0(r30)
    andi. r0, r0, 0x405
    stw r0, 0x0(r30)
    lwz r0, 0x54(r1)
    rlwinm. r0, r0, 0, 30, 30
    bne @L_800CE8C8
    lbz r0, -32080(r13)
    slwi r0, r0, 16
    oris r0, r0, 0x1f00
    or r0, r0, r27
    clrlwi r0, r0, 3
    oris r24, r0, 0xc000
@L_800CE94C:
    lwz r0, 0x0(r30)
    addi r3, r1, 0x44
    li r4, 0x4
    andi. r0, r0, 0x405
    li r5, 0x1
    ori r0, r0, 0xc0
    stw r0, 0x0(r30)
    stw r24, 0x44(r1)
    bl fn_800CEFBC
    cntlzw r0, r3
    srwi r3, r0, 5
@L_800CE978:
    lwz r0, 0x0(r29)
    clrlwi. r0, r0, 31
    bne @L_800CE978
    lwz r0, 0x0(r30)
    andi. r0, r0, 0x405
    cmpwi r3, 0x0
    stw r0, 0x0(r30)
    bne @L_800CE94C
    lis r27, 0x4000
@L_800CE99C:
    lwz r0, 0x0(r30)
    addi r3, r1, 0x3c
    li r4, 0x2
    andi. r0, r0, 0x405
    li r5, 0x1
    ori r0, r0, 0xc0
    stw r0, 0x0(r30)
    stw r27, 0x3c(r1)
    bl fn_800CEFBC
    cntlzw r0, r3
    srwi r26, r0, 5
@L_800CE9C8:
    lwz r0, 0x0(r29)
    clrlwi. r0, r0, 31
    bne @L_800CE9C8
    addi r3, r1, 0x54
    li r4, 0x4
    li r5, 0x0
    bl fn_800CEFBC
    cntlzw r0, r3
    srwi r0, r0, 5
    or r3, r26, r0
@L_800CE9F0:
    lwz r0, 0x0(r29)
    clrlwi. r0, r0, 31
    bne @L_800CE9F0
    lwz r0, 0x0(r30)
    andi. r0, r0, 0x405
    cmpwi r3, 0x0
    stw r0, 0x0(r30)
    bne @L_800CE99C
    lwz r0, 0x54(r1)
    rlwinm. r0, r0, 0, 30, 30
    bne @L_800CE99C
    mr r3, r28
    bl OSRestoreInterrupts
    lmw r24, 0x58(r1)
    li r3, 0x0
    lwz r0, 0x7c(r1)
    addi r1, r1, 0x78
    mtlr r0
    blr
