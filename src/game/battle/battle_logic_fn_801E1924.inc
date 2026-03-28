    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, -32728
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    addi r31, r4, -26008
    lwz r0, 0x4(r3)
    lwz r30, 0x0(r3)
    clrlwi r29, r0, 24
    bl fn_800EE9BC
    mr r3, r30
    mr r4, r29
    bl fn_801E4778
    cmpwi r3, 0x0
    bne @L_801E1984
    mr r4, r30
    addi r3, r31, 0x0
    crxor 6, 6, 6
    bl fn_800DD970
    bl fn_800EE928
    li r0, 0x1
    li r3, 0x0
    stw r0, -21468(r13)
    b @L_801E1B18
@L_801E1984:
    lis r3, -32697
    addi r3, r3, -28624
    bl fn_801E3930
    lis r3, -32697
    addi r3, r3, -28640
    bl fn_801E38E8
    lis r4, -32698
    lis r3, -32697
    addi r5, r4, 27584
    lwzu r4, -28624(r3)
    stw r5, -21444(r13)
    lwz r3, 0x4(r3)
    lhz r0, 0x4(r5)
    subf r0, r4, r0
    srwi r0, r0, 1
    stw r0, -21448(r13)
    lhz r0, 0x6(r5)
    subf r0, r3, r0
    srwi r0, r0, 1
    stw r0, -21452(r13)
    bl fn_801E4650
    li r4, 0x20
    bl fn_800E2B00
    clrlwi. r0, r3, 16
    beq @L_801E19F0
    bl fn_800E27B0
    b @L_801E19F4
@L_801E19F0:
    li r3, 0x0
@L_801E19F4:
    cmplwi r3, 0x0
    stw r3, -21456(r13)
    bne @L_801E1A20
    addi r3, r31, 0x30
    crxor 6, 6, 6
    bl fn_800DD970
    bl fn_800EE928
    li r0, 0x1
    li r3, 0x0
    stw r0, -21468(r13)
    b @L_801E1B18
@L_801E1A20:
    bl fn_801E449C
    lis r3, -32697
    addi r30, r3, -28640
    lwz r0, 0xc(r30)
    cmplwi r0, 0x1
    beq @L_801E1A50
    bl OSGetTick
    lwz r4, 0xc(r30)
    divwu r0, r3, r4
    mullw r0, r0, r4
    subf r5, r0, r3
    b @L_801E1A54
@L_801E1A50:
    li r5, 0x0
@L_801E1A54:
    li r3, 0x0
    li r4, 0x0
    bl fn_801E40F8
    cmpwi r3, 0x0
    bne @L_801E1AA8
    addi r3, r31, 0x5c
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r3, -21456(r13)
    bl fn_800E202C
    clrlwi. r0, r3, 16
    mr r30, r3
    beq @L_801E1A94
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
@L_801E1A94:
    bl fn_800EE928
    li r0, 0x1
    li r3, 0x0
    stw r0, -21468(r13)
    b @L_801E1B18
@L_801E1AA8:
    addi r3, r1, 0xc
    addi r4, r1, 0x8
    bl fn_801E3858
    lwz r3, 0xc(r1)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801E1ADC
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x7f
    li r8, 0x0
    bl fn_8014F2DC
@L_801E1ADC:
    lwz r3, 0x8(r1)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_801E1B04
    li r4, 0x0
    li r5, 0x7f
    li r6, 0x0
    li r7, 0x7f
    li r8, 0x0
    bl fn_8014F2DC
@L_801E1B04:
    bl fn_801E4058
    li r0, 0x1
    li r3, 0x0
    stb r0, -21471(r13)
    stw r0, -21468(r13)
@L_801E1B18:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
