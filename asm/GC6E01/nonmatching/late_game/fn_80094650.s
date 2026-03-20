stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r25, 0x24(r1)
mr r27, r3
mr r31, r4
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lwz r29, 0xc(r3)
cmplwi r29, 0x0
beq @80095668
lha r0, 0x6(r31)
li r30, 0x1
cmpwi r0, 0x1b8
bge @800946B4
cmpwi r0, 0x18b
bge @800946A8
cmpwi r0, 0x182
bge @8009473C
cmpwi r0, 0x170
bge @800946D0
b @8009473C
@800946A8
cmpwi r0, 0x191
bge @8009473C
b @800946D0
@800946B4
cmpwi r0, 0x1d3
bge @800946C8
cmpwi r0, 0x1ca
bge @8009473C
b @800946D0
@800946C8
cmpwi r0, 0x1d9
bge @8009473C
@800946D0
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x1(r3)
cmpwi r0, 0x7
beq @800946FC
bge @8009472C
cmpwi r0, 0x5
bge @8009472C
cmpwi r0, 0x3
bge @800946FC
b @8009472C
@800946FC
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x2(r3)
extsb r0, r0
cmpwi r0, 0x0
blt @80094724
cmpwi r0, 0x4
bgt @80094724
li r30, 0x1
b @80094730
@80094724
li r30, 0x0
b @80094730
@8009472C
li r30, 0x0
@80094730
mr r3, r31
mr r4, r30
bl fn_80109220
@8009473C
clrlwi r0, r30, 24
cmplwi r0, 0x0
beq @80095668
lha r4, 0x6(r31)
li r0, -0x100
lbz r3, 0x8b(r27)
cmpwi r4, 0x1c1
or r30, r3, r0
bge @800947E0
cmpwi r4, 0x18d
beq @80095134
bge @800947A8
cmpwi r4, 0x181
beq @80095668
bge @80094790
cmpwi r4, 0x170
beq @80094F3C
blt @80095668
cmpwi r4, 0x179
bge @800952B4
b @80095490
@80094790
cmpwi r4, 0x18b
beq @80095010
bge @800950A8
cmpwi r4, 0x187
bge @80095668
b @800949F4
@800947A8
cmpwi r4, 0x196
bge @800947C8
cmpwi r4, 0x190
beq @80095668
bge @80094B58
cmpwi r4, 0x18f
bge @800951F4
b @80095668
@800947C8
cmpwi r4, 0x1b8
beq @80094F3C
bge @80095490
cmpwi r4, 0x19b
bge @80095668
b @80094CB8
@800947E0
cmpwi r4, 0x1d7
beq @800951F4
bge @80094820
cmpwi r4, 0x1d3
beq @80095010
bge @80094810
cmpwi r4, 0x1c9
beq @80095668
blt @800952B4
cmpwi r4, 0x1ce
bge @80095668
b @800949F4
@80094810
cmpwi r4, 0x1d5
beq @80095134
bge @80095668
b @800950A8
@80094820
cmpwi r4, 0x59b
bge @80094848
cmpwi r4, 0x1dd
bge @8009483C
cmpwi r4, 0x1d9
bge @80094B58
b @80095668
@8009483C
cmpwi r4, 0x1e1
bge @80095668
b @80094CB8
@80094848
cmpwi r4, 0x12b3
bge @8009485C
cmpwi r4, 0x59f
bge @80095668
b @80094864
@8009485C
cmpwi r4, 0x12b8
bge @80095668
@80094864
lwz r3, lbl_8047C200@sda21(r0)
cmpwi r4, 0x12b3
lwz r0, lbl_8047C204@sda21(r0)
stw r3, 0x18(r1)
stw r0, 0x14(r1)
beq @800948F4
bge @800948A8
cmpwi r4, 0x59d
beq @800948DC
bge @8009489C
cmpwi r4, 0x59b
beq @800948CC
bge @800948D4
b @80094910
@8009489C
cmpwi r4, 0x59f
bge @80094910
b @800948E4
@800948A8
cmpwi r4, 0x12b6
beq @8009490C
bge @800948C0
cmpwi r4, 0x12b5
bge @80094904
b @800948FC
@800948C0
cmpwi r4, 0x12b8
bge @80094910
b @800948EC
@800948CC
li r28, 0x0
b @80094910
@800948D4
li r28, 0x1
b @80094910
@800948DC
li r28, 0x2
b @80094910
@800948E4
li r28, 0x3
b @80094910
@800948EC
li r28, 0x0
b @80094910
@800948F4
li r28, 0x1
b @80094910
@800948FC
li r28, 0x2
b @80094910
@80094904
li r28, 0x3
b @80094910
@8009490C
li r28, 0x4
@80094910
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r0, 0x1(r3)
cmpwi r0, 0x4
beq @80094940
bge @80094934
cmpwi r0, 0x3
bge @800949B4
b @80095668
@80094934
cmpwi r0, 0x7
beq @800949B4
b @80095668
@80094940
lbz r3, 0x3(r3)
extsb r0, r28
extsb r3, r3
cmpw r3, r0
bne @80094974
lwz r0, 0x18(r1)
addi r7, r1, 0x10
li r3, 0x0
li r4, 0x0
stw r0, 0x10(r1)
lha r5, 0x54(r31)
lha r6, 0x56(r31)
bl fn_8001E58C
@80094974
lis r3, lbl_803FB380@ha
extsb r0, r28
addi r3, r3, lbl_803FB380@l
lbz r3, 0x2(r3)
extsb r3, r3
cmpw r3, r0
bne @80095668
lwz r0, 0x14(r1)
addi r7, r1, 0xc
li r3, 0x0
li r4, 0x0
stw r0, 0xc(r1)
lha r5, 0x54(r31)
lha r6, 0x56(r31)
bl fn_8001E58C
b @80095668
@800949B4
lis r3, lbl_803FB380@ha
extsb r0, r28
addi r3, r3, lbl_803FB380@l
lbz r3, 0x2(r3)
extsb r3, r3
cmpw r3, r0
bne @80095668
lwz r0, 0x14(r1)
addi r7, r1, 0x8
li r3, 0x0
li r4, 0x0
stw r0, 0x8(r1)
lha r5, 0x54(r31)
lha r6, 0x56(r31)
bl fn_8001E58C
b @80095668
@800949F4
cmpwi r4, 0x186
beq @80094A68
bge @80094A24
cmpwi r4, 0x183
beq @80094A80
bge @80094A18
cmpwi r4, 0x182
bge @80094A88
b @80094A8C
@80094A18
cmpwi r4, 0x185
bge @80094A70
b @80094A78
@80094A24
cmpwi r4, 0x1cc
beq @80094A50
bge @80094A40
cmpwi r4, 0x1ca
beq @80094A60
bge @80094A58
b @80094A8C
@80094A40
cmpwi r4, 0x1ce
bge @80094A8C
li r28, 0x0
b @80094A8C
@80094A50
li r28, 0x1
b @80094A8C
@80094A58
li r28, 0x2
b @80094A8C
@80094A60
li r28, 0x3
b @80094A8C
@80094A68
li r28, 0x0
b @80094A8C
@80094A70
li r28, 0x1
b @80094A8C
@80094A78
li r28, 0x2
b @80094A8C
@80094A80
li r28, 0x3
b @80094A8C
@80094A88
li r28, 0x4
@80094A8C
clrlwi r30, r28, 16
cmplwi r30, 0x4
bne @80094AA8
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lhz r28, 0x18(r3)
b @80094ADC
@80094AA8
mr r3, r29
mr r6, r30
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r28, r3, 16
mr r3, r29
mr r4, r30
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80094ADC
li r28, 0x0
@80094ADC
clrlwi r0, r28, 16
cmpwi r0, 0x164
beq @80094B04
bge @80094AF8
cmpwi r0, 0x0
beq @80094B04
b @80094B14
@80094AF8
cmpwi r0, 0x166
bge @80094B14
b @80094B0C
@80094B04
li r0, 0x0
b @80094B34
@80094B0C
li r0, 0x5d
b @80094B34
@80094B14
mr r4, r28
li r3, 0x0
li r5, 0x3
li r6, 0x0
bl fn_8011BEB4
clrlwi r3, r3, 16
bl fn_8010C46C
clrlwi r0, r3, 16
@80094B34
cmplwi r0, 0x0
beq @80095668
mr r5, r27
clrlwi r6, r0, 16
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
b @80095668
@80094B58
cmpwi r4, 0x195
beq @80094BCC
bge @80094B88
cmpwi r4, 0x192
beq @80094BE4
bge @80094B7C
cmpwi r4, 0x191
bge @80094BEC
b @80094BF0
@80094B7C
cmpwi r4, 0x194
bge @80094BD4
b @80094BDC
@80094B88
cmpwi r4, 0x1db
beq @80094BB4
bge @80094BA4
cmpwi r4, 0x1d9
beq @80094BC4
bge @80094BBC
b @80094BF0
@80094BA4
cmpwi r4, 0x1dd
bge @80094BF0
li r28, 0x0
b @80094BF0
@80094BB4
li r28, 0x1
b @80094BF0
@80094BBC
li r28, 0x2
b @80094BF0
@80094BC4
li r28, 0x3
b @80094BF0
@80094BCC
li r28, 0x0
b @80094BF0
@80094BD4
li r28, 0x1
b @80094BF0
@80094BDC
li r28, 0x2
b @80094BF0
@80094BE4
li r28, 0x3
b @80094BF0
@80094BEC
li r28, 0x4
@80094BF0
clrlwi r28, r28, 16
cmplwi r28, 0x4
bne @80094C0C
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lhz r27, 0x18(r3)
b @80094C40
@80094C0C
mr r3, r29
mr r6, r28
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r27, r3, 16
mr r3, r29
mr r4, r28
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80094C40
li r27, 0x0
@80094C40
clrlwi r0, r27, 16
cmplwi r0, 0x0
bne @80094C6C
lha r5, 0x54(r31)
mr r7, r30
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0x2be0
bl fn_800FBB34
b @80095668
@80094C6C
mr r4, r27
li r3, 0x0
li r5, 0x1
li r6, 0x0
bl fn_8011BEB4
cmplwi r3, 0x0
beq @80095668
bl fn_800FA280
mr r4, r3
li r3, 0x37
bl fn_80132A38
lha r5, 0x54(r31)
mr r7, r30
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0xe7
bl fn_800FBB34
b @80095668
@80094CB8
cmpwi r4, 0x19a
beq @80094D2C
bge @80094CE8
cmpwi r4, 0x197
beq @80094D44
bge @80094CDC
cmpwi r4, 0x196
bge @80094D4C
b @80094D50
@80094CDC
cmpwi r4, 0x199
bge @80094D34
b @80094D3C
@80094CE8
cmpwi r4, 0x1df
beq @80094D14
bge @80094D04
cmpwi r4, 0x1dd
beq @80094D24
bge @80094D1C
b @80094D50
@80094D04
cmpwi r4, 0x1e1
bge @80094D50
li r28, 0x0
b @80094D50
@80094D14
li r28, 0x1
b @80094D50
@80094D1C
li r28, 0x2
b @80094D50
@80094D24
li r28, 0x3
b @80094D50
@80094D2C
li r28, 0x0
b @80094D50
@80094D34
li r28, 0x1
b @80094D50
@80094D3C
li r28, 0x2
b @80094D50
@80094D44
li r28, 0x3
b @80094D50
@80094D4C
li r28, 0x4
@80094D50
clrlwi r26, r28, 16
cmplwi r26, 0x4
bne @80094D6C
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lhz r27, 0x18(r3)
b @80094DA0
@80094D6C
mr r3, r29
mr r6, r26
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r27, r3, 16
mr r3, r29
mr r4, r26
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80094DA0
li r27, 0x0
@80094DA0
li r3, 0x2bd4
bl fn_800FA444
srwi r3, r3, 16
lha r0, 0x54(r31)
extsh r3, r3
mr r5, r30
subf r3, r3, r0
li r4, 0x0
srwi r0, r3, 31
li r6, 0x2bd4
add r0, r0, r3
srawi r0, r0, 1
extsh r25, r0
mr r3, r25
bl fn_800FB680
clrlwi r0, r27, 16
cmpwi r0, 0x164
beq @80094E04
bge @80094DF8
cmpwi r0, 0x0
beq @80094E04
b @80094E7C
@80094DF8
cmpwi r0, 0x166
bge @80094E7C
b @80094E40
@80094E04
lha r6, 0x56(r31)
mr r5, r25
mr r7, r30
li r3, 0x0
li r4, 0x0
li r8, 0x2be1
bl fn_800FB8C8
lha r5, 0x54(r31)
mr r7, r30
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0x2be1
bl fn_800FB8C8
b @80095668
@80094E40
lha r6, 0x56(r31)
mr r5, r25
mr r7, r30
li r3, 0x0
li r4, 0x0
li r8, 0x2b6d
bl fn_800FB8C8
lha r5, 0x54(r31)
mr r7, r30
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0x2b6d
bl fn_800FB8C8
b @80095668
@80094E7C
clrlwi r0, r28, 16
cmplwi r0, 0x4
bne @80094EA0
mr r4, r27
li r3, 0x0
li r5, 0x2
li r6, 0x0
bl fn_8011BEB4
b @80094EB4
@80094EA0
mr r3, r29
mr r6, r26
li r4, 0x0
li r5, 0x80
bl fn_8012640C
@80094EB4
mr r4, r3
li r3, 0x34
bl fn_80132A38
lha r6, 0x56(r31)
mr r5, r25
mr r7, r30
li r3, 0x0
li r4, 0x0
li r8, 0xd2
bl fn_800FB8C8
clrlwi r0, r28, 16
cmplwi r0, 0x4
bne @80094F00
mr r4, r27
li r3, 0x0
li r5, 0x2
li r6, 0x0
bl fn_8011BEB4
b @80094F10
@80094F00
mr r3, r29
mr r4, r28
bl fn_80123E70
clrlwi r3, r3, 24
@80094F10
mr r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r31)
mr r7, r30
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0xd2
bl fn_800FB8C8
b @80095668
@80094F3C
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r30, 0x2(r3)
extsb r30, r30
clrlwi r0, r30, 16
cmplwi r0, 0x4
bne @80094F60
lhz r28, 0x18(r3)
b @80094F94
@80094F60
mr r3, r29
mr r6, r30
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r28, r3, 16
mr r3, r29
mr r4, r30
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80094F94
li r28, 0x0
@80094F94
clrlwi r0, r28, 16
cmpwi r0, 0x164
beq @80094FBC
bge @80094FB0
cmpwi r0, 0x0
beq @80094FBC
b @80094FCC
@80094FB0
cmpwi r0, 0x166
bge @80094FCC
b @80094FC4
@80094FBC
li r0, 0x0
b @80094FEC
@80094FC4
li r0, 0x5d
b @80094FEC
@80094FCC
mr r4, r28
li r3, 0x0
li r5, 0x24
li r6, 0x0
bl fn_8011BEB4
clrlwi r3, r3, 24
bl fn_801EE0A8
clrlwi r0, r3, 16
@80094FEC
cmplwi r0, 0x0
beq @80095668
mr r5, r27
clrlwi r6, r0, 16
li r3, 0x0
li r4, 0x0
li r7, 0x0
bl fn_801040F0
b @80095668
@80095010
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r28, 0x2(r3)
extsb r28, r28
clrlwi r0, r28, 16
cmplwi r0, 0x4
bne @80095034
lhz r27, 0x18(r3)
b @80095068
@80095034
mr r3, r29
mr r6, r28
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r27, r3, 16
mr r3, r29
mr r4, r28
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80095068
li r27, 0x0
@80095068
mr r4, r27
li r3, 0x0
li r5, 0x23
li r6, 0x0
bl fn_8011BEB4
clrlwi r3, r3, 16
bl fn_801EE07C
bl fn_801EE034
lha r5, 0x54(r31)
mr r8, r3
lha r6, 0x56(r31)
mr r7, r30
li r3, 0x0
li r4, 0x0
bl fn_800FBB34
b @80095668
@800950A8
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r28, 0x2(r3)
extsb r28, r28
clrlwi r0, r28, 16
cmplwi r0, 0x4
bne @800950CC
lhz r27, 0x18(r3)
b @80095100
@800950CC
mr r3, r29
mr r6, r28
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r27, r3, 16
mr r3, r29
mr r4, r28
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80095100
li r27, 0x0
@80095100
mr r4, r27
li r3, 0x0
li r5, 0x22
li r6, 0x0
bl fn_8011BEB4
lha r5, 0x54(r31)
mr r8, r3
lha r6, 0x56(r31)
mr r7, r30
li r3, 0x0
li r4, 0x0
bl fn_800FBB34
b @80095668
@80095134
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r28, 0x2(r3)
extsb r28, r28
clrlwi r0, r28, 16
cmplwi r0, 0x4
bne @80095158
lhz r27, 0x18(r3)
b @8009518C
@80095158
mr r3, r29
mr r6, r28
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r27, r3, 16
mr r3, r29
mr r4, r28
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8009518C
li r27, 0x0
@8009518C
mr r4, r27
li r3, 0x0
li r5, 0x6
li r6, 0x0
bl fn_8011BEB4
cmplwi r3, 0x1
bgt @800951C8
lha r5, 0x54(r31)
mr r7, r30
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0x2be2
bl fn_800FB8C8
b @80095668
@800951C8
mr r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r31)
mr r7, r30
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0xd2
bl fn_800FB8C8
b @80095668
@800951F4
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r28, 0x2(r3)
extsb r28, r28
clrlwi r0, r28, 16
cmplwi r0, 0x4
bne @80095218
lhz r27, 0x18(r3)
b @8009524C
@80095218
mr r3, r29
mr r6, r28
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r27, r3, 16
mr r3, r29
mr r4, r28
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8009524C
li r27, 0x0
@8009524C
mr r4, r27
li r3, 0x0
li r5, 0x7
li r6, 0x0
bl fn_8011BEB4
cmplwi r3, 0x1
bgt @80095288
lha r5, 0x54(r31)
mr r7, r30
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0x2be2
bl fn_800FB8C8
b @80095668
@80095288
mr r4, r3
li r3, 0x34
bl fn_80132A38
lha r5, 0x54(r31)
mr r7, r30
lha r6, 0x56(r31)
li r3, 0x0
li r4, 0x0
li r8, 0xd2
bl fn_800FB8C8
b @80095668
@800952B4
cmpwi r4, 0x1c1
beq @80095374
bge @80095308
cmpwi r4, 0x17d
beq @80095394
bge @800952F0
cmpwi r4, 0x17a
beq @800953AC
bge @800952E4
cmpwi r4, 0x179
bge @800953B4
b @800953B8
@800952E4
cmpwi r4, 0x17c
bge @8009539C
b @800953A4
@800952F0
cmpwi r4, 0x180
beq @8009537C
bge @800953B8
cmpwi r4, 0x17f
bge @80095384
b @8009538C
@80095308
cmpwi r4, 0x1c6
beq @8009534C
bge @8009532C
cmpwi r4, 0x1c4
beq @8009535C
bge @80095354
cmpwi r4, 0x1c3
bge @80095364
b @8009536C
@8009532C
cmpwi r4, 0x1c8
beq @8009533C
bge @800953B8
b @80095344
@8009533C
li r28, 0x1
b @800953B8
@80095344
li r28, 0x2
b @800953B8
@8009534C
li r28, 0x3
b @800953B8
@80095354
li r28, 0x4
b @800953B8
@8009535C
li r28, 0x5
b @800953B8
@80095364
li r28, 0x6
b @800953B8
@8009536C
li r28, 0x7
b @800953B8
@80095374
li r28, 0x8
b @800953B8
@8009537C
li r28, 0x1
b @800953B8
@80095384
li r28, 0x2
b @800953B8
@8009538C
li r28, 0x3
b @800953B8
@80095394
li r28, 0x4
b @800953B8
@8009539C
li r28, 0x5
b @800953B8
@800953A4
li r28, 0x6
b @800953B8
@800953AC
li r28, 0x7
b @800953B8
@800953B4
li r28, 0x8
@800953B8
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r31, 0x2(r3)
extsb r31, r31
clrlwi r0, r31, 16
cmplwi r0, 0x4
bne @800953DC
lhz r30, 0x18(r3)
b @80095410
@800953DC
mr r3, r29
mr r6, r31
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r30, r3, 16
mr r3, r29
mr r4, r31
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80095410
li r30, 0x0
@80095410
clrlwi r0, r30, 16
cmplwi r0, 0x0
beq @80095444
mr r4, r30
li r3, 0x0
li r5, 0x23
li r6, 0x0
bl fn_8011BEB4
clrlwi r3, r3, 16
bl fn_801EE07C
bl fn_801EE064
clrlwi r4, r3, 24
b @80095448
@80095444
li r4, 0x0
@80095448
lis r3, 0x6666
clrlwi r0, r28, 16
addi r3, r3, 0x6667
mr r5, r27
mulhw r6, r3, r4
li r3, 0x0
li r4, 0x0
srawi r6, r6, 2
srwi r7, r6, 31
add r6, r6, r7
cmpw r6, r0
blt @80095480
li r6, 0xf6
b @80095484
@80095480
li r6, 0xf5
@80095484
li r7, 0x0
bl fn_801040F0
b @80095668
@80095490
cmpwi r4, 0x1b9
beq @80095550
bge @800954E4
cmpwi r4, 0x175
beq @80095570
bge @800954CC
cmpwi r4, 0x172
beq @80095588
bge @800954C0
cmpwi r4, 0x171
bge @80095590
b @80095594
@800954C0
cmpwi r4, 0x174
bge @80095578
b @80095580
@800954CC
cmpwi r4, 0x178
beq @80095558
bge @80095594
cmpwi r4, 0x177
bge @80095560
b @80095568
@800954E4
cmpwi r4, 0x1be
beq @80095528
bge @80095508
cmpwi r4, 0x1bc
beq @80095538
bge @80095530
cmpwi r4, 0x1bb
bge @80095540
b @80095548
@80095508
cmpwi r4, 0x1c0
beq @80095518
bge @80095594
b @80095520
@80095518
li r28, 0x1
b @80095594
@80095520
li r28, 0x2
b @80095594
@80095528
li r28, 0x3
b @80095594
@80095530
li r28, 0x4
b @80095594
@80095538
li r28, 0x5
b @80095594
@80095540
li r28, 0x6
b @80095594
@80095548
li r28, 0x7
b @80095594
@80095550
li r28, 0x8
b @80095594
@80095558
li r28, 0x1
b @80095594
@80095560
li r28, 0x2
b @80095594
@80095568
li r28, 0x3
b @80095594
@80095570
li r28, 0x4
b @80095594
@80095578
li r28, 0x5
b @80095594
@80095580
li r28, 0x6
b @80095594
@80095588
li r28, 0x7
b @80095594
@80095590
li r28, 0x8
@80095594
lis r3, lbl_803FB380@ha
addi r3, r3, lbl_803FB380@l
lbz r31, 0x2(r3)
extsb r31, r31
clrlwi r0, r31, 16
cmplwi r0, 0x4
bne @800955B8
lhz r30, 0x18(r3)
b @800955EC
@800955B8
mr r3, r29
mr r6, r31
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r30, r3, 16
mr r3, r29
mr r4, r31
bl fn_80123CD4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800955EC
li r30, 0x0
@800955EC
clrlwi r0, r30, 16
cmplwi r0, 0x0
beq @80095620
mr r4, r30
li r3, 0x0
li r5, 0x23
li r6, 0x0
bl fn_8011BEB4
clrlwi r3, r3, 16
bl fn_801EE07C
bl fn_801EE04C
clrlwi r4, r3, 24
b @80095624
@80095620
li r4, 0x0
@80095624
lis r3, 0x6666
clrlwi r0, r28, 16
addi r3, r3, 0x6667
mr r5, r27
mulhw r6, r3, r4
li r3, 0x0
li r4, 0x0
srawi r6, r6, 2
srwi r7, r6, 31
add r6, r6, r7
cmpw r6, r0
blt @8009565C
li r6, 0xf7
b @80095660
@8009565C
li r6, 0xf5
@80095660
li r7, 0x0
bl fn_801040F0
@80095668
lmw r25, 0x24(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
