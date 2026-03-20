stwu r1, -0x30(r1)
mflr r0
stw r0, 0x34(r1)
stmw r25, 0x14(r1)
mr r28, r3
lis r4, 0xce6
addi r4, r4, 0x1602
bl fn_800F9318
stw r3, lbl_8047A690@sda21(r0)
lis r4, 0xce6
mr r3, r28
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
li r30, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @8008F5A8
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r30, r3
cmplwi r30, 0x1
bge @8008F5A8
li r30, 0x1
@8008F5A8
li r29, 0x0
b @8008F5BC
@8008F5B0
bl fn_800F0308
bl fn_800D3088
add r29, r29, r3
@8008F5BC
cmplw r29, r30
blt @8008F5B0
lis r3, 0xce6
addi r3, r3, 0x1000
bl fn_801CB7C4
lis r4, 0xce6
mr r3, r28
addi r4, r4, 0x1000
bl fn_800F9318
mr r29, r3
li r4, 0x1
lwz r3, 0x144(r29)
bl fn_80118874
li r0, 0x0
lis r4, 0xce6
stw r0, 0x144(r29)
mr r3, r28
lfs f0, lbl_8047C1D4@sda21(r0)
addi r4, r4, 0x1004
stfs f0, 0x8(r1)
bl fn_800F9318
li r4, 0x0
mr r29, r3
bl fn_800ECCA8
mr r3, r29
addi r4, r1, 0x8
li r5, 0x0
bl fn_800EC4D0
lfs f1, 0x8(r1)
mr r3, r29
lfs f0, lbl_8047C1D8@sda21(r0)
li r4, 0x0
fsubs f0, f1, f0
stfs f0, 0x8(r1)
bl fn_800ECCA8
lfs f1, 0x8(r1)
mr r3, r29
bl fn_800ECA78
mr r3, r29
li r4, 0x0
bl fn_800ECB74
mr r3, r29
bl fn_800EC990
lis r4, 0x111b
mr r3, r28
addi r4, r4, 0x1400
bl fn_800F9318
lis r4, 0xce6
mr r29, r3
mr r3, r28
addi r4, r4, 0x1000
bl fn_800F9318
mr r4, r29
bl fn_800E3C08
lis r4, 0xce6
mr r3, r28
addi r4, r4, 0x1000
bl fn_800F9318
li r4, 0x4
bl fn_800E3C00
lis r3, 0xce6
li r4, 0x3
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x0
bl fn_801CB834
li r30, 0x32
bl fn_800D37CC
cmpwi r3, 0x32
bne @8008F6EC
lfs f1, lbl_8047C1E0@sda21(r0)
bl fn_800C46B0
mr r30, r3
cmplwi r30, 0x1
bge @8008F6EC
li r30, 0x1
@8008F6EC
li r29, 0x0
b @8008F700
@8008F6F4
bl fn_800F0308
bl fn_800D3088
add r29, r29, r3
@8008F700
cmplw r29, r30
blt @8008F6F4
lis r3, 0x6bc
addi r3, r3, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r31, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0x6be
mr r30, r3
addi r3, r4, 0x400
bl fn_801CBA0C
mr r0, r3
mr r3, r28
mr r29, r0
mr r4, r31
bl fn_800F9318
li r4, 0x2
mr r27, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r27
bl fn_800E8FE8
mr r3, r27
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r28
mr r4, r30
bl fn_800F9318
li r4, 0x2
mr r27, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r27
bl fn_800E8FE8
mr r3, r27
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r28
mr r4, r29
bl fn_800F9318
li r4, 0x2
mr r27, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r27
bl fn_800E8FE8
mr r3, r27
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
lis r4, 0xcf7
mr r3, r28
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
li r26, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @8008F818
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r26, r3
cmplwi r26, 0x1
bge @8008F818
li r26, 0x1
@8008F818
li r27, 0x0
b @8008F82C
@8008F820
bl fn_800F0308
bl fn_800D3088
add r27, r27, r3
@8008F82C
cmplw r27, r26
blt @8008F820
lis r3, 0xcee
addi r3, r3, 0x1000
bl fn_801CBA0C
lis r4, 0xcee
mr r26, r3
addi r3, r4, 0x1002
bl fn_801CBA0C
lis r4, 0xcee
mr r27, r3
addi r3, r4, 0x1001
bl fn_801CBA0C
mr r0, r3
mr r3, r28
mr r25, r0
mr r4, r31
mr r5, r28
mr r6, r26
li r7, 0x0
bl fn_801845E4
mr r3, r28
mr r4, r30
mr r5, r28
mr r6, r27
li r7, 0x0
bl fn_801845E4
mr r3, r28
mr r4, r29
mr r5, r28
mr r6, r25
li r7, 0x0
bl fn_801845E4
mr r3, r31
li r4, 0x3
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r30
li r4, 0x4
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r29
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
lmw r25, 0x14(r1)
lwz r0, 0x34(r1)
mtlr r0
addi r1, r1, 0x30
blr
