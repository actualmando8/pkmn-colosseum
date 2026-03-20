stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
mr r31, r3
bl fn_800A13F8
lis r4, lbl_803B6DE0@ha
stw r3, lbl_8047A600@sda21(r0)
addi r3, r4, lbl_803B6DE0@l
bl OSCreateAlarm
bl OSDisableInterrupts
lis r4, 0x8000
lis r5, 0x1062
lwz r0, 0xf8(r4)
lis r4, fn_80072684@ha
addi r5, r5, 0x4dd3
lis r6, lbl_803B6DE0@ha
srwi r0, r0, 2
addi r7, r4, fn_80072684@l
mulhwu r0, r5, r0
addi r5, r6, lbl_803B6DE0@l
mr r30, r3
mr r3, r5
li r5, 0x0
srwi r6, r0, 6
bl OSSetAlarm
lwz r3, lbl_8047A600@sda21(r0)
bl fn_800A221C
mr r3, r30
bl OSRestoreInterrupts
mr r3, r31
bl fn_800D0F44
subis r0, r3, 0x4
cmplwi r0, 0x0
beq @800725E0
li r30, 0x1
b @8007264C
@800725E0
mr r3, r31
addi r4, r1, 0x8
bl fn_8025F3F4
cmpwi r3, 0x0
beq @800725FC
li r30, 0x2
b @8007264C
@800725FC
lbz r0, 0x8(r1)
rlwinm r0, r0, 0, 28, 28
cmpwi r0, 0x0
bne @80072614
li r30, -0x1
b @8007264C
@80072614
mr r3, r31
addi r4, r1, 0xc
addi r5, r1, 0x8
bl fn_8025F584
cmpwi r3, 0x0
beq @80072634
li r30, 0x3
b @8007264C
@80072634
lwz r0, 0xc(r1)
cmplwi r0, 0x0
beq @80072648
li r30, 0x4
b @8007264C
@80072648
li r30, 0x0
@8007264C
cmpwi r30, 0x0
beq @8007265C
cmpwi r30, 0x3
blt @80072668
@8007265C
addi r3, r31, 0x1
li r4, 0x1
bl fn_8008ABE4
@80072668
mr r3, r30
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
mtlr r0
addi r1, r1, 0x20
blr
