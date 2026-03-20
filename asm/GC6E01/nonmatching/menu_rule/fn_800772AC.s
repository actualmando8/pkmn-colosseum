stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stmw r26, 0x8(r1)
mr r28, r3
mr r29, r4
cntlzw r0, r28
li r30, 0x0
srwi r31, r0, 5
@800772D0
cmpwi r31, 0x0
li r27, 0x0
bne @800772F8
mr r3, r28
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @800772FC
@800772F8
li r27, 0x1
@800772FC
cmpwi r27, 0x0
beq @8007730C
li r0, 0x1
b @8007749C
@8007730C
cmpwi r30, 0x1
beq @80077358
bge @80077324
cmpwi r30, 0x0
bge @80077330
b @80077480
@80077324
cmpwi r30, 0x3
bge @80077480
b @80077380
@80077330
mr r3, r28
bl fn_8011F4A8
clrlwi r5, r3, 24
lha r0, 0x0(r29)
srawi r4, r5, 31
srwi r3, r0, 31
subfc r0, r0, r5
adde r0, r4, r3
clrlwi r0, r0, 24
b @8007749C
@80077358
mr r3, r28
bl fn_8011F4A8
lha r0, 0x2(r29)
clrlwi r5, r3, 24
srwi r3, r5, 31
srawi r4, r0, 31
subfc r0, r5, r0
adde r0, r4, r3
clrlwi r0, r0, 24
b @8007749C
@80077380
mr r3, r28
bl fn_8011F1A0
mr r26, r3
bl fn_8006B420
clrlwi r0, r26, 16
mr r27, r3
cmpwi r0, 0xaf
beq @800773B8
bge @800773C0
cmpwi r0, 0x0
beq @800773B0
b @800773C0
@800773B0
li r3, 0x1
b @800773C8
@800773B8
li r3, 0x0
b @800773C8
@800773C0
mr r3, r26
bl fn_80142984
@800773C8
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800773DC
li r0, 0x0
b @8007749C
@800773DC
lwz r0, 0x8(r27)
cmpwi r0, 0x1
beq @8007740C
bge @800773F8
cmpwi r0, 0x0
bge @80077404
b @80077478
@800773F8
cmpwi r0, 0x3
bge @80077478
b @80077420
@80077404
li r0, 0x1
b @8007749C
@8007740C
clrlwi r0, r26, 16
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r0, r0, 24
b @8007749C
@80077420
lis r3, lbl_802EE458@ha
lwz r0, lbl_80478928@sda21(r0)
addi r5, r3, lbl_802EE458@l
li r4, 0x0
clrlwi r3, r26, 16
mtctr r0
cmplwi r0, 0x0
ble @80077470
@80077440
lhz r0, 0x0(r5)
cmplw r3, r0
bne @80077464
addi r0, r4, 0x18
lbzx r0, r27, r0
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r0, r0, 24
b @8007749C
@80077464
addi r5, r5, 0x2
addi r4, r4, 0x1
bdnz @80077440
@80077470
li r0, 0x1
b @8007749C
@80077478
li r0, 0x0
b @8007749C
@80077480
lis r3, lbl_80268A48@ha
lis r5, lbl_80268A58@ha
addi r3, r3, lbl_80268A48@l
li r4, 0xfb
addi r5, r5, lbl_80268A58@l
bl fn_80196E10
li r0, 0x0
@8007749C
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @800774B0
li r3, 0x0
b @800774C0
@800774B0
addi r30, r30, 0x1
cmpwi r30, 0x3
blt @800772D0
li r3, 0x1
@800774C0
lmw r26, 0x8(r1)
lwz r0, 0x24(r1)
mtlr r0
addi r1, r1, 0x20
blr
