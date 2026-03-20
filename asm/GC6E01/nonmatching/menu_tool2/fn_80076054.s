stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r25, 0x14(r1)
mr r27, r3
mr r28, r4
lis r3, lbl_80268940@ha
li r25, 0x0
addi r31, r3, lbl_80268940@l
@80076078
li r26, 0x0
@8007607C
mr r3, r27
clrlwi r4, r26, 16
bl fn_8012AC08
mr r4, r25
bl fn_80076398
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800760AC
slwi r0, r25, 1
addi r3, r31, 0xfc
lhzx r3, r3, r0
b @80076320
@800760AC
addi r26, r26, 0x1
cmpwi r26, 0x6
blt @8007607C
addi r25, r25, 0x1
cmplwi r25, 0x6
blt @80076078
cmplwi r28, 0x0
bne @800760D4
li r3, 0x0
b @80076320
@800760D4
li r30, 0x0
@800760D8
li r29, 0x0
@800760DC
mr r3, r27
clrlwi r4, r29, 16
bl fn_8012AC08
mr r25, r3
li r26, 0x0
cmplwi r25, 0x0
beq @80076110
li r4, 0x0
li r5, 0x6e
li r6, 0x0
bl fn_8012640C
cmpwi r3, 0x0
bne @80076114
@80076110
li r26, 0x1
@80076114
cmpwi r26, 0x0
beq @80076124
li r0, 0x1
b @800762AC
@80076124
cmpwi r30, 0x1
beq @80076170
bge @8007613C
cmpwi r30, 0x0
bge @80076148
b @80076298
@8007613C
cmpwi r30, 0x3
bge @80076298
b @80076198
@80076148
mr r3, r25
bl fn_8011F4A8
clrlwi r5, r3, 24
lha r0, 0x0(r28)
srawi r4, r5, 31
srwi r3, r0, 31
subfc r0, r0, r5
adde r0, r4, r3
clrlwi r0, r0, 24
b @800762AC
@80076170
mr r3, r25
bl fn_8011F4A8
lha r0, 0x2(r28)
clrlwi r5, r3, 24
srwi r3, r5, 31
srawi r4, r0, 31
subfc r0, r5, r0
adde r0, r4, r3
clrlwi r0, r0, 24
b @800762AC
@80076198
mr r3, r25
bl fn_8011F1A0
mr r25, r3
bl fn_8006B420
clrlwi r0, r25, 16
mr r26, r3
cmpwi r0, 0xaf
beq @800761D0
bge @800761D8
cmpwi r0, 0x0
beq @800761C8
b @800761D8
@800761C8
li r3, 0x1
b @800761E0
@800761D0
li r3, 0x0
b @800761E0
@800761D8
mr r3, r25
bl fn_80142984
@800761E0
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @800761F4
li r0, 0x0
b @800762AC
@800761F4
lwz r0, 0x8(r26)
cmpwi r0, 0x1
beq @80076224
bge @80076210
cmpwi r0, 0x0
bge @8007621C
b @80076290
@80076210
cmpwi r0, 0x3
bge @80076290
b @80076238
@8007621C
li r0, 0x1
b @800762AC
@80076224
clrlwi r0, r25, 16
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r0, r0, 24
b @800762AC
@80076238
lis r3, lbl_802EE458@ha
lwz r0, lbl_80478928@sda21(r0)
addi r5, r3, lbl_802EE458@l
li r4, 0x0
clrlwi r3, r25, 16
mtctr r0
cmplwi r0, 0x0
ble @80076288
@80076258
lhz r0, 0x0(r5)
cmplw r3, r0
bne @8007627C
addi r0, r4, 0x18
lbzx r0, r26, r0
cntlzw r0, r0
srwi r0, r0, 5
clrlwi r0, r0, 24
b @800762AC
@8007627C
addi r5, r5, 0x2
addi r4, r4, 0x1
bdnz @80076258
@80076288
li r0, 0x1
b @800762AC
@80076290
li r0, 0x0
b @800762AC
@80076298
addi r3, r31, 0x108
addi r5, r31, 0x118
li r4, 0xfb
bl fn_80196E10
li r0, 0x0
@800762AC
clrlwi r0, r0, 24
cmplwi r0, 0x0
bne @800762C8
slwi r0, r30, 1
li r3, lbl_8047C0D0@sda21
lhzx r3, r3, r0
b @80076320
@800762C8
addi r29, r29, 0x1
cmpwi r29, 0x6
blt @800760DC
addi r30, r30, 0x1
cmplwi r30, 0x3
blt @800760D8
li r25, 0x0
@800762E4
mr r3, r27
mr r4, r28
mr r5, r25
bl fn_80076F2C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @80076310
slwi r0, r25, 1
li r3, lbl_8047C0D8@sda21
lhzx r3, r3, r0
b @80076320
@80076310
addi r25, r25, 0x1
cmplwi r25, 0x4
blt @800762E4
li r3, 0x0
@80076320
lmw r25, 0x14(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
