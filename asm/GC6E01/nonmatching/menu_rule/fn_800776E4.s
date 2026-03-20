stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r23, 0xc(r1)
mr r25, r3
li r26, 0x0
b @8007775C
@80077700
mr r3, r25
mr r4, r26
bl fn_8012AC08
li r27, 0x0
mr r24, r3
@80077714
mr r3, r24
mr r4, r27
bl fn_80076398
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80077734
li r0, 0x0
b @80077744
@80077734
addi r27, r27, 0x1
cmpwi r27, 0x6
blt @80077714
li r0, 0x1
@80077744
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80077758
li r0, 0x0
b @8007776C
@80077758
addi r26, r26, 0x1
@8007775C
clrlwi r0, r26, 16
cmplwi r0, 0x6
blt @80077700
li r0, 0x1
@8007776C
clrlwi r3, r0, 24
neg r0, r3
or r0, r0, r3
srwi r0, r0, 31
cmpwi r0, 0x0
beq @80077A44
bl fn_8006B420
li r29, 0x0
mr r27, r3
mr r24, r29
@80077794
mr r3, r25
mr r4, r27
mr r5, r24
bl fn_80076F2C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800777B8
li r0, 0x0
b @800777C8
@800777B8
addi r24, r24, 0x1
cmpwi r24, 0x4
blt @80077794
li r0, 0x1
@800777C8
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @800777DC
li r0, 0x0
b @80077A34
@800777DC
li r28, 0x0
@800777E0
mr r3, r25
clrlwi r4, r28, 16
bl fn_8012AC08
mr r30, r3
cmplwi r30, 0x0
beq @80077A1C
bl fn_80123FBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
beq @80077A1C
cntlzw r0, r30
li r26, 0x0
srwi r31, r0, 5
@80077814
cmpwi r31, 0x0
li r24, 0x0
bne @8007783C
mr r3, r30
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80077840
@8007783C
li r24, 0x1
@80077840
cmpwi r24, 0x0
beq @80077850
li r0, 0x1
b @800779E0
@80077850
cmpwi r26, 0x1
beq @8007789C
bge @80077868
cmpwi r26, 0x0
bge @80077874
b @800779C4
@80077868
cmpwi r26, 0x3
bge @800779C4
b @800778C4
@80077874
mr r3, r30
bl fn_8011F4A8
clrlwi r5, r3, 24
lha r0, 0x0(r27)
srawi r4, r5, 31
srwi r3, r0, 31
subfc r0, r0, r5
adde r0, r4, r3
clrlwi r0, r0, 24
b @800779E0
@8007789C
mr r3, r30
bl fn_8011F4A8
lha r0, 0x2(r27)
clrlwi r5, r3, 24
srwi r3, r5, 31
srawi r4, r0, 31
subfc r0, r5, r0
adde r0, r4, r3
clrlwi r0, r0, 24
b @800779E0
@800778C4
mr r3, r30
bl fn_8011F1A0
mr r23, r3
bl fn_8006B420
clrlwi r0, r23, 16
mr r24, r3
cmpwi r0, 0xaf
beq @800778FC
bge @80077904
cmpwi r0, 0x0
beq @800778F4
b @80077904
@800778F4
li r3, 0x1
b @8007790C
@800778FC
li r3, 0x0
b @8007790C
@80077904
mr r3, r23
bl fn_80142984
@8007790C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80077920
li r0, 0x0
b @800779E0
@80077920
lwz r0, 0x8(r24)
cmpwi r0, 0x1
beq @80077950
bge @8007793C
cmpwi r0, 0x0
bge @80077948
b @800779BC
@8007793C
cmpwi r0, 0x3
bge @800779BC
b @80077964
@80077948
li r0, 0x1
b @800779E0
@80077950
clrlwi r0, r23, 16
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r0, r0, 24
b @800779E0
@80077964
lis r3, lbl_802EE458@ha
lwz r0, lbl_80478928@sda21(r0)
addi r5, r3, lbl_802EE458@l
li r4, 0x0
clrlwi r3, r23, 16
mtctr r0
cmplwi r0, 0x0
ble @800779B4
@80077984
lhz r0, 0x0(r5)
cmplw r3, r0
bne @800779A8
addi r0, r4, 0x18
lbzx r0, r24, r0
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r0, r0, 24
b @800779E0
@800779A8
addi r5, r5, 0x2
addi r4, r4, 0x1
bdnz @80077984
@800779B4
li r0, 0x1
b @800779E0
@800779BC
li r0, 0x0
b @800779E0
@800779C4
lis r3, lbl_80268A48@ha
lis r5, lbl_80268A58@ha
addi r3, r3, lbl_80268A48@l
li r4, 0xfb
addi r5, r5, lbl_80268A58@l
bl fn_80196E10
li r0, 0x0
@800779E0
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @800779F4
li r0, 0x0
b @80077A04
@800779F4
addi r26, r26, 0x1
cmpwi r26, 0x3
blt @80077814
li r0, 0x1
@80077A04
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @80077A18
li r0, 0x0
b @80077A34
@80077A18
addi r29, r29, 0x1
@80077A1C
addi r28, r28, 0x1
cmpwi r28, 0x6
blt @800777E0
neg r0, r29
andc r0, r0, r29
srwi r0, r0, 31
@80077A34
clrlwi r3, r0, 24
neg r0, r3
or r0, r0, r3
srwi r0, r0, 31
@80077A44
clrlwi r3, r0, 24
lmw r23, 0xc(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
