stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
mr r28, r3
mr r29, r4
mr r30, r5
cmplwi r28, 0x0
li r31, 0x0
beq @80077520
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80077524
@80077520
li r31, 0x1
@80077524
cmpwi r31, 0x0
beq @80077534
li r3, 0x1
b @800776C4
@80077534
cmpwi r30, 0x1
beq @80077580
bge @8007754C
cmpwi r30, 0x0
bge @80077558
b @800776A8
@8007754C
cmpwi r30, 0x3
bge @800776A8
b @800775A8
@80077558
mr r3, r28
bl fn_8011F4A8
clrlwi r5, r3, 24
lha r0, 0x0(r29)
srawi r4, r5, 31
srwi r3, r0, 31
subfc r0, r0, r5
adde r0, r4, r3
clrlwi r3, r0, 24
b @800776C4
@80077580
mr r3, r28
bl fn_8011F4A8
lha r0, 0x2(r29)
clrlwi r5, r3, 24
srwi r3, r5, 31
srawi r4, r0, 31
subfc r0, r5, r0
adde r0, r4, r3
clrlwi r3, r0, 24
b @800776C4
@800775A8
mr r3, r28
bl fn_8011F1A0
mr r30, r3
bl fn_8006B420
clrlwi r0, r30, 16
mr r31, r3
cmpwi r0, 0xaf
beq @800775E0
bge @800775E8
cmpwi r0, 0x0
beq @800775D8
b @800775E8
@800775D8
li r3, 0x1
b @800775F0
@800775E0
li r3, 0x0
b @800775F0
@800775E8
mr r3, r30
bl fn_80142984
@800775F0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80077604
li r3, 0x0
b @800776C4
@80077604
lwz r0, 0x8(r31)
cmpwi r0, 0x1
beq @80077634
bge @80077620
cmpwi r0, 0x0
bge @8007762C
b @800776A0
@80077620
cmpwi r0, 0x3
bge @800776A0
b @80077648
@8007762C
li r3, 0x1
b @800776C4
@80077634
clrlwi r0, r30, 16
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r3, r0, 24
b @800776C4
@80077648
lis r3, lbl_802EE458@ha
lwz r0, lbl_80478928@sda21(r0)
addi r5, r3, lbl_802EE458@l
li r4, 0x0
clrlwi r3, r30, 16
mtctr r0
cmplwi r0, 0x0
ble @80077698
@80077668
lhz r0, 0x0(r5)
cmplw r3, r0
bne @8007768C
add r3, r31, r4
lbz r0, 0x18(r3)
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r3, r0, 24
b @800776C4
@8007768C
addi r5, r5, 0x2
addi r4, r4, 0x1
bdnz @80077668
@80077698
li r3, 0x1
b @800776C4
@800776A0
li r3, 0x0
b @800776C4
@800776A8
lis r3, lbl_80268A48@ha
lis r5, lbl_80268A58@ha
addi r3, r3, lbl_80268A48@l
li r4, 0xfb
addi r5, r5, lbl_80268A58@l
bl fn_80196E10
li r3, 0x0
@800776C4
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
