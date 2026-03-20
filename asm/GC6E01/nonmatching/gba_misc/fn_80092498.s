stwu r1, -0x20(r1)
mflr r0
stw r0, 0x24(r1)
stw r31, 0x1c(r1)
stw r30, 0x18(r1)
stw r29, 0x14(r1)
mr r29, r3
lis r4, 0xb63
addi r4, r4, 0x1602
bl fn_800F9318
stw r3, lbl_8047A690@sda21(r0)
lis r4, 0xb63
mr r3, r29
addi r4, r4, 0x1002
bl fn_800F9318
stw r3, lbl_8047A694@sda21(r0)
li r3, 0x280
li r4, 0x1e0
bl fn_800E8FA0
lis r3, 0xb63
li r4, 0x0
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x0
bl fn_801CB834
li r30, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @80092524
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r30, r3
cmplwi r30, 0x1
bge @80092524
li r30, 0x1
@80092524
li r31, 0x0
b @80092538
@8009252C
bl fn_800F0308
bl fn_800D3088
add r31, r31, r3
@80092538
cmplw r31, r30
blt @8009252C
lis r3, 0xb63
addi r3, r3, 0x1000
bl fn_801CB7C4
lis r4, 0xb63
mr r3, r29
addi r4, r4, 0x1000
bl fn_800F9318
mr r31, r3
li r4, 0x1
lwz r3, 0x144(r31)
bl fn_80118874
li r0, 0x0
lis r4, 0x112b
stw r0, 0x144(r31)
mr r3, r29
addi r4, r4, 0x1400
bl fn_800F9318
lis r4, 0xb63
mr r31, r3
mr r3, r29
addi r4, r4, 0x1000
bl fn_800F9318
mr r4, r31
bl fn_800E3C08
lis r4, 0xb63
mr r3, r29
addi r4, r4, 0x1000
bl fn_800F9318
li r4, 0x4
bl fn_800E3C00
lis r4, 0xb83
mr r3, r29
addi r4, r4, 0x1800
li r5, 0x0
li r6, 0x0
bl fn_80176E0C
li r31, 0x1
bl fn_800D37CC
cmpwi r3, 0x32
bne @800925F8
lfs f1, lbl_8047C1D0@sda21(r0)
bl fn_800C46B0
mr r31, r3
cmplwi r31, 0x1
bge @800925F8
li r31, 0x1
@800925F8
li r30, 0x0
b @8009260C
@80092600
bl fn_800F0308
bl fn_800D3088
add r30, r30, r3
@8009260C
cmplw r30, r31
blt @80092600
lis r3, 0xb63
li r4, 0x0
addi r3, r3, 0x1000
li r5, 0x0
li r6, 0x0
bl fn_801CB834
li r3, 0x1
bl fn_80176B48
li r3, 0x83
bl fn_800FF58C
li r3, 0x0
li r4, 0x0
bl fn_8011288C
lwz r0, 0x24(r1)
lwz r31, 0x1c(r1)
lwz r30, 0x18(r1)
lwz r29, 0x14(r1)
mtlr r0
addi r1, r1, 0x20
blr
