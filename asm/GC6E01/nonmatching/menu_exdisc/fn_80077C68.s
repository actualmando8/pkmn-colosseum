stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
stw r31, 0xc(r1)
stw r30, 0x8(r1)
mr r30, r3
bl fn_8006B420
clrlwi r0, r30, 16
mr r31, r3
cmpwi r0, 0xaf
beq @80077CAC
bge @80077CB4
cmpwi r0, 0x0
beq @80077CA4
b @80077CB4
@80077CA4
li r3, 0x1
b @80077CBC
@80077CAC
li r3, 0x0
b @80077CBC
@80077CB4
mr r3, r30
bl fn_80142984
@80077CBC
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80077CD0
li r3, 0x0
b @80077D70
@80077CD0
lwz r0, 0x8(r31)
cmpwi r0, 0x1
beq @80077D00
bge @80077CEC
cmpwi r0, 0x0
bge @80077CF8
b @80077D6C
@80077CEC
cmpwi r0, 0x3
bge @80077D6C
b @80077D14
@80077CF8
li r3, 0x1
b @80077D70
@80077D00
clrlwi r0, r30, 16
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r3, r0, 24
b @80077D70
@80077D14
lis r3, lbl_802EE458@ha
lwz r0, lbl_80478928@sda21(r0)
addi r4, r3, lbl_802EE458@l
li r5, 0x0
clrlwi r3, r30, 16
mtctr r0
cmplwi r0, 0x0
ble @80077D64
@80077D34
lhz r0, 0x0(r4)
cmplw r3, r0
bne @80077D58
add r3, r31, r5
lbz r0, 0x18(r3)
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r3, r0, 24
b @80077D70
@80077D58
addi r4, r4, 0x2
addi r5, r5, 0x1
bdnz @80077D34
@80077D64
li r3, 0x1
b @80077D70
@80077D6C
li r3, 0x0
@80077D70
lwz r0, 0x14(r1)
lwz r31, 0xc(r1)
lwz r30, 0x8(r1)
mtlr r0
addi r1, r1, 0x10
blr
