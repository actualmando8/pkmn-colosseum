stwu r1, -0x60(r1)
mflr r0
stw r0, 0x64(r1)
stmw r15, 0x1c(r1)
mr r31, r3
lis r4, 0xce6
addi r4, r4, 0x1602
bl fn_800F9318
stw r3, lbl_8047A690@sda21(r0)
lis r4, 0xce6
mr r3, r31
addi r4, r4, 0x1002
bl fn_800F9318
stw r3, lbl_8047A694@sda21(r0)
li r3, 0x280
li r4, 0x1e0
bl fn_800E8FA0
lis r3, 0xce6
li r4, 0x0
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x0
bl fn_801CB834
li r18, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @80090184
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r18, r3
cmplwi r18, 0x1
bge @80090184
li r18, 0x1
@80090184
li r17, 0x0
b @80090198
@8009018C
bl fn_800F0308
bl fn_800D3088
add r17, r17, r3
@80090198
cmplw r17, r18
blt @8009018C
lis r3, 0xce6
addi r3, r3, 0x1000
bl fn_801CB7C4
li r18, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @800901D4
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r18, r3
cmplwi r18, 0x1
bge @800901D4
li r18, 0x1
@800901D4
li r17, 0x0
b @800901E8
@800901DC
bl fn_800F0308
bl fn_800D3088
add r17, r17, r3
@800901E8
cmplw r17, r18
blt @800901DC
lis r4, 0xce6
mr r3, r31
addi r4, r4, 0x1000
bl fn_800F9318
mr r17, r3
li r4, 0x1
lwz r3, 0x144(r17)
bl fn_80118874
li r0, 0x0
lis r4, 0xce6
stw r0, 0x144(r17)
mr r3, r31
lfs f0, lbl_8047C1D4@sda21(r0)
addi r4, r4, 0x1004
stfs f0, 0x8(r1)
bl fn_800F9318
li r4, 0x0
mr r17, r3
bl fn_800ECCA8
mr r3, r17
addi r4, r1, 0x8
li r5, 0x0
bl fn_800EC4D0
lfs f1, 0x8(r1)
mr r3, r17
lfs f0, lbl_8047C1D8@sda21(r0)
li r4, 0x0
fsubs f0, f1, f0
stfs f0, 0x8(r1)
bl fn_800ECCA8
lfs f1, 0x8(r1)
mr r3, r17
bl fn_800ECA78
mr r3, r17
li r4, 0x0
bl fn_800ECB74
mr r3, r17
bl fn_800EC990
lis r3, 0x6bc
addi r3, r3, 0x400
bl fn_801CBA0C
lis r4, 0xcea
mr r23, r3
addi r3, r4, 0x1000
bl fn_801CBA0C
mr r0, r3
mr r3, r31
mr r22, r0
mr r4, r23
bl fn_800F9318
li r4, 0x2
mr r17, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r17
bl fn_800E8FE8
mr r3, r17
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
lis r3, 0xd29
addi r3, r3, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r24, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd24
mr r25, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r26, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd24
mr r27, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd29
mr r30, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd24
mr r29, r3
addi r3, r4, 0x400
bl fn_801CBA0C
mr r0, r3
mr r3, r31
mr r28, r0
mr r4, r24
bl fn_800F9318
li r4, 0x2
mr r17, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r17
bl fn_800E8FE8
mr r3, r17
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r25
bl fn_800F9318
li r4, 0x2
mr r17, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r17
bl fn_800E8FE8
mr r3, r17
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r26
bl fn_800F9318
li r4, 0x2
mr r17, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r17
bl fn_800E8FE8
mr r3, r17
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r27
bl fn_800F9318
li r4, 0x2
mr r17, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r17
bl fn_800E8FE8
mr r3, r17
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r30
bl fn_800F9318
li r4, 0x2
mr r17, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r17
bl fn_800E8FE8
mr r3, r17
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r29
bl fn_800F9318
li r4, 0x2
mr r17, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r17
bl fn_800E8FE8
mr r3, r17
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r28
bl fn_800F9318
li r4, 0x2
mr r17, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r17
bl fn_800E8FE8
mr r3, r17
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
lis r4, 0xcf3
mr r3, r31
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
li r18, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @80090500
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r18, r3
cmplwi r18, 0x1
bge @80090500
li r18, 0x1
@80090500
li r17, 0x0
b @80090514
@80090508
bl fn_800F0308
bl fn_800D3088
add r17, r17, r3
@80090514
cmplw r17, r18
blt @80090508
lis r3, 0xcea
addi r3, r3, 0x1006
bl fn_801CBA0C
lis r4, 0xcea
mr r21, r3
addi r3, r4, 0x1007
bl fn_801CBA0C
lis r4, 0xcea
mr r20, r3
addi r3, r4, 0x1001
bl fn_801CBA0C
lis r4, 0xcea
mr r19, r3
addi r3, r4, 0x1002
bl fn_801CBA0C
lis r4, 0xcea
mr r18, r3
addi r3, r4, 0x1003
bl fn_801CBA0C
lis r4, 0xcea
mr r17, r3
addi r3, r4, 0x1004
bl fn_801CBA0C
lis r4, 0xcea
mr r16, r3
addi r3, r4, 0x1005
bl fn_801CBA0C
mr r0, r3
mr r3, r31
mr r15, r0
mr r4, r23
mr r5, r31
mr r6, r22
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r24
mr r5, r31
mr r6, r21
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r25
mr r5, r31
mr r6, r20
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r26
mr r5, r31
mr r6, r19
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r27
mr r5, r31
mr r6, r18
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r30
mr r5, r31
mr r6, r17
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r29
mr r5, r31
mr r6, r16
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r28
mr r5, r31
mr r6, r15
li r7, 0x0
bl fn_801845E4
mr r3, r23
li r4, 0x3
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r24
li r4, 0x4
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r25
li r4, 0x3
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r26
li r4, 0x4
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r27
li r4, 0x3
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r30
li r4, 0x5
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r29
li r4, 0x4
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r28
li r4, 0x4
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
lmw r15, 0x1c(r1)
lwz r0, 0x64(r1)
mtlr r0
addi r1, r1, 0x60
blr
