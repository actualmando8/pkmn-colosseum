stwu r1, -0x50(r1)
mflr r0
stw r0, 0x54(r1)
stmw r17, 0x14(r1)
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
li r22, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @8008D3CC
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r22, r3
cmplwi r22, 0x1
bge @8008D3CC
li r22, 0x1
@8008D3CC
li r21, 0x0
b @8008D3E0
@8008D3D4
bl fn_800F0308
bl fn_800D3088
add r21, r21, r3
@8008D3E0
cmplw r21, r22
blt @8008D3D4
lis r3, 0xce6
addi r3, r3, 0x1000
bl fn_801CB7C4
lis r4, 0xce6
mr r3, r31
addi r4, r4, 0x1000
bl fn_800F9318
mr r21, r3
li r4, 0x1
lwz r3, 0x144(r21)
bl fn_80118874
li r0, 0x0
lis r4, 0xce6
stw r0, 0x144(r21)
mr r3, r31
lfs f0, lbl_8047C1D4@sda21(r0)
addi r4, r4, 0x1004
stfs f0, 0x8(r1)
bl fn_800F9318
li r4, 0x0
mr r21, r3
bl fn_800ECCA8
mr r3, r21
addi r4, r1, 0x8
li r5, 0x0
bl fn_800EC4D0
lfs f1, 0x8(r1)
mr r3, r21
lfs f0, lbl_8047C1D8@sda21(r0)
li r4, 0x0
fsubs f0, f1, f0
stfs f0, 0x8(r1)
bl fn_800ECCA8
lfs f1, 0x8(r1)
mr r3, r21
bl fn_800ECA78
mr r3, r21
li r4, 0x0
bl fn_800ECB74
mr r3, r21
bl fn_800EC990
lis r4, 0x1120
mr r3, r31
addi r4, r4, 0x1400
bl fn_800F9318
lis r4, 0xce6
mr r21, r3
mr r3, r31
addi r4, r4, 0x1000
bl fn_800F9318
mr r4, r21
bl fn_800E3C08
lis r4, 0xce6
mr r3, r31
addi r4, r4, 0x1000
bl fn_800F9318
li r4, 0x4
bl fn_800E3C00
lis r3, 0x6af
addi r3, r3, 0x400
bl fn_801CBA0C
lis r4, 0x6bc
mr r30, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r24, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r25, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r26, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r27, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r29, r3
addi r3, r4, 0x400
bl fn_801CBA0C
mr r0, r3
mr r3, r31
mr r28, r0
mr r4, r30
bl fn_800F9318
li r4, 0x2
mr r21, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r21
bl fn_800E8FE8
mr r3, r21
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r24
bl fn_800F9318
li r4, 0x2
mr r21, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r21
bl fn_800E8FE8
mr r3, r21
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r25
bl fn_800F9318
li r4, 0x2
mr r21, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r21
bl fn_800E8FE8
mr r3, r21
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r26
bl fn_800F9318
li r4, 0x2
mr r21, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r21
bl fn_800E8FE8
mr r3, r21
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r27
bl fn_800F9318
li r4, 0x2
mr r21, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r21
bl fn_800E8FE8
mr r3, r21
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r29
bl fn_800F9318
li r4, 0x2
mr r21, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r21
bl fn_800E8FE8
mr r3, r21
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r28
bl fn_800F9318
li r4, 0x2
mr r21, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r21
bl fn_800E8FE8
mr r3, r21
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
lis r4, 0x1151
mr r3, r31
addi r4, r4, 0x1400
bl fn_800F9318
mr r21, r3
mr r3, r31
mr r4, r30
bl fn_800F9318
mr r4, r21
bl fn_800E3C08
mr r3, r31
mr r4, r30
bl fn_800F9318
li r4, 0x4
bl fn_800E3C00
lis r4, 0xcff
mr r3, r31
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
li r22, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @8008D72C
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r22, r3
cmplwi r22, 0x1
bge @8008D72C
li r22, 0x1
@8008D72C
li r21, 0x0
b @8008D740
@8008D734
bl fn_800F0308
bl fn_800D3088
add r21, r21, r3
@8008D740
cmplw r21, r22
blt @8008D734
lis r3, 0xd09
addi r3, r3, 0x1000
bl fn_801CBA0C
lis r4, 0xd09
mr r23, r3
addi r3, r4, 0x1001
bl fn_801CBA0C
lis r4, 0xd09
mr r22, r3
addi r3, r4, 0x1006
bl fn_801CBA0C
lis r4, 0xd09
mr r21, r3
addi r3, r4, 0x1002
bl fn_801CBA0C
lis r4, 0xd09
mr r20, r3
addi r3, r4, 0x1003
bl fn_801CBA0C
lis r4, 0xd09
mr r19, r3
addi r3, r4, 0x1004
bl fn_801CBA0C
lis r4, 0xd09
mr r18, r3
addi r3, r4, 0x1005
bl fn_801CBA0C
mr r0, r3
mr r3, r31
mr r17, r0
mr r4, r30
mr r5, r31
mr r6, r23
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r24
mr r5, r31
mr r6, r22
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r25
mr r5, r31
mr r6, r21
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r26
mr r5, r31
mr r6, r20
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r27
mr r5, r31
mr r6, r19
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r29
mr r5, r31
mr r6, r18
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r28
mr r5, r31
mr r6, r17
li r7, 0x0
bl fn_801845E4
lis r3, 0xce6
li r4, 0x1
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x0
bl fn_801CB834
mr r3, r30
li r4, 0x9
li r5, 0x0
li r6, 0x0
bl fn_801CB834
mr r3, r24
li r4, 0x7
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r25
li r4, 0xa
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r26
li r4, 0xa
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r27
li r4, 0xa
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r29
li r4, 0xa
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r28
li r4, 0xa
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
lmw r17, 0x14(r1)
lwz r0, 0x54(r1)
mtlr r0
addi r1, r1, 0x50
blr
