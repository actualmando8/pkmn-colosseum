stwu r1, -0xb30(r1)
mflr r0
stw r0, 0xb34(r1)
stw r31, 0xb2c(r1)
stw r30, 0xb28(r1)
stw r29, 0xb24(r1)
stw r28, 0xb20(r1)
mr r28, r3
mr r29, r4
lis r4, lbl_80267DD8@ha
addi r3, r1, 0x8
addi r31, r4, lbl_80267DD8@l
addi r4, r28, 0xb44
bl fn_8012AC64
lwz r4, 0x0(r31)
mr r3, r29
addi r5, r1, 0x8
bl fn_801F9CBC
addi r3, r1, 0x8
bl fn_8012A130
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8006AB28
addi r3, r31, 0x10
addi r5, r31, 0x7c
li r4, 0x258
bl fn_80196E10
@8006AB28
addi r3, r1, 0x8
li r4, 0x0
bl fn_8012A7B4
li r30, 0x0
b @8006AB68
@8006AB3C
mr r4, r30
addi r3, r1, 0x8
bl fn_8012AC08
mr r31, r3
bl fn_80077A5C
clrlwi r0, r3, 24
cmplwi r0, 0x0
bne @8006AB64
mr r3, r31
bl fn_8012086C
@8006AB64
addi r30, r30, 0x1
@8006AB68
clrlwi r0, r30, 16
cmplwi r0, 0x6
blt @8006AB3C
lhz r31, 0x2(r28)
mr r3, r28
li r4, 0x0
li r5, 0x1660
bl memset
sth r31, 0x2(r28)
addi r3, r28, 0x2c
addi r4, r1, 0x8
bl fn_8012AC64
addi r3, r28, 0xb44
addi r4, r1, 0x8
bl fn_8012AC64
sth r29, 0x0(r28)
clrlwi r4, r29, 16
lwz r3, lbl_80478F20@sda21(r0)
lwz r0, 0x0(r3)
cmplw r0, r4
bgt @8006ABC4
li r0, -0x1
b @8006AC04
@8006ABC4
cmpwi r4, 0x9
bge @8006ABDC
cmpwi r4, 0x1
beq @8006AC00
bge @8006ABF0
b @8006AC00
@8006ABDC
cmpwi r4, 0x30a
bge @8006AC00
cmpwi r4, 0x308
bge @8006ABF8
b @8006AC00
@8006ABF0
li r0, 0x1
b @8006AC04
@8006ABF8
li r0, 0x2
b @8006AC04
@8006AC00
li r0, 0x0
@8006AC04
stw r0, 0x4(r28)
lwz r0, 0xb34(r1)
lwz r31, 0xb2c(r1)
lwz r30, 0xb28(r1)
lwz r29, 0xb24(r1)
lwz r28, 0xb20(r1)
mtlr r0
addi r1, r1, 0xb30
blr
