    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r30, r3
    mr r31, r4
    lha r0, 0x6(r31)
    cmpwi r0, 0xb34
    beq @L_80064E1C
    bge @L_8006448C
    cmpwi r0, 0xb2a
    beq @L_800653D8
    bge @L_80064450
    cmpwi r0, 0xb21
    beq @L_8006526C
    bge @L_80064438
    cmpwi r0, 0xb1f
    beq @L_8006516C
    bge @L_800651D0
    cmpwi r0, 0xb1e
    bge @L_80065490
    b @L_80065608
@L_80064438:
    cmpwi r0, 0xb28
    beq @L_80065320
    bge @L_8006537C
    cmpwi r0, 0xb27
    bge @L_800652C4
    b @L_80065608
@L_80064450:
    cmpwi r0, 0xb2f
    beq @L_800649A4
    bge @L_80064474
    cmpwi r0, 0xb2d
    beq @L_80064794
    bge @L_8006489C
    cmpwi r0, 0xb2c
    bge @L_8006468C
    b @L_80065434
@L_80064474:
    cmpwi r0, 0xb32
    beq @L_80064C64
    bge @L_80064D40
    cmpwi r0, 0xb31
    bge @L_80064B88
    b @L_80064AAC
@L_8006448C:
    cmpwi r0, 0x1099
    bge @L_800644DC
    cmpwi r0, 0xb3a
    beq @L_80064644
    bge @L_800644C4
    cmpwi r0, 0xb37
    beq @L_80065098
    bge @L_800644B8
    cmpwi r0, 0xb36
    bge @L_80064FC4
    b @L_80064EF0
@L_800644B8:
    cmpwi r0, 0xb39
    bge @L_800645EC
    b @L_80064520
@L_800644C4:
    cmpwi r0, 0xe32
    beq @L_800654D4
    blt @L_80065608
    cmpwi r0, 0x1097
    bge @L_80065528
    b @L_80065608
@L_800644DC:
    cmpwi r0, 0x10a2
    bge @L_80064508
    cmpwi r0, 0x109e
    bge @L_800644FC
    cmpwi r0, 0x109b
    beq @L_80065540
    bge @L_8006554C
    b @L_80065534
@L_800644FC:
    cmpwi r0, 0x10a0
    bge @L_80065564
    b @L_80065558
@L_80064508:
    cmpwi r0, 0x10a5
    beq @L_80065588
    bge @L_80065608
    cmpwi r0, 0x10a4
    bge @L_8006557C
    b @L_80065570
@L_80064520:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    mr r31, r3
    bl fn_8011F4F0
    cmplwi r3, 0x0
    bne @L_8006454C
    li r3, 0x1
    bl fn_800FA280
@L_8006454C:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xe7
    bl fn_800FB680
    mr r3, r31
    bl fn_8001DA60
    clrlwi r0, r3, 24
    cmpwi r0, 0x1
    beq @L_800645A8
    bge @L_80064598
    cmpwi r0, 0x0
    bge @L_800645A0
    b @L_800645B0
@L_80064598:
    cmpwi r0, 0x3
    b @L_800645B0
@L_800645A0:
    li r3, 0xd67
    b @L_800645B4
@L_800645A8:
    li r3, 0xd68
    b @L_800645B4
@L_800645B0:
    li r3, 0x0
@L_800645B4:
    cmplwi r3, 0x0
    beq @L_80065608
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r3, 0x5a
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xcf
    bl fn_800FB680
    b @L_80065608
@L_800645EC:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    cmplwi r3, 0x0
    beq @L_80065608
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 24
    li r3, 0x34
    bl fn_80132A38
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xd3
    bl fn_800FB680
    b @L_80065608
@L_80064644:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    bl fn_8011F188
    mr r0, r3
    li r3, 0x34
    clrlwi r4, r0, 16
    bl fn_80132A38
    lbz r5, 0x8b(r30)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xd3
    bl fn_800FB680
    b @L_80065608
@L_8006468C:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x0
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x0
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800646D8
    li r28, 0x0
    b @L_80064700
@L_800646D8:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_800646F4
    cmpwi r28, 0x0
    beq @L_800646FC
    b @L_80064700
@L_800646F4:
    cmpw r28, r3
    bge @L_80064700
@L_800646FC:
    li r28, 0x0
@L_80064700:
    lis r3, 0x1
    clrlwi r4, r28, 16
    subi r0, r3, 0x2
    cmpw r4, r0
    beq @L_80064734
    bge @L_80064724
    cmpwi r4, 0x0
    beq @L_80064750
    b @L_8006473C
@L_80064724:
    cmpw r4, r3
    bge @L_8006473C
    li r4, 0x933
    b @L_80064750
@L_80064734:
    li r4, 0x934
    b @L_80064750
@L_8006473C:
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    mr r4, r3
@L_80064750:
    cmplwi r4, 0x0
    beq @L_80065608
    mr r3, r4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r4, 0x8b(r30)
    li r0, -0x100
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    or r7, r4, r0
    li r4, 0x0
    li r8, 0xe9
    bl fn_800FBB34
    b @L_80065608
@L_80064794:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x1
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x1
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800647E0
    li r28, 0x0
    b @L_80064808
@L_800647E0:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_800647FC
    cmpwi r28, 0x0
    beq @L_80064804
    b @L_80064808
@L_800647FC:
    cmpw r28, r3
    bge @L_80064808
@L_80064804:
    li r28, 0x0
@L_80064808:
    lis r3, 0x1
    clrlwi r4, r28, 16
    subi r0, r3, 0x2
    cmpw r4, r0
    beq @L_8006483C
    bge @L_8006482C
    cmpwi r4, 0x0
    beq @L_80064858
    b @L_80064844
@L_8006482C:
    cmpw r4, r3
    bge @L_80064844
    li r4, 0x933
    b @L_80064858
@L_8006483C:
    li r4, 0x934
    b @L_80064858
@L_80064844:
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    mr r4, r3
@L_80064858:
    cmplwi r4, 0x0
    beq @L_80065608
    mr r3, r4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r4, 0x8b(r30)
    li r0, -0x100
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    or r7, r4, r0
    li r4, 0x0
    li r8, 0xe9
    bl fn_800FBB34
    b @L_80065608
@L_8006489C:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x2
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x2
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800648E8
    li r28, 0x0
    b @L_80064910
@L_800648E8:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_80064904
    cmpwi r28, 0x0
    beq @L_8006490C
    b @L_80064910
@L_80064904:
    cmpw r28, r3
    bge @L_80064910
@L_8006490C:
    li r28, 0x0
@L_80064910:
    lis r3, 0x1
    clrlwi r4, r28, 16
    subi r0, r3, 0x2
    cmpw r4, r0
    beq @L_80064944
    bge @L_80064934
    cmpwi r4, 0x0
    beq @L_80064960
    b @L_8006494C
@L_80064934:
    cmpw r4, r3
    bge @L_8006494C
    li r4, 0x933
    b @L_80064960
@L_80064944:
    li r4, 0x934
    b @L_80064960
@L_8006494C:
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    mr r4, r3
@L_80064960:
    cmplwi r4, 0x0
    beq @L_80065608
    mr r3, r4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r4, 0x8b(r30)
    li r0, -0x100
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    or r7, r4, r0
    li r4, 0x0
    li r8, 0xe9
    bl fn_800FBB34
    b @L_80065608
@L_800649A4:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x3
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x3
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800649F0
    li r28, 0x0
    b @L_80064A18
@L_800649F0:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_80064A0C
    cmpwi r28, 0x0
    beq @L_80064A14
    b @L_80064A18
@L_80064A0C:
    cmpw r28, r3
    bge @L_80064A18
@L_80064A14:
    li r28, 0x0
@L_80064A18:
    lis r3, 0x1
    clrlwi r4, r28, 16
    subi r0, r3, 0x2
    cmpw r4, r0
    beq @L_80064A4C
    bge @L_80064A3C
    cmpwi r4, 0x0
    beq @L_80064A68
    b @L_80064A54
@L_80064A3C:
    cmpw r4, r3
    bge @L_80064A54
    li r4, 0x933
    b @L_80064A68
@L_80064A4C:
    li r4, 0x934
    b @L_80064A68
@L_80064A54:
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    mr r4, r3
@L_80064A68:
    cmplwi r4, 0x0
    beq @L_80065608
    mr r3, r4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r4, 0x8b(r30)
    li r0, -0x100
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    or r7, r4, r0
    li r4, 0x0
    li r8, 0xe9
    bl fn_800FBB34
    b @L_80065608
@L_80064AAC:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x0
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x0
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80064AF8
    li r28, 0x0
    b @L_80064B20
@L_80064AF8:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_80064B14
    cmpwi r28, 0x0
    beq @L_80064B1C
    b @L_80064B20
@L_80064B14:
    cmpw r28, r3
    bge @L_80064B20
@L_80064B1C:
    li r28, 0x0
@L_80064B20:
    lis r3, 0x1
    clrlwi r4, r28, 16
    subi r0, r3, 0x2
    cmpw r4, r0
    beq @L_80065608
    bge @L_80064B40
    cmpwi r4, 0x0
    beq @L_80065608
@L_80064B40:
    mr r3, r29
    li r4, 0x0
    li r5, 0x80
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lbz r4, 0x8b(r30)
    li r0, -0x100
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    or r7, r4, r0
    li r4, 0x0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_80065608
@L_80064B88:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x1
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x1
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80064BD4
    li r28, 0x0
    b @L_80064BFC
@L_80064BD4:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_80064BF0
    cmpwi r28, 0x0
    beq @L_80064BF8
    b @L_80064BFC
@L_80064BF0:
    cmpw r28, r3
    bge @L_80064BFC
@L_80064BF8:
    li r28, 0x0
@L_80064BFC:
    lis r3, 0x1
    clrlwi r4, r28, 16
    subi r0, r3, 0x2
    cmpw r4, r0
    beq @L_80065608
    bge @L_80064C1C
    cmpwi r4, 0x0
    beq @L_80065608
@L_80064C1C:
    mr r3, r29
    li r4, 0x0
    li r5, 0x80
    li r6, 0x1
    bl fn_8012640C
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lbz r4, 0x8b(r30)
    li r0, -0x100
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    or r7, r4, r0
    li r4, 0x0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_80065608
@L_80064C64:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x2
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x2
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80064CB0
    li r28, 0x0
    b @L_80064CD8
@L_80064CB0:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_80064CCC
    cmpwi r28, 0x0
    beq @L_80064CD4
    b @L_80064CD8
@L_80064CCC:
    cmpw r28, r3
    bge @L_80064CD8
@L_80064CD4:
    li r28, 0x0
@L_80064CD8:
    lis r3, 0x1
    clrlwi r4, r28, 16
    subi r0, r3, 0x2
    cmpw r4, r0
    beq @L_80065608
    bge @L_80064CF8
    cmpwi r4, 0x0
    beq @L_80065608
@L_80064CF8:
    mr r3, r29
    li r4, 0x0
    li r5, 0x80
    li r6, 0x2
    bl fn_8012640C
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lbz r4, 0x8b(r30)
    li r0, -0x100
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    or r7, r4, r0
    li r4, 0x0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_80065608
@L_80064D40:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x3
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x3
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80064D8C
    li r28, 0x0
    b @L_80064DB4
@L_80064D8C:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_80064DA8
    cmpwi r28, 0x0
    beq @L_80064DB0
    b @L_80064DB4
@L_80064DA8:
    cmpw r28, r3
    bge @L_80064DB4
@L_80064DB0:
    li r28, 0x0
@L_80064DB4:
    lis r3, 0x1
    clrlwi r4, r28, 16
    subi r0, r3, 0x2
    cmpw r4, r0
    beq @L_80065608
    bge @L_80064DD4
    cmpwi r4, 0x0
    beq @L_80065608
@L_80064DD4:
    mr r3, r29
    li r4, 0x0
    li r5, 0x80
    li r6, 0x3
    bl fn_8012640C
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    lbz r4, 0x8b(r30)
    li r0, -0x100
    lha r5, 0x54(r31)
    li r3, 0x0
    lha r6, 0x56(r31)
    or r7, r4, r0
    li r4, 0x0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_80065608
@L_80064E1C:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x0
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x0
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80064E68
    li r28, 0x0
    b @L_80064E90
@L_80064E68:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_80064E84
    cmpwi r28, 0x0
    beq @L_80064E8C
    b @L_80064E90
@L_80064E84:
    cmpw r28, r3
    bge @L_80064E90
@L_80064E8C:
    li r28, 0x0
@L_80064E90:
    clrlwi r0, r28, 16
    cmplwi r0, 0xffff
    bne @L_80064EA0
    li r0, 0xa5
@L_80064EA0:
    cmplwi r0, 0x0
    beq @L_80065608
    clrlwi r3, r0, 16
    bl fn_8011CA34
    bl fn_8011C9EC
    lis r4, 0x1
    clrlwi r5, r3, 24
    subi r0, r4, 0x2
    cmpw r5, r0
    beq @L_80065608
    lis r3, lbl_802EDB40@ha
    slwi r0, r5, 1
    addi r3, r3, lbl_802EDB40@l
    mr r5, r30
    lhzx r6, r3, r0
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80065608
@L_80064EF0:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x1
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x1
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80064F3C
    li r28, 0x0
    b @L_80064F64
@L_80064F3C:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_80064F58
    cmpwi r28, 0x0
    beq @L_80064F60
    b @L_80064F64
@L_80064F58:
    cmpw r28, r3
    bge @L_80064F64
@L_80064F60:
    li r28, 0x0
@L_80064F64:
    clrlwi r0, r28, 16
    cmplwi r0, 0xffff
    bne @L_80064F74
    li r0, 0xa5
@L_80064F74:
    cmplwi r0, 0x0
    beq @L_80065608
    clrlwi r3, r0, 16
    bl fn_8011CA34
    bl fn_8011C9EC
    lis r4, 0x1
    clrlwi r5, r3, 24
    subi r0, r4, 0x2
    cmpw r5, r0
    beq @L_80065608
    lis r3, lbl_802EDB40@ha
    slwi r0, r5, 1
    addi r3, r3, lbl_802EDB40@l
    mr r5, r30
    lhzx r6, r3, r0
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80065608
@L_80064FC4:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x2
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x2
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80065010
    li r28, 0x0
    b @L_80065038
@L_80065010:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_8006502C
    cmpwi r28, 0x0
    beq @L_80065034
    b @L_80065038
@L_8006502C:
    cmpw r28, r3
    bge @L_80065038
@L_80065034:
    li r28, 0x0
@L_80065038:
    clrlwi r0, r28, 16
    cmplwi r0, 0xffff
    bne @L_80065048
    li r0, 0xa5
@L_80065048:
    cmplwi r0, 0x0
    beq @L_80065608
    clrlwi r3, r0, 16
    bl fn_8011CA34
    bl fn_8011C9EC
    lis r4, 0x1
    clrlwi r5, r3, 24
    subi r0, r4, 0x2
    cmpw r5, r0
    beq @L_80065608
    lis r3, lbl_802EDB40@ha
    slwi r0, r5, 1
    addi r3, r3, lbl_802EDB40@l
    mr r5, r30
    lhzx r6, r3, r0
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80065608
@L_80065098:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x7f
    li r6, 0x3
    bl fn_8012640C
    clrlwi r28, r3, 16
    mr r3, r29
    li r4, 0x3
    bl fn_80123CD4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_800650E4
    li r28, 0x0
    b @L_8006510C
@L_800650E4:
    lis r3, 0x1
    subi r0, r3, 0x2
    cmpw r28, r0
    bge @L_80065100
    cmpwi r28, 0x0
    beq @L_80065108
    b @L_8006510C
@L_80065100:
    cmpw r28, r3
    bge @L_8006510C
@L_80065108:
    li r28, 0x0
@L_8006510C:
    clrlwi r0, r28, 16
    cmplwi r0, 0xffff
    bne @L_8006511C
    li r0, 0xa5
@L_8006511C:
    cmplwi r0, 0x0
    beq @L_80065608
    clrlwi r3, r0, 16
    bl fn_8011CA34
    bl fn_8011C9EC
    lis r4, 0x1
    clrlwi r5, r3, 24
    subi r0, r4, 0x2
    cmpw r5, r0
    beq @L_80065608
    lis r3, lbl_802EDB40@ha
    slwi r0, r5, 1
    addi r3, r3, lbl_802EDB40@l
    mr r5, r30
    lhzx r6, r3, r0
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80065608
@L_8006516C:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_8012640C
    clrlwi r0, r3, 16
    lis r3, lbl_802ED9FC@ha
    slwi r0, r0, 1
    mr r5, r30
    addi r4, r3, lbl_802ED9FC@l
    li r3, 0x0
    lhzx r6, r4, r0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80065608
@L_800651D0:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    li r4, 0x0
    mr r29, r3
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_8012640C
    clrlwi r31, r3, 16
    mr r3, r29
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x16
    li r6, 0x1
    bl fn_8012640C
    clrlwi r0, r3, 16
    cmplw r31, r0
    beq @L_80065608
    lis r3, lbl_802ED9FC@ha
    slwi r0, r0, 1
    addi r3, r3, lbl_802ED9FC@l
    mr r5, r30
    lhzx r6, r3, r0
    li r3, 0x0
    li r4, 0x0
    li r7, 0x0
    bl fn_801040F0
    b @L_80065608
@L_8006526C:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    lbz r4, 0x8b(r30)
    li r0, -0x100
    or r29, r4, r0
    bl fn_801230E0
    clrlwi r4, r3, 16
    cmplwi r4, 0x0
    beq @L_80065608
    li r3, 0x2d
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r29
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0x30da
    bl fn_800FBB34
    b @L_80065608
@L_800652C4:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    lbz r6, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    li r5, 0x88
    or r29, r6, r0
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r29
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_80065608
@L_80065320:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    lbz r6, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    li r5, 0x89
    or r29, r6, r0
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r29
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_80065608
@L_8006537C:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    lbz r6, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    li r5, 0x8a
    or r29, r6, r0
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r29
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_80065608
@L_800653D8:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    lbz r6, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    li r5, 0x8b
    or r29, r6, r0
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r29
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_80065608
@L_80065434:
    lis r4, lbl_803A9F08@ha
    li r3, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r4, 0xc(r4)
    bl fn_8025D970
    lbz r6, 0x8b(r30)
    li r0, -0x100
    li r4, 0x0
    li r5, 0x8c
    or r29, r6, r0
    li r6, 0x0
    bl fn_8012640C
    extsh r4, r3
    li r3, 0x34
    bl fn_80132A38
    lha r5, 0x54(r31)
    mr r7, r29
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xdf
    bl fn_800FBB34
    b @L_80065608
@L_80065490:
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    lwz r29, 0xc(r3)
    bl fn_8025DA88
    mulli r0, r29, 0xc
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    add r3, r3, r0
    addi r3, r3, 0x30
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80065608
    lhz r5, 0x2(r3)
    mr r3, r30
    mr r4, r31
    bl fn_8010B9E8
    b @L_80065608
@L_800654D4:
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    lwz r29, 0xc(r3)
    bl fn_8025DA88
    mr r4, r29
    li r3, 0x0
    bl fn_8025D970
    bl fn_801230E0
    clrlwi r0, r3, 16
    cmplwi r0, 0x0
    beq @L_80065514
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80065608
@L_80065514:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80065608
@L_80065528:
    li r5, 0x6
    bl fn_80060EF4
    b @L_80065608
@L_80065534:
    li r5, 0x6
    bl fn_80060EF4
    b @L_80065608
@L_80065540:
    li r5, -0x1
    bl fn_80060EF4
    b @L_80065608
@L_8006554C:
    li r5, 0x3
    bl fn_80060EF4
    b @L_80065608
@L_80065558:
    li r5, 0x4
    bl fn_80060EF4
    b @L_80065608
@L_80065564:
    li r5, 0x2
    bl fn_80060EF4
    b @L_80065608
@L_80065570:
    li r5, 0x1
    bl fn_80060EF4
    b @L_80065608
@L_8006557C:
    li r5, 0x0
    bl fn_80060EF4
    b @L_80065608
@L_80065588:
    lis r3, lbl_802EF0A8@ha
    addi r3, r3, lbl_802EF0A8@l
    addis r28, r3, 0x2
    bl fn_8025DAD0
    cmpwi r3, 0x0
    bne @L_800655AC
    li r3, 0x3db4
    bl fn_800FA280
    b @L_800655C4
@L_800655AC:
    bl fn_8006B1D4
    mr r4, r3
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x3c1e
    bl fn_800FA280
@L_800655C4:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lha r5, 0x50(r31)
    li r0, -0x100
    lha r3, -0x2df2(r28)
    li r8, 0xcf
    lbz r4, 0x8b(r30)
    subf r3, r5, r3
    lha r10, 0x52(r31)
    lha r9, -0x2df0(r28)
    subi r3, r3, 0x12
    lha r5, -0x2dee(r28)
    or r7, r4, r0
    lha r6, -0x2dec(r28)
    subf r4, r10, r9
    bl fn_800FBB34
@L_80065608:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
