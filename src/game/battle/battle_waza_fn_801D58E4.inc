    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    lis r3, -32728
    stw r0, 0x44(r1)
    stw r31, 0x3c(r1)
    addi r31, r3, -27848
    bl fn_800057A8
    cmpwi r3, 0x2
    bne @L_801D5980
    lwz r11, 0x24(r31)
    addi r3, r1, 0x24
    lwz r10, 0x28(r31)
    lwz r9, 0x2c(r31)
    lwz r8, 0x30(r31)
    lwz r7, 0x34(r31)
    lwz r6, 0x38(r31)
    lwz r5, 0x3c(r31)
    lwz r4, 0x40(r31)
    lwz r0, 0x44(r31)
    stw r11, 0x24(r1)
    stw r10, 0x28(r1)
    stw r9, 0x2c(r1)
    stw r8, 0x18(r1)
    stw r7, 0x1c(r1)
    stw r6, 0x20(r1)
    stw r5, 0xc(r1)
    stw r4, 0x10(r1)
    stw r0, 0x14(r1)
    bl fn_801778DC
    addi r3, r1, 0x18
    bl fn_80177930
    addi r3, r1, 0xc
    bl fn_80177984
    li r3, 0x2
    bl fn_80177A44
    lis r3, -32698
    li r0, 0x0
    stw r0, 29688(r3)
    b @L_801D5994
@L_801D5980:
    li r3, 0x7
    bl fn_80177A44
    lis r3, -32698
    li r0, 0x1
    stw r0, 29688(r3)
@L_801D5994:
    lis r3, -32698
    li r0, 0x0
    addi r3, r3, 29688
    li r4, 0x0
    stw r0, 0x4(r3)
    addi r3, r3, 0x8
    li r5, 0x4
    bl memset
    lis r3, -32698
    li r5, 0x0
    addi r4, r3, 29688
    li r0, -0x1
    stw r5, 0x14(r4)
    addi r3, r1, 0x8
    stw r0, 0x18(r4)
    stb r5, 0xb(r1)
    stb r5, 0xa(r1)
    stb r5, 0x9(r1)
    stb r5, 0x8(r1)
    bl fn_80168570
    lis r4, -32698
    li r0, 0x0
    addi r6, r4, 29688
    li r4, 0x0
    stw r3, 0x10(r6)
    addi r3, r6, 0x61c
    li r5, 0x8
    stw r0, 0x65c(r6)
    stw r0, 0x664(r6)
    sth r0, 0x660(r6)
    sth r0, 0x668(r6)
    bl memset
    lis r3, -32698
    li r4, 0x0
    addi r3, r3, 29688
    li r0, 0x1
    stw r4, 0x624(r3)
    stw r4, 0x870(r3)
    stw r4, 0xc(r3)
    stw r4, 0x880(r3)
    stw r4, 0x878(r3)
    stw r0, 0x874(r3)
    bl fn_801E11E8
    lis r4, -32698
    addi r4, r4, 29688
    stb r3, 0x87d(r4)
    bl fn_801E1258
    bl fn_801C31EC
    lis r4, -32739
    li r3, 0x1
    addi r6, r4, 24636
    li r5, 0x0
    li r4, 0x7f
    bl fn_800FE834
    lis r4, -32698
    li r0, 0x0
    addi r4, r4, 29688
    stw r3, -21544(r13)
    stw r0, 0x63c(r4)
    lwz r31, 0x3c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
