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
beq @80064E1C
bge @8006448C
cmpwi r0, 0xb2a
beq @800653D8
bge @80064450
cmpwi r0, 0xb21
beq @8006526C
bge @80064438
cmpwi r0, 0xb1f
beq @8006516C
bge @800651D0
cmpwi r0, 0xb1e
bge @80065490
b @80065608
@80064438
cmpwi r0, 0xb28
beq @80065320
bge @8006537C
cmpwi r0, 0xb27
bge @800652C4
b @80065608
@80064450
cmpwi r0, 0xb2f
beq @800649A4
bge @80064474
cmpwi r0, 0xb2d
beq @80064794
bge @8006489C
cmpwi r0, 0xb2c
bge @8006468C
b @80065434
@80064474
cmpwi r0, 0xb32
beq @80064C64
bge @80064D40
cmpwi r0, 0xb31
bge @80064B88
b @80064AAC
@8006448C
cmpwi r0, 0x1099
bge @800644DC
cmpwi r0, 0xb3a
beq @80064644
bge @800644C4
cmpwi r0, 0xb37
beq @80065098
bge @800644B8
cmpwi r0, 0xb36
bge @80064FC4
b @80064EF0
@800644B8
cmpwi r0, 0xb39
bge @800645EC
b @80064520
@800644C4
cmpwi r0, 0xe32
beq @800654D4
blt @80065608
cmpwi r0, 0x1097
bge @80065528
b @80065608
@800644DC
cmpwi r0, 0x10a2
bge @80064508
cmpwi r0, 0x109e
bge @800644FC
cmpwi r0, 0x109b
beq @80065540
bge @8006554C
b @80065534
@800644FC
cmpwi r0, 0x10a0
bge @80065564
b @80065558
@80064508
cmpwi r0, 0x10a5
beq @80065588
bge @80065608
cmpwi r0, 0x10a4
bge @8006557C
b @80065570
@80064520
lis r4, lbl_803A9F08@ha
li r3, 0x0
addi r4, r4, lbl_803A9F08@l
lwz r4, 0xc(r4)
bl fn_8025D970
mr r31, r3
bl fn_8011F4F0
cmplwi r3, 0x0
bne @8006454C
li r3, 0x1
bl fn_800FA280
@8006454C
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
beq @800645A8
bge @80064598
cmpwi r0, 0x0
bge @800645A0
b @800645B0
@80064598
cmpwi r0, 0x3
b @800645B0
@800645A0
li r3, 0xd67
b @800645B4
@800645A8
li r3, 0xd68
b @800645B4
@800645B0
li r3, 0x0
@800645B4
cmplwi r3, 0x0
beq @80065608
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
b @80065608
@800645EC
lis r4, lbl_803A9F08@ha
li r3, 0x0
addi r4, r4, lbl_803A9F08@l
lwz r4, 0xc(r4)
bl fn_8025D970
cmplwi r3, 0x0
beq @80065608
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
b @80065608
@80064644
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
b @80065608
@8006468C
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
bne @800646D8
li r28, 0x0
b @80064700
@800646D8
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @800646F4
cmpwi r28, 0x0
beq @800646FC
b @80064700
@800646F4
cmpw r28, r3
bge @80064700
@800646FC
li r28, 0x0
@80064700
lis r3, 0x1
clrlwi r4, r28, 16
subi r0, r3, 0x2
cmpw r4, r0
beq @80064734
bge @80064724
cmpwi r4, 0x0
beq @80064750
b @8006473C
@80064724
cmpw r4, r3
bge @8006473C
li r4, 0x933
b @80064750
@80064734
li r4, 0x934
b @80064750
@8006473C
li r3, 0x0
li r5, 0x1
li r6, 0x0
bl fn_8011BEB4
mr r4, r3
@80064750
cmplwi r4, 0x0
beq @80065608
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
b @80065608
@80064794
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
bne @800647E0
li r28, 0x0
b @80064808
@800647E0
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @800647FC
cmpwi r28, 0x0
beq @80064804
b @80064808
@800647FC
cmpw r28, r3
bge @80064808
@80064804
li r28, 0x0
@80064808
lis r3, 0x1
clrlwi r4, r28, 16
subi r0, r3, 0x2
cmpw r4, r0
beq @8006483C
bge @8006482C
cmpwi r4, 0x0
beq @80064858
b @80064844
@8006482C
cmpw r4, r3
bge @80064844
li r4, 0x933
b @80064858
@8006483C
li r4, 0x934
b @80064858
@80064844
li r3, 0x0
li r5, 0x1
li r6, 0x0
bl fn_8011BEB4
mr r4, r3
@80064858
cmplwi r4, 0x0
beq @80065608
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
b @80065608
@8006489C
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
bne @800648E8
li r28, 0x0
b @80064910
@800648E8
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @80064904
cmpwi r28, 0x0
beq @8006490C
b @80064910
@80064904
cmpw r28, r3
bge @80064910
@8006490C
li r28, 0x0
@80064910
lis r3, 0x1
clrlwi r4, r28, 16
subi r0, r3, 0x2
cmpw r4, r0
beq @80064944
bge @80064934
cmpwi r4, 0x0
beq @80064960
b @8006494C
@80064934
cmpw r4, r3
bge @8006494C
li r4, 0x933
b @80064960
@80064944
li r4, 0x934
b @80064960
@8006494C
li r3, 0x0
li r5, 0x1
li r6, 0x0
bl fn_8011BEB4
mr r4, r3
@80064960
cmplwi r4, 0x0
beq @80065608
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
b @80065608
@800649A4
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
bne @800649F0
li r28, 0x0
b @80064A18
@800649F0
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @80064A0C
cmpwi r28, 0x0
beq @80064A14
b @80064A18
@80064A0C
cmpw r28, r3
bge @80064A18
@80064A14
li r28, 0x0
@80064A18
lis r3, 0x1
clrlwi r4, r28, 16
subi r0, r3, 0x2
cmpw r4, r0
beq @80064A4C
bge @80064A3C
cmpwi r4, 0x0
beq @80064A68
b @80064A54
@80064A3C
cmpw r4, r3
bge @80064A54
li r4, 0x933
b @80064A68
@80064A4C
li r4, 0x934
b @80064A68
@80064A54
li r3, 0x0
li r5, 0x1
li r6, 0x0
bl fn_8011BEB4
mr r4, r3
@80064A68
cmplwi r4, 0x0
beq @80065608
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
b @80065608
@80064AAC
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
bne @80064AF8
li r28, 0x0
b @80064B20
@80064AF8
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @80064B14
cmpwi r28, 0x0
beq @80064B1C
b @80064B20
@80064B14
cmpw r28, r3
bge @80064B20
@80064B1C
li r28, 0x0
@80064B20
lis r3, 0x1
clrlwi r4, r28, 16
subi r0, r3, 0x2
cmpw r4, r0
beq @80065608
bge @80064B40
cmpwi r4, 0x0
beq @80065608
@80064B40
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
b @80065608
@80064B88
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
bne @80064BD4
li r28, 0x0
b @80064BFC
@80064BD4
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @80064BF0
cmpwi r28, 0x0
beq @80064BF8
b @80064BFC
@80064BF0
cmpw r28, r3
bge @80064BFC
@80064BF8
li r28, 0x0
@80064BFC
lis r3, 0x1
clrlwi r4, r28, 16
subi r0, r3, 0x2
cmpw r4, r0
beq @80065608
bge @80064C1C
cmpwi r4, 0x0
beq @80065608
@80064C1C
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
b @80065608
@80064C64
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
bne @80064CB0
li r28, 0x0
b @80064CD8
@80064CB0
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @80064CCC
cmpwi r28, 0x0
beq @80064CD4
b @80064CD8
@80064CCC
cmpw r28, r3
bge @80064CD8
@80064CD4
li r28, 0x0
@80064CD8
lis r3, 0x1
clrlwi r4, r28, 16
subi r0, r3, 0x2
cmpw r4, r0
beq @80065608
bge @80064CF8
cmpwi r4, 0x0
beq @80065608
@80064CF8
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
b @80065608
@80064D40
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
bne @80064D8C
li r28, 0x0
b @80064DB4
@80064D8C
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @80064DA8
cmpwi r28, 0x0
beq @80064DB0
b @80064DB4
@80064DA8
cmpw r28, r3
bge @80064DB4
@80064DB0
li r28, 0x0
@80064DB4
lis r3, 0x1
clrlwi r4, r28, 16
subi r0, r3, 0x2
cmpw r4, r0
beq @80065608
bge @80064DD4
cmpwi r4, 0x0
beq @80065608
@80064DD4
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
b @80065608
@80064E1C
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
bne @80064E68
li r28, 0x0
b @80064E90
@80064E68
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @80064E84
cmpwi r28, 0x0
beq @80064E8C
b @80064E90
@80064E84
cmpw r28, r3
bge @80064E90
@80064E8C
li r28, 0x0
@80064E90
clrlwi r0, r28, 16
cmplwi r0, 0xffff
bne @80064EA0
li r0, 0xa5
@80064EA0
cmplwi r0, 0x0
beq @80065608
clrlwi r3, r0, 16
bl fn_8011CA34
bl fn_8011C9EC
lis r4, 0x1
clrlwi r5, r3, 24
subi r0, r4, 0x2
cmpw r5, r0
beq @80065608
lis r3, lbl_802EDB40@ha
slwi r0, r5, 1
addi r3, r3, lbl_802EDB40@l
mr r5, r30
lhzx r6, r3, r0
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
b @80065608
@80064EF0
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
bne @80064F3C
li r28, 0x0
b @80064F64
@80064F3C
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @80064F58
cmpwi r28, 0x0
beq @80064F60
b @80064F64
@80064F58
cmpw r28, r3
bge @80064F64
@80064F60
li r28, 0x0
@80064F64
clrlwi r0, r28, 16
cmplwi r0, 0xffff
bne @80064F74
li r0, 0xa5
@80064F74
cmplwi r0, 0x0
beq @80065608
clrlwi r3, r0, 16
bl fn_8011CA34
bl fn_8011C9EC
lis r4, 0x1
clrlwi r5, r3, 24
subi r0, r4, 0x2
cmpw r5, r0
beq @80065608
lis r3, lbl_802EDB40@ha
slwi r0, r5, 1
addi r3, r3, lbl_802EDB40@l
mr r5, r30
lhzx r6, r3, r0
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
b @80065608
@80064FC4
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
bne @80065010
li r28, 0x0
b @80065038
@80065010
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @8006502C
cmpwi r28, 0x0
beq @80065034
b @80065038
@8006502C
cmpw r28, r3
bge @80065038
@80065034
li r28, 0x0
@80065038
clrlwi r0, r28, 16
cmplwi r0, 0xffff
bne @80065048
li r0, 0xa5
@80065048
cmplwi r0, 0x0
beq @80065608
clrlwi r3, r0, 16
bl fn_8011CA34
bl fn_8011C9EC
lis r4, 0x1
clrlwi r5, r3, 24
subi r0, r4, 0x2
cmpw r5, r0
beq @80065608
lis r3, lbl_802EDB40@ha
slwi r0, r5, 1
addi r3, r3, lbl_802EDB40@l
mr r5, r30
lhzx r6, r3, r0
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
b @80065608
@80065098
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
bne @800650E4
li r28, 0x0
b @8006510C
@800650E4
lis r3, 0x1
subi r0, r3, 0x2
cmpw r28, r0
bge @80065100
cmpwi r28, 0x0
beq @80065108
b @8006510C
@80065100
cmpw r28, r3
bge @8006510C
@80065108
li r28, 0x0
@8006510C
clrlwi r0, r28, 16
cmplwi r0, 0xffff
bne @8006511C
li r0, 0xa5
@8006511C
cmplwi r0, 0x0
beq @80065608
clrlwi r3, r0, 16
bl fn_8011CA34
bl fn_8011C9EC
lis r4, 0x1
clrlwi r5, r3, 24
subi r0, r4, 0x2
cmpw r5, r0
beq @80065608
lis r3, lbl_802EDB40@ha
slwi r0, r5, 1
addi r3, r3, lbl_802EDB40@l
mr r5, r30
lhzx r6, r3, r0
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
b @80065608
@8006516C
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
b @80065608
@800651D0
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
beq @80065608
lis r3, lbl_802ED9FC@ha
slwi r0, r0, 1
addi r3, r3, lbl_802ED9FC@l
mr r5, r30
lhzx r6, r3, r0
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
b @80065608
@8006526C
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
beq @80065608
li r3, 0x2d
bl fn_80132A38
lha r5, 0x54(r31)
mr r7, r29
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0x30da
bl fn_800FBB34
b @80065608
@800652C4
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
b @80065608
@80065320
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
b @80065608
@8006537C
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
b @80065608
@800653D8
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
b @80065608
@80065434
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
b @80065608
@80065490
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
beq @80065608
lhz r5, 0x2(r3)
mr r3, r30
mr r4, r31
bl fn_8010B9E8
b @80065608
@800654D4
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
beq @80065514
lbz r0, 0x4(r31)
ori r0, r0, 0x2
extsb r0, r0
stb r0, 0x4(r31)
b @80065608
@80065514
lbz r0, 0x4(r31)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r31)
b @80065608
@80065528
li r5, 0x6
bl fn_80060EF4
b @80065608
@80065534
li r5, 0x6
bl fn_80060EF4
b @80065608
@80065540
li r5, -0x1
bl fn_80060EF4
b @80065608
@8006554C
li r5, 0x3
bl fn_80060EF4
b @80065608
@80065558
li r5, 0x4
bl fn_80060EF4
b @80065608
@80065564
li r5, 0x2
bl fn_80060EF4
b @80065608
@80065570
li r5, 0x1
bl fn_80060EF4
b @80065608
@8006557C
li r5, 0x0
bl fn_80060EF4
b @80065608
@80065588
lis r3, lbl_802EF0A8@ha
addi r3, r3, lbl_802EF0A8@l
addis r28, r3, 0x2
bl fn_8025DAD0
cmpwi r3, 0x0
bne @800655AC
li r3, 0x3db4
bl fn_800FA280
b @800655C4
@800655AC
bl fn_8006B1D4
mr r4, r3
li r3, 0x2f
bl fn_80132A38
li r3, 0x3c1e
bl fn_800FA280
@800655C4
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
@80065608
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
