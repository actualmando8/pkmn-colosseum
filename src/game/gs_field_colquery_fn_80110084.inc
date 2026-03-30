    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_8010CBD0
    mr. r6, r3
    bne @L_801100B4
    li r3, 0x1
    b @L_8011019C
@L_801100B4:
    li r0, 0x6
    li r7, 0x0
    mtctr r0
@L_801100C0:
    lhz r0, 0xa10(r3)
    clrlwi. r0, r0, 31
    beq @L_80110148
    lhz r0, 0xa24(r3)
    addi r7, r7, 0x1
    clrlwi. r0, r0, 31
    beq @L_80110148
    lhz r0, 0xa38(r3)
    addi r7, r7, 0x1
    clrlwi. r0, r0, 31
    beq @L_80110148
    lhz r0, 0xa4c(r3)
    addi r7, r7, 0x1
    clrlwi. r0, r0, 31
    beq @L_80110148
    lhz r0, 0xa60(r3)
    addi r7, r7, 0x1
    clrlwi. r0, r0, 31
    beq @L_80110148
    lhz r0, 0xa74(r3)
    addi r7, r7, 0x1
    clrlwi. r0, r0, 31
    beq @L_80110148
    lhz r0, 0xa88(r3)
    addi r7, r7, 0x1
    clrlwi. r0, r0, 31
    beq @L_80110148
    lhz r0, 0xa9c(r3)
    addi r7, r7, 0x1
    clrlwi. r0, r0, 31
    beq @L_80110148
    addi r3, r3, 0xa0
    addi r7, r7, 0x1
    bdnz @L_801100C0
@L_80110148:
    cmpwi r7, 0x30
    blt @L_80110158
    li r3, 0x5
    b @L_8011019C
@L_80110158:
    mulli r5, r7, 0x14
    lwz r4, 0x0(r31)
    li r0, 0x0
    li r3, 0x0
    add r5, r6, r5
    stw r4, 0xa00(r5)
    lwz r4, 0x4(r31)
    stw r4, 0xa04(r5)
    lfs f0, 0x8(r31)
    stfs f0, 0xa08(r5)
    lfs f0, 0xc(r31)
    stfs f0, 0xa0c(r5)
    sth r0, 0xa10(r5)
    lhz r0, 0xa10(r5)
    ori r0, r0, 0x1
    sth r0, 0xa10(r5)
    stw r7, 0x0(r30)
@L_8011019C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
