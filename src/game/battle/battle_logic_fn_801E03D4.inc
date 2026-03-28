    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x1
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    li r29, 0x0
@L_801E03F4:
    cmplwi r29, 0xc
    bgt @L_801E0738
    lis r3, -32713
    slwi r0, r29, 2
    addi r3, r3, 20920
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r3, 0x3b21
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801E043C
    li r29, 0x2
    b @L_801E0738
@L_801E043C:
    li r29, 0x1
    b @L_801E0738
    li r3, 0x3b22
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r29, 0xc
    b @L_801E0738
    li r3, 0x3b23
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r3, 0x6
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BDF4
    mr. r30, r3
    blt @L_801E0494
    li r29, 0x3
    b @L_801E0738
@L_801E0494:
    li r29, 0x1
    b @L_801E0738
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r30, 16
    mr r29, r3
    bl fn_8012AC08
    mr r4, r3
    mr r3, r29
    bl fn_80129D64
    clrlwi. r0, r3, 24
    beq @L_801E04D0
    li r29, 0x4
    b @L_801E0738
@L_801E04D0:
    li r29, 0x5
    b @L_801E0738
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r30, 16
    bl fn_8012AC08
    mr r29, r3
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    bne @L_801E0504
    li r0, 0x0
    b @L_801E0534
@L_801E0504:
    mr r3, r29
    bl fn_8011EE40
    clrlwi. r0, r3, 16
    beq @L_801E0530
    bl fn_801EEC74
    clrlwi. r0, r3, 24
    beq @L_801E0528
    li r0, 0x1
    b @L_801E0534
@L_801E0528:
    li r0, 0x0
    b @L_801E0534
@L_801E0530:
    li r0, 0x1
@L_801E0534:
    clrlwi. r0, r0, 24
    beq @L_801E0544
    li r29, 0x6
    b @L_801E0738
@L_801E0544:
    li r29, 0xb
    b @L_801E0738
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r30, 16
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x3b24
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r29, 0xc
    b @L_801E0738
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r30, 16
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x3b25
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801E05D8
    li r29, 0x7
    b @L_801E0738
@L_801E05D8:
    li r29, 0x1
    b @L_801E0738
    li r3, 0x3b26
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    mr r4, r30
    li r3, 0x2
    bl fn_80029760
    cmpwi r3, 0x0
    bne @L_801E0610
    li r29, 0x9
    b @L_801E0738
@L_801E0610:
    li r29, 0x8
    b @L_801E0738
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r30, 16
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x3b27
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r29, 0xc
    b @L_801E0738
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r30, 16
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x3b1f
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    bl fn_8001E184
    extsb. r0, r3
    bne @L_801E06A4
    li r29, 0xa
    b @L_801E0738
@L_801E06A4:
    li r29, 0x7
    b @L_801E0738
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r30, 16
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x3b47
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r29, 0xc
    b @L_801E0738
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    clrlwi r4, r30, 16
    bl fn_8012AC08
    bl fn_8011F4F0
    mr r4, r3
    li r3, 0x32
    bl fn_80132A38
    li r3, 0x3b20
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    bl fn_80106698
    li r29, 0xc
    b @L_801E0738
    li r3, 0x1
    bl fn_801069FC
    li r31, 0x0
@L_801E0738:
    cmpwi r31, 0x0
    bne @L_801E03F4
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
