    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r0, 0x0(r3)
    lwz r3, 0x8(r3)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800ECBA4
    lwz r3, 0x10(r3)
    @L_800ECBA4:
    stw r31, 0x8c(r30)
    lwz r0, 0x8c(r30)
    cmpwi r0, 0x1
    beq @L_800ECBEC
    bge @L_800ECC10
    cmpwi r0, 0x0
    bge @L_800ECBC4
    b @L_800ECC10
    @L_800ECBC4:
    lis r4, fn_800EE054@ha
    lis r5, 0x1
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    subi r5, r5, 0x64d1
    li r4, 0x6
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801C028C
    b @L_800ECC10
    @L_800ECBEC:
    lis r4, fn_800EE054@ha
    lis r5, 0x1
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    subi r5, r5, 0x64d1
    li r4, 0x6
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800ECC10:
    lwz r3, 0x0(r30)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_800ECC90
    rlwinm. r0, r3, 0, 14, 14
    lwz r3, 0x8(r30)
    beq @L_800ECC2C
    lwz r3, 0x10(r3)
    @L_800ECC2C:
    stw r31, 0xa4(r30)
    lwz r0, 0xa4(r30)
    cmpwi r0, 0x1
    beq @L_800ECC70
    bge @L_800ECC90
    cmpwi r0, 0x0
    bge @L_800ECC4C
    b @L_800ECC90
    @L_800ECC4C:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801C028C
    b @L_800ECC90
    @L_800ECC70:
    lis r4, fn_800EE054@ha
    li r5, 0x64db
    addi r6, r4, fn_800EE054@l
    li r7, 0x3
    li r4, 0x6
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801C028C
    @L_800ECC90:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
