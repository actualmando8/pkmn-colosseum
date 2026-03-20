stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
lis r3, lbl_803FB318@ha
slwi r30, r29, 2
addi r0, r3, lbl_803FB318@l
add r31, r0, r30
subi r31, r31, 0x4
lwz r0, 0x0(r31)
cmpwi r0, 0x2
beq @8008ADB8
bge @8008AC80
cmpwi r0, 0x1
bge @8008AC8C
b @8008ADD4
@8008AC80
cmpwi r0, 0x4
bge @8008ADD4
b @8008AD3C
@8008AC8C
subi r3, r29, 0x1
addi r4, r1, 0x8
bl fn_80073A44
cmpwi r3, 0x0
bne @8008ACC8
slwi r7, r29, 1
li r6, lbl_8047A684@sda21
li r0, lbl_8047A67C@sda21
lhz r5, 0x8(r1)
add r4, r0, r7
add r6, r6, r7
li r0, 0x0
sth r5, -0x2(r4)
sth r0, -0x2(r6)
b @8008ADEC
@8008ACC8
cmpwi r3, 0x2
bgt @8008ACFC
slwi r0, r29, 1
li r4, lbl_8047A684@sda21
add r5, r4, r0
lhz r4, -0x2(r5)
addi r4, r4, 0x1
clrlwi r0, r4, 16
sth r4, -0x2(r5)
cmplwi r0, 0xa
bgt @8008ACFC
li r3, 0x0
b @8008ADEC
@8008ACFC
slwi r7, r29, 1
li r0, lbl_8047A67C@sda21
add r8, r0, r7
li r6, 0x0
subi r8, r8, 0x2
lis r4, lbl_803FB308@ha
sth r6, 0x0(r8)
addi r4, r4, lbl_803FB308@l
li r0, lbl_8047A684@sda21
add r5, r4, r30
stw r6, 0x0(r31)
add r4, r0, r7
stw r6, -0x4(r5)
sth r6, -0x2(r4)
sth r6, 0x0(r8)
b @8008ADEC
@8008AD3C
subi r3, r29, 0x1
bl fn_80073990
cmpwi r3, 0x0
bne @8008AD64
slwi r0, r29, 1
li r4, lbl_8047A684@sda21
add r4, r4, r0
li r0, 0x0
sth r0, -0x2(r4)
b @8008ADA0
@8008AD64
cmpwi r3, 0x2
bgt @8008AD98
slwi r0, r29, 1
li r4, lbl_8047A684@sda21
add r5, r4, r0
lhz r4, -0x2(r5)
addi r4, r4, 0x1
clrlwi r0, r4, 16
sth r4, -0x2(r5)
cmplwi r0, 0xa
bgt @8008AD98
li r3, 0x0
b @8008ADA0
@8008AD98
li r0, 0x0
stw r0, 0x0(r31)
@8008ADA0
slwi r0, r29, 1
li r4, lbl_8047A67C@sda21
add r4, r4, r0
li r0, 0x0
sth r0, -0x2(r4)
b @8008ADEC
@8008ADB8
slwi r0, r29, 1
li r3, lbl_8047A67C@sda21
add r3, r3, r0
li r0, 0x0
sth r0, -0x2(r3)
li r3, 0x0
b @8008ADEC
@8008ADD4
slwi r0, r29, 1
li r3, lbl_8047A67C@sda21
add r3, r3, r0
li r0, 0x0
sth r0, -0x2(r3)
li r3, 0x1
@8008ADEC
lis r4, lbl_803FB308@ha
addi r0, r4, lbl_803FB308@l
add r4, r0, r30
stw r3, -0x4(r4)
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
