stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
stw r28, 0x10(r1)
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
li r29, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @8008FF24
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r29, r3
cmplwi r29, 0x1
bge @8008FF24
li r29, 0x1
@8008FF24
li r30, 0x0
b @8008FF38
@8008FF2C
bl fn_800F0308
bl fn_800D3088
add r30, r30, r3
@8008FF38
cmplw r30, r29
blt @8008FF2C
lis r3, 0xce6
addi r3, r3, 0x1000
bl fn_801CB7C4
lis r4, 0xce6
mr r3, r31
addi r4, r4, 0x1000
bl fn_800F9318
mr r30, r3
li r4, 0x1
lwz r3, 0x144(r30)
bl fn_80118874
li r0, 0x0
lis r4, 0xce6
stw r0, 0x144(r30)
mr r3, r31
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
lis r3, 0x6bd
addi r3, r3, 0x400
bl fn_801CBA0C
mr r0, r3
mr r3, r31
mr r28, r0
mr r4, r28
bl fn_800F9318
li r4, 0x2
mr r29, r3
bl fn_800E9108
lwz r4, lbl_8047A690@sda21(r0)
mr r3, r29
bl fn_800E8FE8
mr r3, r29
li r4, 0x1
li r5, lbl_8047A694@sda21
bl fn_800E900C
lis r4, 0xcf4
mr r3, r31
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
li r30, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @8009006C
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r30, r3
cmplwi r30, 0x1
bge @8009006C
li r30, 0x1
@8009006C
li r29, 0x0
b @80090080
@80090074
bl fn_800F0308
bl fn_800D3088
add r29, r29, r3
@80090080
cmplw r29, r30
blt @80090074
lis r3, 0xceb
addi r3, r3, 0x1000
bl fn_801CBA0C
mr r0, r3
mr r3, r31
mr r6, r0
mr r4, r28
mr r5, r31
li r7, 0x0
bl fn_801845E4
mr r3, r28
li r4, 0x2
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
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
lwz r28, 0x10(r1)
mtlr r0
addi r1, r1, 0x20
blr
