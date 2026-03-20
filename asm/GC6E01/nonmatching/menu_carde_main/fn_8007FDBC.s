stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r20, 0x10(r1)
mr r29, r3
mr r20, r4
lis r4, lbl_80268B88@ha
li r3, 0x500
addi r31, r4, lbl_80268B88@l
li r4, 0x20
bl fn_800E2C04
mr r28, r3
clrlwi r0, r28, 16
cmplwi r0, 0x0
bne @8007FE08
addi r3, r31, 0x1f0
li r4, 0x1a2
li r5, lbl_8047C140@sda21
bl fn_80196E10
@8007FE08
mr r3, r28
bl fn_800E27B0
li r4, 0x0
mr r28, r3
li r5, 0x4e8
bl memset
cmplwi r20, 0x0
mr r30, r28
beq @8007FE48
mr r3, r28
mr r4, r20
li r5, 0x50
bl fn_800F9D24
li r0, 0x0
sth r0, 0x9e(r28)
b @8007FE50
@8007FE48
li r0, 0x0
sth r0, 0x0(r28)
@8007FE50
lwz r3, 0xb0(r30)
cmplwi r3, 0x0
beq @8007FE98
bl fn_800E202C
mr r26, r3
clrlwi r0, r26, 16
cmplwi r0, 0x0
bne @8007FE80
addi r3, r31, 0x1f0
li r4, 0x1ab
li r5, lbl_8047C140@sda21
bl fn_80196E10
@8007FE80
mr r3, r26
bl fn_800E24B0
mr r3, r26
bl fn_800E209C
li r0, 0x0
stw r0, 0xb0(r30)
@8007FE98
li r3, 0x0
bl fn_80083BF8
mr r22, r3
stw r3, 0xac(r30)
cmpwi r22, 0x0
beq @8007FF48
slwi r26, r22, 2
li r4, 0x20
addi r0, r26, 0x1f
clrrwi r3, r0, 5
bl fn_800E2C04
mr r27, r3
clrlwi r0, r27, 16
cmplwi r0, 0x0
bne @8007FEE4
addi r3, r31, 0x1f0
li r4, 0x1a2
li r5, lbl_8047C140@sda21
bl fn_80196E10
@8007FEE4
mr r3, r27
bl fn_800E27B0
mr r28, r3
mr r5, r26
li r4, 0x0
bl memset
li r27, 0x0
stw r28, 0xb0(r30)
mr r26, r27
b @8007FF28
@8007FF0C
mr r4, r27
li r3, 0x0
bl fn_80083AF4
lwz r4, 0xb0(r30)
addi r27, r27, 0x1
stwx r3, r4, r26
addi r26, r26, 0x4
@8007FF28
cmpw r27, r22
blt @8007FF0C
lis r4, fn_8007FD64@ha
lwz r3, 0xb0(r30)
addi r6, r4, fn_8007FD64@l
li r5, 0x4
mr r4, r22
bl fn_800CA620
@8007FF48
lwz r0, 0xac(r30)
cmpwi r0, 0x0
beq @8007FF5C
li r0, 0x0
b @8007FF60
@8007FF5C
li r0, -0x1
@8007FF60
stw r0, 0xa4(r30)
mr r3, r29
li r4, 0x79b
bl fn_801046C8
stw r3, 0x118(r30)
mr r3, r29
li r4, 0x79c
bl fn_801046C8
stw r3, 0x11c(r30)
mr r3, r29
li r4, 0x79d
bl fn_801046C8
stw r3, 0x120(r30)
mr r3, r29
li r4, 0x780
bl fn_801046C8
stw r3, 0x124(r30)
mr r3, r29
li r4, 0x781
bl fn_801046C8
stw r3, 0x128(r30)
mr r3, r29
li r4, 0x782
bl fn_801046C8
stw r3, 0x12c(r30)
mr r3, r29
li r4, 0x1193
bl fn_801046C8
stw r3, 0x130(r30)
mr r3, r29
li r4, 0x1195
bl fn_801046C8
stw r3, 0x134(r30)
mr r3, r29
li r4, 0x1194
bl fn_801046C8
stw r3, 0x138(r30)
mr r3, r29
li r4, 0x796
bl fn_801046C8
stw r3, 0x13c(r30)
mr r3, r29
li r4, 0x793
bl fn_801046C8
stw r3, 0x140(r30)
mr r3, r29
li r4, 0x797
bl fn_801046C8
stw r3, 0x144(r30)
mr r3, r29
li r4, 0x1196
bl fn_801046C8
stw r3, 0x148(r30)
mr r3, r29
li r4, 0x792
bl fn_801046C8
stw r3, 0x14c(r30)
mr r3, r29
li r4, 0x1126
bl fn_801046C8
stw r3, 0x150(r30)
mr r3, r29
li r4, 0x795
bl fn_801046C8
stw r3, 0x154(r30)
mr r3, r29
li r4, 0x791
bl fn_801046C8
stw r3, 0x158(r30)
mr r3, r29
li r4, 0x1125
bl fn_801046C8
stw r3, 0x15c(r30)
mr r3, r29
li r4, 0x799
bl fn_801046C8
stw r3, 0x160(r30)
mr r3, r29
li r4, 0x79a
bl fn_801046C8
stw r3, 0x164(r30)
mr r3, r29
li r4, 0x825
bl fn_801046C8
stw r3, 0x168(r30)
mr r3, r29
li r4, 0x826
bl fn_801046C8
stw r3, 0x16c(r30)
mr r24, r30
addi r25, r31, 0x0
addi r23, r31, 0x90
addi r22, r31, 0x120
li r21, 0x0
@800800D8
mr r26, r25
mr r27, r24
mr r28, r23
mr r31, r22
li r20, 0x0
@800800EC
lhz r4, 0x0(r26)
mr r3, r29
bl fn_801046C8
stw r3, 0x170(r27)
mr r3, r29
lhz r4, 0x0(r28)
bl fn_801046C8
stw r3, 0x3b0(r27)
mr r3, r29
lhz r4, 0x0(r31)
bl fn_801046C8
stw r3, 0x290(r27)
addi r26, r26, 0x48
addi r27, r27, 0x90
addi r28, r28, 0x48
addi r31, r31, 0x48
addi r20, r20, 0x1
cmpwi r20, 0x2
blt @800800EC
addi r25, r25, 0x2
addi r24, r24, 0x4
addi r23, r23, 0x2
addi r22, r22, 0x2
addi r21, r21, 0x1
cmpwi r21, 0x24
blt @800800D8
mr r3, r29
li r4, 0x119a
bl fn_801046C8
stw r3, 0x4d0(r30)
mr r3, r29
li r4, 0x11c2
bl fn_801046C8
stw r3, 0x4d4(r30)
mr r3, r29
li r4, 0x790
bl fn_801046C8
stw r3, 0x4d8(r30)
mr r3, r29
li r4, 0x798
bl fn_801046C8
stw r3, 0x4dc(r30)
mr r3, r29
li r4, 0x78f
bl fn_801046C8
stw r3, 0x4e0(r30)
mr r3, r29
li r4, 0x794
bl fn_801046C8
stw r3, 0x4e4(r30)
mr r3, r30
lwz r4, 0x200(r30)
lha r0, 0x50(r4)
sth r0, 0xce(r30)
lha r0, 0x52(r4)
sth r0, 0xd0(r30)
lha r0, 0x54(r4)
sth r0, 0xd4(r30)
lha r0, 0x56(r4)
sth r0, 0xd2(r30)
lwz r4, 0x440(r30)
lha r0, 0x50(r4)
sth r0, 0xd6(r30)
lha r0, 0x52(r4)
sth r0, 0xd8(r30)
lha r0, 0x54(r4)
sth r0, 0xdc(r30)
lha r0, 0x56(r4)
sth r0, 0xda(r30)
lwz r4, 0x320(r30)
lha r0, 0x50(r4)
sth r0, 0xde(r30)
lha r0, 0x52(r4)
sth r0, 0xe0(r30)
lha r0, 0x54(r4)
sth r0, 0xe4(r30)
lha r0, 0x56(r4)
sth r0, 0xe2(r30)
lwz r4, 0x118(r30)
lha r0, 0x50(r4)
sth r0, 0xe6(r30)
lha r0, 0x52(r4)
sth r0, 0xe8(r30)
lha r0, 0x54(r4)
sth r0, 0xec(r30)
lha r0, 0x56(r4)
sth r0, 0xea(r30)
lwz r4, 0x11c(r30)
lha r0, 0x50(r4)
sth r0, 0xee(r30)
lha r0, 0x52(r4)
sth r0, 0xf0(r30)
lha r0, 0x54(r4)
sth r0, 0xf4(r30)
lha r0, 0x56(r4)
sth r0, 0xf2(r30)
lwz r4, 0x120(r30)
lha r0, 0x50(r4)
sth r0, 0xf6(r30)
lha r0, 0x52(r4)
sth r0, 0xf8(r30)
lha r0, 0x54(r4)
sth r0, 0xfc(r30)
lha r0, 0x56(r4)
sth r0, 0xfa(r30)
lwz r4, 0x15c(r30)
lha r0, 0x50(r4)
sth r0, 0xfe(r30)
lha r0, 0x52(r4)
sth r0, 0x100(r30)
lha r0, 0x54(r4)
sth r0, 0x104(r30)
lha r0, 0x56(r4)
sth r0, 0x102(r30)
lwz r4, 0x154(r30)
lha r0, 0x50(r4)
sth r0, 0x106(r30)
lha r0, 0x52(r4)
sth r0, 0x108(r30)
lha r0, 0x54(r4)
sth r0, 0x10c(r30)
lha r0, 0x56(r4)
sth r0, 0x10a(r30)
lwz r4, 0x14c(r30)
lha r0, 0x50(r4)
sth r0, 0x10e(r30)
lha r0, 0x52(r4)
sth r0, 0x110(r30)
lha r0, 0x54(r4)
sth r0, 0x114(r30)
lha r0, 0x56(r4)
sth r0, 0x112(r30)
lmw r20, 0x10(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
