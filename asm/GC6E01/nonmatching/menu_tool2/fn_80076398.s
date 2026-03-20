stwu r1, -0x150(r1)
mflr r0
stw r0, 0x154(r1)
stw r31, 0x14c(r1)
stw r30, 0x148(r1)
stw r29, 0x144(r1)
stw r28, 0x140(r1)
mr r31, r3
mr r28, r4
cmplwi r31, 0x0
lis r4, lbl_80268940@ha
addi r29, r4, lbl_80268940@l
li r30, 0x0
beq @800763E8
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @800763EC
@800763E8
li r30, 0x1
@800763EC
cmpwi r30, 0x0
beq @800763FC
li r3, 0x1
b @80076798
@800763FC
cmpwi r28, 0x3
beq @800766B0
bge @80076420
cmpwi r28, 0x1
beq @80076678
bge @80076694
cmpwi r28, 0x0
bge @80076430
b @80076784
@80076420
cmpwi r28, 0x5
beq @80076748
bge @80076784
b @800766EC
@80076430
mr r3, r31
li r4, 0x2
bl fn_80076398
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076450
li r3, 0x1
b @80076798
@80076450
mr r3, r31
bl fn_8011F5C8
clrlwi r0, r3, 16
cmpwi r0, 0x19a
beq @80076474
bge @80076490
cmpwi r0, 0x97
beq @80076474
b @80076490
@80076474
mr r3, r31
bl fn_8011E7A4
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076490
li r0, 0x0
b @80076494
@80076490
li r0, 0x1
@80076494
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @800764A8
li r3, 0x0
b @80076798
@800764A8
mr r4, r31
addi r3, r1, 0x8
bl fn_8011F5FC
addi r3, r1, 0x8
bl fn_8012546C
mr r3, r31
bl fn_8011F15C
clrlwi r30, r3, 16
addi r3, r1, 0x8
bl fn_8011F15C
clrlwi r0, r3, 16
cmplw r0, r30
blt @8007657C
mr r3, r31
bl fn_8011F130
clrlwi r30, r3, 16
addi r3, r1, 0x8
bl fn_8011F130
clrlwi r0, r3, 16
cmplw r0, r30
blt @8007657C
mr r3, r31
bl fn_8011F104
clrlwi r30, r3, 16
addi r3, r1, 0x8
bl fn_8011F104
clrlwi r0, r3, 16
cmplw r0, r30
blt @8007657C
mr r3, r31
bl fn_8011F0D8
clrlwi r30, r3, 16
addi r3, r1, 0x8
bl fn_8011F0D8
clrlwi r0, r3, 16
cmplw r0, r30
blt @8007657C
mr r3, r31
bl fn_8011F0AC
clrlwi r30, r3, 16
addi r3, r1, 0x8
bl fn_8011F0AC
clrlwi r0, r3, 16
cmplw r0, r30
blt @8007657C
mr r3, r31
bl fn_8011F080
clrlwi r30, r3, 16
addi r3, r1, 0x8
bl fn_8011F080
clrlwi r0, r3, 16
cmplw r0, r30
bge @80076584
@8007657C
li r3, 0x0
b @80076798
@80076584
mr r3, r31
bl fn_8011F054
clrlwi r29, r3, 16
mr r3, r31
bl fn_8011F028
clrlwi r0, r3, 16
mr r3, r31
add r29, r29, r0
bl fn_8011EFFC
clrlwi r0, r3, 16
mr r3, r31
add r29, r29, r0
bl fn_8011EFD0
clrlwi r0, r3, 16
mr r3, r31
add r29, r29, r0
bl fn_8011EFA4
clrlwi r0, r3, 16
mr r3, r31
add r29, r29, r0
bl fn_8011EF78
clrlwi r0, r3, 16
add r29, r29, r0
cmplwi r29, 0x1fe
ble @800765F0
li r3, 0x0
b @80076798
@800765F0
mr r3, r31
bl fn_8011F054
clrlwi r0, r3, 16
cmplwi r0, 0xff
bgt @80076668
mr r3, r31
bl fn_8011F028
clrlwi r0, r3, 16
cmplwi r0, 0xff
bgt @80076668
mr r3, r31
bl fn_8011EFFC
clrlwi r0, r3, 16
cmplwi r0, 0xff
bgt @80076668
mr r3, r31
bl fn_8011EFD0
clrlwi r0, r3, 16
cmplwi r0, 0xff
bgt @80076668
mr r3, r31
bl fn_8011EFA4
clrlwi r0, r3, 16
cmplwi r0, 0xff
bgt @80076668
mr r3, r31
bl fn_8011EF78
clrlwi r0, r3, 16
cmplwi r0, 0xff
ble @80076670
@80076668
li r3, 0x0
b @80076798
@80076670
li r3, 0x1
b @80076798
@80076678
mr r3, r31
bl fn_8011FC74
clrlwi r0, r3, 24
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r3, r0, 24
b @80076798
@80076694
mr r3, r31
bl fn_8011E8DC
clrlwi r0, r3, 24
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r3, r0, 24
b @80076798
@800766B0
mr r3, r31
bl fn_8011F1A0
clrlwi r0, r3, 16
cmpwi r0, 0xaf
beq @800766DC
bge @800766E4
cmpwi r0, 0x0
beq @800766D4
b @800766E4
@800766D4
li r3, 0x1
b @80076798
@800766DC
li r3, 0x0
b @80076798
@800766E4
bl fn_80142984
b @80076798
@800766EC
li r29, 0x0
@800766F0
mr r3, r31
clrlwi r6, r29, 16
li r4, 0x0
li r5, 0x7f
bl fn_8012640C
clrlwi r28, r3, 16
cmplwi r28, 0x0
beq @80076734
li r3, 0x0
bl fn_8011CA34
mr r30, r3
mr r3, r28
bl fn_8011CA34
cmplw r3, r30
bne @80076734
li r3, 0x0
b @80076798
@80076734
addi r29, r29, 0x1
cmpwi r29, 0x4
blt @800766F0
li r3, 0x1
b @80076798
@80076748
mr r3, r31
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
clrlwi r0, r3, 16
cmplwi r0, 0x0
bne @80076778
addi r3, r29, 0x108
addi r5, r29, 0x14c
li r4, 0x25e
bl fn_80196E10
@80076778
mr r3, r31
bl fn_80123FBC
b @80076798
@80076784
addi r3, r29, 0x108
addi r5, r29, 0x118
li r4, 0x274
bl fn_80196E10
li r3, 0x0
@80076798
lwz r0, 0x154(r1)
lwz r31, 0x14c(r1)
lwz r30, 0x148(r1)
lwz r29, 0x144(r1)
lwz r28, 0x140(r1)
mtlr r0
addi r1, r1, 0x150
blr
