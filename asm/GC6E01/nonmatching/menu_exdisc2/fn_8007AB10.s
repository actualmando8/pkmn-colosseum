stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r31, r3
mr r30, r4
cmpwi r31, 0x0
bne @8007AB40
li r3, 0x0
b @8007B074
@8007AB40
lwz r0, 0x0(r30)
cmpwi r0, 0x0
bne @8007ABF0
li r3, 0x1
bl fn_800D0F44
subis r0, r3, 0x4
cmplwi r0, 0x0
beq @8007ABC0
cmplwi r31, 0x10
bgt @8007ABB8
lis r3, jumptable_802EE5C0@ha
slwi r0, r31, 2
addi r3, r3, jumptable_802EE5C0@l
lwzx r0, r3, r0
mtctr r0
bctr
li r3, 0x2
b @8007B074
li r3, 0x4
b @8007B074
li r3, 0x7
b @8007B074
li r3, 0x4
b @8007B074
li r3, 0xb
b @8007B074
li r3, 0x4
b @8007B074
li r3, 0x13
b @8007B074
@8007ABB8
li r3, 0x0
b @8007B074
@8007ABC0
li r0, 0x1
lis r3, lbl_803F7A30@ha
stw r0, 0x0(r30)
lis r5, lbl_802EE508@ha
addi r7, r5, lbl_802EE508@l
addi r3, r3, lbl_803F7A30@l
lbz r8, lbl_80478930@sda21(r0)
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x3
bl fn_8007B350
@8007ABF0
lis r3, lbl_803F7A30@ha
addi r29, r3, lbl_803F7A30@l
lbz r0, 0x345(r29)
cmplwi r0, 0x0
beq @8007B054
addi r3, r29, 0x28
addi r4, r1, 0x8
bl fn_800A1E54
li r0, 0x0
li r3, 0x1
stb r0, 0x345(r29)
bl fn_800D0F44
subis r0, r3, 0x4
cmplwi r0, 0x0
beq @8007AC34
li r0, 0xa
b @8007AC44
@8007AC34
lwz r0, 0x8(r1)
cmpwi r0, 0x0
bne @8007AC44
li r0, 0x0
@8007AC44
cmplwi r0, 0xa
bgt @8007B070
lis r3, jumptable_802EE594@ha
slwi r0, r0, 2
addi r3, r3, jumptable_802EE594@l
lwzx r0, r3, r0
mtctr r0
bctr
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x3
bl fn_8007B350
cmplwi r31, 0x10
bgt @8007B070
lis r3, jumptable_802EE550@ha
slwi r0, r31, 2
addi r3, r3, jumptable_802EE550@l
lwzx r0, r3, r0
mtctr r0
bctr
li r3, 0x2
b @8007B074
li r3, 0x4
b @8007B074
li r3, 0x7
b @8007B074
li r3, 0x4
b @8007B074
li r3, 0xb
b @8007B074
li r3, 0x4
b @8007B074
li r3, 0x13
b @8007B074
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x3
bl fn_8007B350
cmpwi r31, 0xf
bne @8007B070
li r3, 0x10
b @8007B074
cmpwi r31, 0x12
bne @8007AD64
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x2
bl fn_8007B350
cmpwi r31, 0x12
bne @8007AD5C
li r3, 0x0
b @8007B074
@8007AD5C
li r3, 0x12
b @8007B074
@8007AD64
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x3
bl fn_8007B350
cmplwi r31, 0xc
bgt @8007B070
lis r3, jumptable_802EE51C@ha
slwi r0, r31, 2
addi r3, r3, jumptable_802EE51C@l
lwzx r0, r3, r0
mtctr r0
bctr
li r3, 0x10
b @8007B074
li r3, 0x11
b @8007B074
li r3, 0xb
b @8007B074
li r3, 0x4
b @8007B074
cmpwi r31, 0x3
beq @8007ADEC
cmpwi r31, 0xa
beq @8007ADEC
cmpwi r31, 0xc
beq @8007ADEC
cmpwi r31, 0xe
bne @8007AE2C
@8007ADEC
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x1
bl fn_8007B350
cmpwi r31, 0xe
bne @8007AE24
li r3, 0x0
b @8007B074
@8007AE24
li r3, 0xe
b @8007B074
@8007AE2C
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x3
bl fn_8007B350
cmpwi r31, 0x6
beq @8007AE80
bge @8007AE6C
cmpwi r31, 0x1
beq @8007AE78
b @8007B070
@8007AE6C
cmpwi r31, 0x8
beq @8007AE88
b @8007B070
@8007AE78
li r3, 0x2
b @8007B074
@8007AE80
li r3, 0x7
b @8007B074
@8007AE88
li r3, 0x4
b @8007B074
cmpwi r31, 0x1
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
lbz r8, lbl_80478930@sda21(r0)
addi r7, r4, lbl_802EE508@l
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r0, 0x0
beq @8007AF04
cmpwi r31, 0x3
beq @8007AF04
cmpwi r31, 0x6
beq @8007AF04
cmpwi r31, 0x8
beq @8007AF04
cmpwi r31, 0xa
beq @8007AF04
cmpwi r31, 0xc
beq @8007AF04
cmpwi r31, 0xe
beq @8007AF04
cmpwi r31, 0xf
beq @8007AF04
cmpwi r31, 0x11
beq @8007AF04
cmpwi r31, 0x12
bne @8007AF08
@8007AF04
li r0, 0x1
@8007AF08
cmpwi r0, 0x0
beq @8007AF18
li r9, 0x0
b @8007AF1C
@8007AF18
li r9, 0x3
@8007AF1C
bl fn_8007B350
b @8007B070
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x3
bl fn_8007B350
li r3, 0x14
b @8007B074
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x3
bl fn_8007B350
cmpwi r31, 0x15
beq @8007B070
li r3, 0x15
b @8007B074
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x3
bl fn_8007B350
cmpwi r31, 0x16
beq @8007B070
li r3, 0x16
b @8007B074
lis r3, lbl_803F7A30@ha
lis r4, lbl_802EE508@ha
addi r7, r4, lbl_802EE508@l
lbz r8, lbl_80478930@sda21(r0)
addi r3, r3, lbl_803F7A30@l
li r4, 0x1
li r5, lbl_8047A640@sda21
li r6, 0x4a
li r9, 0x3
bl fn_8007B350
cmpwi r31, 0x17
beq @8007B070
li r3, 0x17
b @8007B074
subi r0, r31, 0xe
li r3, 0x0
cmplwi r0, 0x1
stw r3, 0x0(r30)
ble @8007B020
cmpwi r31, 0x11
beq @8007B020
cmpwi r31, 0x12
bne @8007B070
@8007B020
li r3, 0x13
b @8007B074
subi r0, r31, 0xe
li r3, 0x0
cmplwi r0, 0x1
stw r3, 0x0(r30)
ble @8007B04C
cmpwi r31, 0x11
beq @8007B04C
cmpwi r31, 0x12
bne @8007B070
@8007B04C
li r3, 0x13
b @8007B074
@8007B054
lbz r0, 0x346(r29)
cmpwi r0, 0x2
bne @8007B070
cmpwi r31, 0xe
bne @8007B070
li r3, 0xf
b @8007B074
@8007B070
li r3, 0x0
@8007B074
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
