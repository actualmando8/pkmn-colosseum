stwu r1, -0x40(r1)
mflr r0
stw r0, 0x44(r1)
stmw r21, 0x14(r1)
mr r31, r3
lis r4, 0x6dd
addi r4, r4, 0x1604
bl fn_800F9318
stw r3, lbl_8047A690@sda21(r0)
lis r4, 0x6dd
mr r3, r31
addi r4, r4, 0x1001
bl fn_800F9318
stw r3, lbl_8047A694@sda21(r0)
li r3, 0x280
li r4, 0x1e0
bl fn_800E8FA0
lis r3, 0xd24
addi r3, r3, 0x400
bl fn_801CBA0C
lis r4, 0xd24
mr r30, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd24
mr r29, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd24
mr r28, r3
addi r3, r4, 0x400
bl fn_801CBA0C
lis r4, 0xd24
mr r27, r3
addi r3, r4, 0x400
bl fn_801CBA0C
mr r0, r3
mr r3, r31
mr r26, r0
mr r4, r30
bl fn_800F9318
li r4, 0x2
mr r25, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r25
bl fn_800E8FE8
mr r3, r25
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r29
bl fn_800F9318
li r4, 0x2
mr r25, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r25
bl fn_800E8FE8
mr r3, r25
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r28
bl fn_800F9318
li r4, 0x2
mr r25, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r25
bl fn_800E8FE8
mr r3, r25
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r27
bl fn_800F9318
li r4, 0x2
mr r25, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r25
bl fn_800E8FE8
mr r3, r25
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
mr r3, r31
mr r4, r26
bl fn_800F9318
li r4, 0x2
mr r25, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r25
bl fn_800E8FE8
mr r3, r25
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
lis r4, 0xb88
mr r3, r31
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
li r24, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @8009231C
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r24, r3
cmplwi r24, 0x1
bge @8009231C
li r24, 0x1
@8009231C
li r25, 0x0
b @80092330
@80092324
bl fn_800F0308
bl fn_800D3088
add r25, r25, r3
@80092330
cmplw r25, r24
blt @80092324
lis r3, 0xb85
addi r3, r3, 0x1004
bl fn_801CBA0C
lis r4, 0xb85
mr r24, r3
addi r3, r4, 0x1003
bl fn_801CBA0C
lis r4, 0xb85
mr r25, r3
addi r3, r4, 0x1001
bl fn_801CBA0C
lis r4, 0xb85
mr r23, r3
addi r3, r4, 0x1002
bl fn_801CBA0C
lis r4, 0xb85
mr r22, r3
addi r3, r4, 0x1003
bl fn_801CBA0C
mr r0, r3
mr r3, r31
mr r21, r0
mr r4, r30
mr r5, r31
mr r6, r24
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r29
mr r5, r31
mr r6, r25
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r28
mr r5, r31
mr r6, r23
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r27
mr r5, r31
mr r6, r22
li r7, 0x0
bl fn_801845E4
mr r3, r31
mr r4, r26
mr r5, r31
mr r6, r21
li r7, 0x0
bl fn_801845E4
mr r3, r30
li r4, 0x6
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r29
li r4, 0x6
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r28
li r4, 0x8
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r27
li r4, 0x8
li r5, 0x0
li r6, 0x1
bl fn_801CB834
mr r3, r26
li r4, 0x7
li r5, 0x0
li r6, 0x1
bl fn_801CB834
li r3, 0x1
bl fn_80176B48
li r3, 0x83
bl fn_800FF58C
li r3, 0x0
li r4, 0x0
bl fn_8011288C
lmw r21, 0x14(r1)
lwz r0, 0x44(r1)
mtlr r0
addi r1, r1, 0x40
blr
