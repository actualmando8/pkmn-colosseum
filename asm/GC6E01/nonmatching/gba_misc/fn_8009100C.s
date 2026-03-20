stwu r1, -0x60(r1)
mflr r0
stw r0, 0x64(r1)
stmw r14, 0x18(r1)
mr r15, r3
lis r4, 0x6db
addi r4, r4, 0x1604
bl fn_800F9318
stw r3, lbl_8047A690@sda21(r0)
lis r4, 0x6db
mr r3, r15
addi r4, r4, 0x1001
bl fn_800F9318
stw r3, lbl_8047A694@sda21(r0)
li r3, 0x280
li r4, 0x1e0
bl fn_800E8FA0
lis r3, 0x6bc
addi r3, r3, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r24, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd24
mr r23, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd29
mr r22, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r21, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd24
mr r20, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd29
mr r19, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r18, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r17, r3
addi r3, r4, 0x400
bl fn_801CBA0C
mr r0, r3
mr r3, r15
mr r16, r0
mr r4, r24
bl fn_800F9318
li r4, 0x2
mr r14, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r14
bl fn_800E8FE8
mr r3, r14
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r15
mr r4, r23
bl fn_800F9318
li r4, 0x2
mr r14, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r14
bl fn_800E8FE8
mr r3, r14
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r15
mr r4, r22
bl fn_800F9318
li r4, 0x2
mr r14, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r14
bl fn_800E8FE8
mr r3, r14
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r15
mr r4, r21
bl fn_800F9318
li r4, 0x2
mr r14, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r14
bl fn_800E8FE8
mr r3, r14
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r15
mr r4, r20
bl fn_800F9318
li r4, 0x2
mr r14, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r14
bl fn_800E8FE8
mr r3, r14
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r15
mr r4, r19
bl fn_800F9318
li r4, 0x2
mr r14, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r14
bl fn_800E8FE8
mr r3, r14
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r15
mr r4, r18
bl fn_800F9318
li r4, 0x2
mr r14, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r14
bl fn_800E8FE8
mr r3, r14
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r15
mr r4, r17
bl fn_800F9318
li r4, 0x2
mr r14, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r14
bl fn_800E8FE8
mr r3, r14
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r15
mr r4, r16
bl fn_800F9318
li r4, 0x2
mr r14, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r14
bl fn_800E8FE8
mr r3, r14
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
lis r4, 0xc39
mr r3, r15
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
li r25, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @800912F8
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r25, r3
cmplwi r25, 0x1
bge @800912F8
li r25, 0x1
@800912F8
li r14, 0x0
b @8009130C
@80091300
bl fn_800F0308
bl fn_800D3088
add r14, r14, r3
@8009130C
cmplw r14, r25
blt @80091300
lis r3, 0xc38
addi r3, r3, 0x1000
bl fn_801CBA0C
lis r4, 0xc38
stw r3, 0x8(r1)
addi r3, r4, 0x1008
bl fn_801CBA0C
lis r4, 0xc38
mr r14, r3
addi r3, r4, 0x1001
bl fn_801CBA0C
lis r4, 0xc38
mr r31, r3
addi r3, r4, 0x1002
bl fn_801CBA0C
lis r4, 0xc38
mr r30, r3
addi r3, r4, 0x1003
bl fn_801CBA0C
lis r4, 0xc38
mr r29, r3
addi r3, r4, 0x1004
bl fn_801CBA0C
lis r4, 0xc38
mr r28, r3
addi r3, r4, 0x1005
bl fn_801CBA0C
lis r4, 0xc38
mr r27, r3
addi r3, r4, 0x1006
bl fn_801CBA0C
lis r4, 0xc38
mr r26, r3
addi r3, r4, 0x1007
bl fn_801CBA0C
mr r0, r3
lwz r6, 0x8(r1)
mr r3, r15
mr r4, r24
mr r25, r0
mr r5, r15
li r7, 0x0
bl fn_801845E4
mr r3, r15
mr r4, r23
mr r5, r15
mr r6, r14
li r7, 0x0
bl fn_801845E4
mr r3, r15
mr r4, r22
mr r5, r15
mr r6, r31
li r7, 0x0
bl fn_801845E4
mr r3, r15
mr r4, r21
mr r5, r15
mr r6, r30
li r7, 0x0
bl fn_801845E4
mr r3, r15
mr r4, r20
mr r5, r15
mr r6, r29
li r7, 0x0
bl fn_801845E4
mr r3, r15
mr r4, r19
mr r5, r15
mr r6, r28
li r7, 0x0
bl fn_801845E4
mr r3, r15
mr r4, r18
mr r5, r15
mr r6, r27
li r7, 0x0
bl fn_801845E4
mr r3, r15
mr r4, r17
mr r5, r15
mr r6, r26
li r7, 0x0
bl fn_801845E4
mr r3, r15
mr r4, r16
mr r5, r15
mr r6, r25
li r7, 0x0
bl fn_801845E4
mr r3, r24
li r4, 0x3
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r23
li r4, 0x5
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r22
li r4, 0xe
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r21
li r4, 0xf
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r20
li r4, 0x4
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r19
li r4, 0x4
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r18
li r4, 0xe
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r17
li r4, 0xe
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r16
li r4, 0xe
li r5, 0x0
li r6, 0x1
bl fn_801CB834
li r3, 0x1
bl fn_80176B48
li r3, 0x89
bl fn_800FF58C
li r3, 0x0
li r4, 0x0
bl fn_8011288C
lmw r14, 0x18(r1)
lwz r0, 0x64(r1)
mtlr r0
addi r1, r1, 0x60
blr
